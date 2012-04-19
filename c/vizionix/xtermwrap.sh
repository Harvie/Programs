#!/bin/bash
PATH="./:$PATH"
eval "$(mkxsock)"
xterm -xrm "XTerm.vt100.allowSendEvents:true" -into "$XSOCK_WID" $@

