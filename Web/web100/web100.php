<?php
session_start();

$flag = "ehe flag";
$password = "asdfqwe";


if(isset($_GET['action']) && $_GET['action'] === 'logout') {
	$_SESSION['logged_in'] = false;
}

function generate_new_login_token() {
	$pool = "abcdefghijklmnopqrstuvwxyz0123456789:.'!?/*=}%+^'!!'^+%/()=?_^";
	$token_length = 32;
	$token = "";
	for($i=0;$i<$token_length;$i++) {
		$token .= $pool[rand(0, strlen($pool)-1)];
	}

	$_SESSION['stored_token'] = $token;
}

echo "Shuffled password: " . str_shuffle($password) ."<br/>";

$logged_in = isset($_SESSION['logged_in']) && $_SESSION['logged_in'];
if(isset($_GET['action']) && $_GET['action'] == 'login') {
	if(!isset($_POST['token']) || $_POST['token'] !== $_SESSION['stored_token'] || $_POST['password'] !== $password || $_POST['username'] != 'admin') {
		echo "Login failed.";
	} else {
		echo "Login successful.<br/>\n";
		$print_form = false;
		$logged_in = true;
		$_SESSION['logged_in'] = true;
	}
}
if(!$logged_in) {
	generate_new_login_token();
	$form = '
			<form action="?action=login" method="post">
				Username:
				<input type="text" name="username"/><br/><br/>
				Password:
				<input type="password" name="password"/><br/><br/>
				<input type="hidden" name="token" value="'.$_SESSION['stored_token'].'"/>
				<input type="submit" value = "Login"/>
			</form>
		';
	echo $form;	
} else {
	echo "Flag: ". $flag;

	echo '
		<form action="?action=logout" method="post">
			<input type="submit" value = "Logout"/>
		</form>
		';
}
