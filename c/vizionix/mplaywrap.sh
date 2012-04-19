#!/bin/bash
PATH="./:$PATH"
eval "$(mkxsock)"
mplayer -fs -wid "$XSOCK_WID" $@
