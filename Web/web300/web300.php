<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$flag = "ehe flag";
$dbname = "web300";
$dbuser = "web300suser";
$dbpassword = "susersuser";

function random_32_generate() {
	$pool = "abcdef0123456789";
	$tt = "";
	for($i=0;$i<31;$i++) {
		$tt.=$pool[rand(0,strlen($pool)-1)];
	}
	return $tt . "o";
}

function random_26_generate($odd = false) {
	$pool = "qwertyuopasdfghjklizxcvbnm0123456789";
	$tt = "";
	for($i=0;$i<26;$i++) {
		$tt .= $pool[rand(0,strlen($pool)-1)];
	}
	if($odd)
		$tt .= "o";
	return $tt;
}

$pdo = null;
try {
	$pdo = new PDO("mysql:dbname=$dbname;host=localhost",$dbuser,$dbpassword);
} catch(PDOException $e) {
	echo "Database baglantisi kurulamadi.Boyle birsey olmamaliydi.Bir gorevli cagiriniz.";
	die();
}

$created = $pdo->exec("CREATE TABLE IF NOT EXISTS users(
			id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
			username VARCHAR(30) NOT NULL,
			password_md5ed VARCHAR(50) NOT NULL
		)
	    ");
$pdo->exec("CREATE TABLE IF NOT EXISTS sessions(
			user_id INT PRIMARY KEY NOT NULL,
			session_id VARCHAR(30) NOT NULL
		)
		");
$pdo->exec("INSERT INTO users (id,username,password_md5ed) VALUES (1,'admin','".random_32_generate()."')");
$pdo->exec("INSERT INTO sessions (user_id,session_id) VALUES (1,'".random_26_generate(true)."')");




$logged_in = false;
$username = null;
$password = null;
$user_id = null;
$sessid = null;

function make_login($un,$pw) {
	global $pdo;
	$prepared = $pdo->prepare("SELECT id FROM users WHERE username = :un AND password_md5ed = :pass");
	$prepared->bindValue(":un",$un,PDO::PARAM_STR);
	$prepared->bindValue(":pass",md5($pw),PDO::PARAM_STR);
	$prepared->execute();
	if($prepared->rowCount() == 0) {
		return array("fail" => array("message" => "Wrong username/password combination."));
	}
	$uid = $prepared->fetch(PDO::FETCH_ASSOC)['id'];
	$sid = random_26_generate();
	$pdo->query("INSERT INTO sessions (user_id,session_id) VALUES ($uid,'$sid') ON DUPLICATE KEY UPDATE session_id='$sid'");
	setcookie("PHPSESSID", $sid, time()+60*60*24*30); // 30days
	return array("success" => array("username" => $un, "sessid" => $sid, "user_id" => $uid));
}

function make_register($un,$pw) {
	global $pdo;
	$prepared = $pdo->prepare("SELECT COUNT(*) as c FROM users WHERE username = :un");
	$prepared->bindValue(":un",$un,PDO::PARAM_STR);
	$prepared->execute();
	$count = $prepared->fetch(PDO::FETCH_ASSOC)['c'];
	if($count != 0) {
		return array("fail" => array("message" => "Username already registered."));
	}

	$prepared = $pdo->prepare("INSERT INTO users (username,password_md5ed) VALUES (:un,:pw)");
	$prepared->bindValue(":un",$un,PDO::PARAM_STR);
	$prepared->bindValue(":pw",md5($pw),PDO::PARAM_STR);
	$prepared->execute();
	return array("success" => true);
}

function make_login_sid($sid) {
	global $pdo;
	$res = $pdo->query("SELECT users.username as username,sessions.session_id as sessid,users.id as user_id FROM sessions,users WHERE sessions.user_id = users.id AND sessions.session_id = '$sid'");
	if($res->rowCount() == 0) {
		return array("fail" => array("message" => "Invalid session id"));
	}
	$arr = $res->fetch(PDO::FETCH_ASSOC);
	return array("success" => $arr);
}

function make_remove_sid($sid) {
	global $pdo;
	$prepared = $pdo->prepare("DELETE FROM sessions WHERE session_id = :sid");
	$prepared->bindValue(":sid", $sid, PDO::PARAM_STR);
	$prepared->execute();
}


if(isset($_GET['action']) && $_GET['action'] == 'login') {
	if(!isset($_POST['username']) || !isset($_POST['password'])) {
		die("Username or password field is empty.Login failed.");
	}
	$username = (string)$_POST['username'];
	$password = (string)$_POST['password'];
	if($username == '' || $password == '') {
		die("Username or password field is empty.Login failed.");
	}
	$res = make_login($username,$password);
	if(array_key_exists("success",$res)) {
		$logged_in = true;
		$username = $res["success"]["username"];
		$sessid = $res["success"]["sessid"];
		$user_id = $res["success"]["user_id"];
	} else {
		die($res["fail"]["message"]);
	}
} else if (isset($_GET['action']) && $_GET['action'] == 'register') {
	if(!isset($_POST['username']) || !isset($_POST['password'])) {
		die("Username or password field is empty.Register failed.");
	}
	$username = (string)$_POST['username'];
	$password = (string)$_POST['password'];
	if($username == "" || $password == "") {
		die("Username or password field is empty.Register failed.");
	}
	$res = make_register($username,$password);
	if(array_key_exists("success",$res)) {
		$log_res = make_login($username,$password);
		if(array_key_exists("success",$res)) {
			$logged_in = true;
			$username = $log_res["success"]["username"];
			$sessid = $log_res["success"]["sessid"];
			$user_id = $log_res["success"]["user_id"];
		} else {
			die("Unexpected.<br/>");
		}
	} else {
		die($res["fail"]["message"]);
	}
} else if(isset($_GET['action']) && $_GET['action'] == 'logout') {
	if(isset($_COOKIE['PHPSESSID']) && !empty($_COOKIE['PHPSESSID'])) {
		make_remove_sid($_COOKIE['PHPSESSID']);
		setcookie("PHPSESSID", "", time()-3600);
	}
} else if (isset($_COOKIE['PHPSESSID']) && !empty($_COOKIE['PHPSESSID'])) {
	$res = make_login_sid($_COOKIE['PHPSESSID']);
	if(array_key_exists("success",$res)) {
		$logged_in = true;
		$username = $res["success"]["username"];
		$sessid = $res["success"]["sessid"];
		$user_id = $res["success"]["user_id"];
	}
}

if($logged_in) {
	if($username == 'admin' && $user_id == 1) {
		echo "Flag: " . $flag . "<br/>";
	}
	echo "Hello, " . $username . "!<br/>";
	echo '
		<form action="?action=logout" method="post">
			<input type="submit" value = "Logout"/>
		</form>
	';
} else {
	echo '
		<form action="?action=login" method="post">
			Username: <br/>
			<input type="text" name="username"/><br/>
			Password:<br/>
			<input type="password" name="password"/><br/>
			<input type="submit" value="Login"/>
		</form><br/><br/><br/><br/>
	';
	echo '
		<form action="?action=register" method="post">
			Username: <br/>
			<input type="text" name="username"/><br/>
			Password: <br/>
			<input type="password" name="password"/><br/>
			<input type="submit" value="Register"/>
		</form>
	';
}


?>
