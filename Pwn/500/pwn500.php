$res="Numbers only\n\n";

$res.="Just cat the flag\n";
$res.="\t\tTo connect u may use: nc 192.168.122.70 ".(6000+$team->id-1)."\n\n";

$file='/d75ff083f9805f9bface1ad38c1f1644';
$res.="Here you can download the executable that protects the file:\n";
$res.="<a href='$file'>$file</a>\n";

return $res;
