#!/bin/bash
kill -0 $XSOCK_PID || {
	PATH="./:$PATH"
	eval "$(mkxsock)"
}
export XSOCK_PID XSOCK_WID
