<h1>ASIIArt Evolution Matrix</h1>
<?php

$aart='';
$width=20;
$height=10;
$childs=14;
$chars='.*+oO ';
$ratio=9; //probability 1/ratio

if(isset($_POST['aart']))	$aart=$_POST['aart'];
if(isset($_POST['width']))      $width=$_POST['width'];
if(isset($_POST['height']))     $height=$_POST['height'];
if(isset($_POST['childs']))     $childs=$_POST['childs'];
if(isset($_POST['chars']))      $chars=$_POST['chars'];
if(isset($_POST['ratio']))      $ratio=$_POST['ratio'];

function mkchild($text, &$chars, $ratio) {
	if($text == '') {
		for($i=0;$i<$GLOBALS['height'];$i++) {
			for($j=0;$j<$GLOBALS['width'];$j++) {
				echo($chars[rand(0,strlen($chars)-1)]);
			}
			echo("\n");
		}
	} else {
		if(!rand(0,$GLOBALS['childs']*0.7)) {
			for($i=0;$i<strlen($chars);$i++) {
				$c=$chars[rand(0,strlen($chars)-1)];
				$text=str_replace($chars[rand(0,strlen($chars)-1)], $c, $text);
			}
		}

		if(!rand(0,$GLOBALS['childs']*0.4)) {
			$c=''; $d=0;
			for($i=0;$i<strlen($chars);$i++) {
				if(substr_count($text, $chars[$i]) > $d) {
					$c = $chars[$i]; $d = substr_count($text, $chars[$i]);
				}
			}
			$text=str_replace($chars[rand(0,strlen($chars)-1)], $c, $text);
		}

		for($i=0;$i<strlen($text);$i++){
			if(!rand(0,$ratio) && $text[$i]!="\n" && $text[$i]!="\r") {
				$text[$i]=$chars[rand(0,strlen($chars)-1)];
			}
		}
		echo($text);
	}
}

srand(time(0));
?>
	<form action="?" method="POST" style="display: inline;">
		<textarea name="aart" cols="<?=$width?>" rows="<?=$height?>" style="float: left; display: inline;"><?php echo($aart);	?></textarea><br />
		<input type="hidden" name="width" value="<?=$width ?>">
		<input type="hidden" name="height" value="<?=$height ?>">
		Childs <input type="text" name="childs" value="<?=$childs ?>">
		Charset <input type="text" name="chars" value="<?=$chars ?>">
		Replace 1/<input type="text" name="ratio" value="<?=$ratio ?>">
		<input type="submit" value="SET">
	</form>
	<br /><br />
	<form action="?" method="POST" style="float: left; display: inline;">
		Width <input type="text" name="width" value="<?=$width ?>">
		Height <input type="text" name="height" value="<?=$height ?>">
		<input type="hidden" name="childs" value="<?=$childs ?>">
		<input type="hidden" name="chars" value="<?=$chars ?>">
		<input type="hidden" name="ratio" value="<?=$ratio ?>">
		<input type="submit" value="RESET">
		<br /><br />
		<a href="https://kyberia.sk/id/4339876/">Info...</a>
	</form>
	<form action="?" method="POST" style="float: left; display: inline;">
		/<input type="submit" value="RESET SETTINGS">
	</form>

	<hr style="clear: both;"/>
<?php
for($i=0;$i<$childs;$i++) {
	?>
	<form action="?" method="POST" style="float: left; display: inline;">
		<textarea name="aart" cols="<?=$width?>" rows="<?=$height?>"><?php mkchild($aart, $chars, $ratio); ?></textarea><br />
		<input type="hidden" name="width" value="<?=$width ?>">
		<input type="hidden" name="height" value="<?=$height ?>">
		<input type="hidden" name="childs" value="<?=$childs ?>">
		<input type="hidden" name="chars" value="<?=$chars ?>">
		<input type="hidden" name="ratio" value="<?=$ratio ?>">
		<input type="submit" value="<?=$i?>">
	</form>
	<?php
}


