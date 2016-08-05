#!/bin/sh

while getopts ":a:b:c:d:f" OPT; do
	echo "$OPT ==> $OPTARG";
	case $OPT in
		a  ) ahoj=$OPTARG;;

		'?') echo "Unknown option: -$OPTARG"			>&2; exit 1;;
		':') echo "Missing option argument for -$OPTARG"	>&2; exit 2;;
		*  ) echo "Unimplemented option: -$OPT"			>&2; exit 3;;
	esac
done

echo
shift $(($OPTIND -1)); echo "Rest: $@";

: ${ahoj:=default}
echo ahoj: $ahoj
