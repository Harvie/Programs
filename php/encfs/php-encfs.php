<?php
/*
	Harvie 2oo9 - http://blog.harvie.cz/

	This simple script allows you to encrypt you webpages/etc.
	using EncFS (you need to have EncFS installed of course).

	You need write permissions to mountpoint for www server
	and maybe add www server's user to the fuse group:

	gpasswd fuse -a www-data
	or
	gpasswd fuse -a http

	If you want this script to mount EncFS to be accessible by
	other users, you can add user_allow_other to /etc/fuse.conf
*/

if(trim($_SERVER['HTTPS']) == '') {
	die('<h1>Error: Cannot use EncFS without TLS!</h1>Use https:// instead of http://');
}

function encfs_mount($what, $where, $password, $arguments='') {
	system('/bin/echo '.escapeshellarg($password).' | /usr/bin/encfs -S '.escapeshellarg($what).' '.escapeshellarg($where)." $arguments".' 2>&1');
	flush();
}

function encfs_unmount($where) {
	system('/usr/bin/fusermount -zu '.escapeshellarg($where).' 2>&1');
	flush();
}

