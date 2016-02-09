$res=""; 
$res.="When you decrypt the following text, you will know how to encrypt your flag and ofc after that u will submit it to us\n\n";

$res.="To decrypt: 756e646f627d7269603334377d6c6534\n\n";

$target="";
$sql = "SELECT `target` FROM `crypt100` WHERE `teamID`=?";
$stmt = $db->prepare($sql); $stmt->bind_param("i", $team->id);
$stmt->bind_result($target);
$stmt->execute();
$stmt->store_result();
$stmt->fetch();
$stmt->close();

$res.="To encrypt: ".$target."\n\n";

return $res;

