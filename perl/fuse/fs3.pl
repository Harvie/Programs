#!/usr/bin/env perl

use Fuse;

#use JSON;
#print encode_json {};

my ($mountpoint) = "";
$mountpoint = shift(@ARGV) if @ARGV;
Fuse::main(mountpoint=>$mountpoint, getattr=>"main::my_getattr", getdir=>"main::my_getdir");

sub my_getattr {
	#($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size, $atime,$mtime,$ctime,$blksize,$blocks)
	#return (1,1,0777,1,0,0,1,1,1,1,1,1,1);
	my ($size) = 1024;
	my ($modes) = (0040<<9) + 0755; #dir=0040, file=0100
	my ($dev, $ino, $rdev, $blocks, $gid, $uid, $nlink, $blksize) = (0,0,0,1,0,0,1,1024);
	my ($atime, $ctime, $mtime);
	$atime = $ctime = $mtime = 666666;
	return ($dev,$ino,$modes,$nlink,$uid,$gid,$rdev,$size,$atime,$mtime,$ctime,$blksize,$blocks);
}

sub my_getdir {
	return ('.', 'a', 'b', 0);
}
