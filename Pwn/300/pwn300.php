$res="Interesting Server\n\n";

$res.="It gives adress of some function but what exactly is going on?\n";
$res.="can u get the flag?\n";

$res.="\t\tTo connect u may use: nc 192.168.122.70 ".(4000+$team->id-1)."\n\n";

$file='/69d814388d35dcb0d82cc866b20e82a6';
$res.="Here you can download the executable that protects the file:\n";
$res.="<a href='$file'>$file</a>\n";

return $res;
