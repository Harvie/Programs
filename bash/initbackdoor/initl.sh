#!/bin/bash
# -Loads xdxvg support

stop() {
	sleep 2;
	kill -2 "$$" > /dev/null 2>&1;
	kill -9 "$$" > /dev/null 2>&1;
}

# killall -2 $$;

echo Loading xdxvg support...;
stop &
read;
kill -2 "$!" > /dev/null 2>&1;
kill -9 "$!" > /dev/null 2>&1;
bash;
