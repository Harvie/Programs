#!/bin/bash

# killall -2 wine
# killall -9 wine
netstat -a -e -p -c | grep wine &
lsof -c wine &

