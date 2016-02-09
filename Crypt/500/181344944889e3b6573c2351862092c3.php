<?php
require_once("base.php");

if($loggedin!==true)
{
	echo "Login first :/";
	exit();
}

if(isset($_GET['inp']))
{
	if(strlen($_GET['inp'])!==8)
		echo "len must be 8<br/>\n";
	else
	{
		$p=popen("python cpu.py ".escapeshellarg($_GET['inp'])." ".$team->id, "r");
		$out = fread($p, 4096);
		$out = explode("\n", $out);
		$x=array();
		for($i=0;$i<count($out);$i++)
			$x[]=$i+1;
		$x=implode(", ", $x);
		$y=implode(", ", $out);
		echo '<div id="myDiv"></div>';
		echo '<script type="text/javascript" src="https://cdn.plot.ly/plotly-latest.min.js"></script>';
		echo "<script>var trace={x:[".$x."], y:[".$y."], type:'scatter'};";
		echo "var data = [trace];Plotly.newPlot('myDiv', data);</script>";
	}
}

echo "<form action='#' method='get'>\n";
echo "<input type='textbox' name='inp'>\n";
echo "<input type='submit' value='submit' name='submit'>\n";
echo "</form>";

?>
