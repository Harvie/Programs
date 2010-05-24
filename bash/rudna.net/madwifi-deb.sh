#!/bin/sh
apt-get update;
apt-get install apt dpkg;
apt-get install doc-debian debian-policy debmake devscripts fakeroot dh-make debhelper;
apt-get install make g++ gcc perl autoconf automake sed libc6-dev pbuilder;

tar -xvvzf "$1".tar.gz;
cd "$1";

