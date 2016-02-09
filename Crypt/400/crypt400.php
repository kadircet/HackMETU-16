$res=""; 
$res.="You must decrypt the following RSA encrypted message, but all you know is, it has been encrypted with";
$res.=" one of the public keys in the attachment :/\n\n";
$res.="e is 65537 for all keys\n\n";

$res.="<a href='/1b2624d510c924b147ccfac372f4d143.txt'>attachment</a>\n\n";

$target="";
$sql = "SELECT `target` FROM `crypt400` WHERE `teamID`=?";
$stmt = $db->prepare($sql);
$stmt->bind_param("i", $team->id);
$stmt->bind_result($target);
$stmt->execute();
$stmt->store_result();
$stmt->fetch();
$stmt->close();

$res.="The message you need to decrypt is: ".$target.". Can you decrypt it?\n\n";

return $res;
