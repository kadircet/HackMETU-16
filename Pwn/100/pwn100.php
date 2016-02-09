$res="Echo Server\n\n";

$res.="Hi bro, i found an echo service on my victim i believe it can be expolited\n";
$res.="but could not manage to do so. Could you help me on that??\n";

$res.="\t\tTo connect u may use: nc 192.168.122.70 ".(2000+$team->id-1)."\n\n";


return $res;
