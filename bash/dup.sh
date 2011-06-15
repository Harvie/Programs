CONF=.dup.conf
[ -f "$CONF" ] && eval "$(cat "$CONF")" || { echo no "$CONF"; exit 23; }
export PASSPHRASE="$DUPLICITY_PASSPHRASE";

DUPLICITY_SOURCE="./"
DUPLICITY=duplicity
#BACKUP_TARGET="file:///tmp/bkp"

case "$1" in
	backup)
		"$DUPLICITY" $DFLAGS "$DUPLICITY_SOURCE" "$DUPLICITY_TARGET";;
	full)
		"$DUPLICITY" full $DUPLICITY_FLAGS "$DUPLICITY_SOURCE" "$DUPLICITY_TARGET";;
	verify)
		"$DUPLICITY" verify $DUPLICITY_FLAGS "$DUPLICITY_TARGET" "$DUPLICITY_SOURCE";;
	list)
		"$DUPLICITY" list-current-files "$DUPLICITY_TARGET";;
	status)
		"$DUPLICITY" collection-status "$DUPLICITY_TARGET";;
	cleanup)
		"$DUPLICITY" cleanup --force "$DUPLICITY_TARGET";;
	restore)
		echo	you can restore your files using this command:
		echo	"PASSPHRASE='$PASSPHRASE' '$DUPLICITY' restore '$DUPLICITY_TARGET' /tmp/restored";;
	man)
		man duplicity;;
	*|help)
		echo "
	###################################################################
	#
	#  $0 for backing-up using duplicity
	#  --> http://duplicity.nongnu.org/
	#
	#	$0          -	same as '$0 help'
	#	$0 backup   -	backup (full or incremental)
	#	$0 full     -	full backup
	#	$0 verify   -	compare local files with backup
	#	$0 list     -	list files on backup
	#	$0 status   -	print backup status
	#	$0 cleanup  -	remove extraneous files from backup
	#	$0 restore  -	print command to restore files
	#	$0 help     -	print this help
	#	$0 man      -	show duplicity manpage
	#
";;
esac

export PASSPHRASE=''
