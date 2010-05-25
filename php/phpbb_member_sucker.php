<?php
$url		= 'http://harvie.ath.cx/wan/student-ssps.cz/board/memberlist.php';
$preg		= "/[-a-z0-9\._]+@[-a-z0-9\._]+\.[a-z]{2,4}/";
$start_user	= 0;
$max_users	= 200;
$step = 50;

$mblist = $url.'?start=';
for($i=$start_user;$i<$max_users;$i+=$step) {
	$url = $mblist.$i;
	preg_match_all($preg, @file_get_contents($url), $users); $users = $users[0];
	foreach($users as $user) echo($user."\n");
}