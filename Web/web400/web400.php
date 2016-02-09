<?php
session_start();

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$flag = "e3d63a59d9ab4033652596a1fa56770f";



class USER_DRIVER {
	private $username;
	private $logged_in;
	private $file;
	private $filename;
	private $is_admin;
	public function __construct() {
		$this->filename = "somefile";
		$this->file = null;
		$this->logged_in = false;
		$this->username = null;
		$this->is_admin = false;
		if(isset($_SESSION['username'])) {
				$this->username = $_SESSION['username'];
				$this->logged_in = true;
		}
		if(isset($_SESSION['is_admin']))
			$this->is_admin = $_SESSION['is_admin'];
	}

	public function __destruct() {
		if($this->file != null) {
			fclose($this->file);
		}
	}
	public function login($u) {
		$this->logged_in = true;
		$this->username = $u;
		$_SESSION['username'] = $u;
	}

	public function logged_in() {
		return $this->logged_in;
	}

	public function is_admin() {
		return $this->is_admin;
	}

	public function logout() {
		$this->logged_in = false;
		$this->username = null;
		unset($_SESSION['username']);
		$this->is_admin = false;
		unset($_SESSION['is_admin']);
	}

	public function set_access_level($val) {
		$this->is_admin = (bool)$val;
		$_SESSION['is_admin'] = (bool)$val;
	}

	public function send_message($mess) {
		if(!$this->logged_in()) return;
		if($this->file == null) {
			$this->file = fopen($this->filename,"a+");
		}
		fwrite($this->file,htmlspecialchars($mess) . ",by " . htmlspecialchars($this->username) . " @ " .  date('m/d/Y h:i:s a', time()) . "<br/>");
	}

	public function print_messages() {
		echo file_get_contents($this->filename);
	}

	public function __toString() {
		echo var_dump($this);
		echo var_dump(get_class_methods("USER_DRIVER"));
	}
}

$UD = new USER_DRIVER();
if(isset($_POST['action'])) {
	$reflection = new ReflectionObject($UD);
	$method = $reflection->getMethod($_POST['action']);
	if($method->getNumberOfParameters() == 1) {
		if(isset($_POST['data'])) {
			$method->invoke($UD,$_POST['data']);
		}
	} else {
		$method->invoke($UD);
	}
}

if($UD->logged_in()) {
	$UD->print_messages();
	if($UD->is_admin()) {
		echo "<br/><br/>";
		echo "Hello, admin!<br/>";
		echo "Flag: " . $flag . "<br/>";
	}
	echo '<form action="" method="post">
		<textarea name="data"></textarea>
		<input type="submit" value="Send"/>
		<input type="hidden" name="action" value="send_message"/>
		</form>
		';
	echo '<form action="" method="post">
			<input type="hidden" name="action" value="logout"/>
			<input type="submit" value="Leave"/>
		</form>
	';
} else {
	echo '<form action="" method="post">
		<input type="hidden" name="action" value="login"/>
		Choose a username:
		<input type="text" name="data"/>
		<input type="submit" value="Join"/>
		</form>
	';
}

?>

		
