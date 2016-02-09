$res="Message Logging Server\n\n";

$res.="Hi bro, i found a logging service on my victim i believe it can be expolited\n";
$res.="but could not manage to do so. Could you help me on that??\n";

$res.="\t\tTo connect u may use: nc 192.168.122.70 ".(3000+$team->id-1)."\n\n";

$file='/5ac15b90fa6f601589fe2ba90c810b51';
$res.="Here you can download the executable that protects the file:\n";
$res.="<a href='$file'>$file</a>\n";

return $res;

return $res;
