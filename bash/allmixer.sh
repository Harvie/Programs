#!/bin/sh
# call amixer for all cards in ALSA
# (I am calling this from XFCE keyboard shortcuts to set volume globally)
#
# Usage examples:
# ./allmixer.sh sset PCM 10%+
# ./allmixer.sh sset PCM 10%-

export LANG=C
for i in {0..99}; do
	cmd="amixer -c$i $@"
	echo === $cmd ===
	$cmd 2>&1 | grep 'Invalid card number' && break
done;
echo DONE
