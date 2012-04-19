#!/bin/bash
. getxsock.sh
mplayer -fs -wid "$XSOCK_WID" $@
