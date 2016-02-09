$res="Interesting Server\n\n";

$res.="I must change my name bro, i must change it\n";
$res.="after that i might get the flag..\n";

$res.="\t\tTo connect u may use: nc 192.168.122.70 ".(5000+$team->id-1)."\n\n";

$file='/b99c2f3799ce7a9ff7460abb5d507a72';
$res.="Here you can download the executable that protects the file:\n";
$res.="<a href='$file'>$file</a>\n";

return $res;
