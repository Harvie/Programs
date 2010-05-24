<form name="mydata" action="<?php echo($_POST["serviceurl"]); ?>" method="POST">
<?php
//$POST["serviceurl"] = redirect adresa

function form_submit($name) {
	echo("\n\n<script type=\"text/javascript\">document.".$name.".submit()</script>");
}

///CODE//////////////////////////

$out = "\r\n\r\n";
foreach($_POST as $ind => $val) {
	$out = $out.$ind." = ".$val."\r\n";
	echo("\n<input type=\"hidden\" name=\"".$ind."\" value=\"".$val."\" />");
}
echo("\r\n<input type=\"submit\" name=\"mydatasubmit\">\r\n</form>");

$fp = fopen("log.txt", "a");
fwrite($fp, $out);
fclose($fp);

//Debug
echo("<!--\r\n");
print_r($_POST);
echo("\r\n-->");

//form_submit("mydata");
header("Location: ".$_POST["serviceurl"]);

?>
