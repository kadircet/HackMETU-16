$res=""; 
$res.="You were eavesdropping on a network and intercepted some diffie-hellman keypairs as following\n\n";

$res.="(1099511627791, 152351, 565537237703) and (1099511627791, 152351, 894246743889)\n\n";

$target="";
$sql = "SELECT `target` FROM `crypt200` WHERE `teamID`=?";
$stmt = $db->prepare($sql);
$stmt->bind_param("i", $team->id);
$stmt->bind_result($target);
$stmt->execute();
$stmt->store_result();
$stmt->fetch();
$stmt->close();

$res.="After those packages you intercepted an ecrypted message: ".$target.". Can you decrypt it?\n\n";

return $res;
