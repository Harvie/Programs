#!/bin/bash
jackd -Rd alsa &
jack_pid="$!";
pd -rt -jack $@;
kill -TERM "$jack_pid";
((
	sleep 1 &&
	kill -KILL "$jack_pid" > /dev/null 2>&1
) & )
