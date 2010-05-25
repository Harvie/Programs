<?php
require_once('./php-encfs.php');

$encrypted = '/mnt/data1/domains/kyberia.cz/encfs/a';		//encfs encrypted data
$plaintext = '/mnt/data1/domains/kyberia.cz/encfs/b';		//plaintext mountpoint
$umountpasswd = '';			//password for unmounting (can't be same as mounting password)
$encfsargs = '--public';		//arguments passed to encfs

?>
<h1><a href="?">EncFS Web Interface</a></h1>
<?php echo("encfs '<b>$encrypted</b>' '$plaintext' $encfsargs<br /><br />"); ?>

<style>/*<!--*/
input {
	width:200px;
}
/*-->*/</style>

<form action="?" method="POST">
	<input type="submit" value="MOUNT (DECRYPT)" />
	<input type="password" name="mnt" value="" />
</form>
<form action="?" method="POST">
	<input type="submit" value="UNMOUNT (ENCRYPT)" />
	<input type="password" name="umnt" value="" />
</form>
Brougt to you by <a href="http://blog.harvie.cz/">Harvie</a> 2oo9!

<br /><pre>
<?php

if(isset($_POST['mnt']) && isset($_POST['umnt']))
	die('Error: Cannot do both mount and umount!');

if(isset($_POST['mnt'])) {
	if($_POST['mnt'] == $umountpasswd)
		die('Error: It is VERY dangerous to have same mount nad unmount password!!! Change it!');
	if(trim($_POST['mnt']) == '')
		die('Error: Mount password cannot be empty!!! Change it!');

	encfs_mount($encrypted, $plaintext, $_POST['mnt'], $encfsargs);
}

if(isset($_POST['umnt'])) {
	if($_POST['umnt'] == $umountpasswd) {
		encfs_unmount($plaintext);
	} else {
		echo('Error: Invalid unmount password. You can change it in .php file.');
	}
}

?>
</pre>
