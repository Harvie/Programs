#!/bin/bash
TMPFILE="/tmp/xsock-$USER";
[ -f "$TMPFILE" ] && . "$TMPFILE"
[ -n "$XSOCK_PID" ] && kill -0 "$XSOCK_PID" 2>/dev/null || {
	PATH="./:$PATH"
	eval "$(mkxsock $@ | tee $TMPFILE)"
	echo "XSOCK_PID set to $XSOCK_PID" >&2
}
export XSOCK_PID XSOCK_WID
