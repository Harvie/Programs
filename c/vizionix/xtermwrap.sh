#!/bin/bash
. getxsock.sh
xterm -xrm "XTerm.vt100.allowSendEvents:true" -into "$XSOCK_WID" -bg black -fg white $@
