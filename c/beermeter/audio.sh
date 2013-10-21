#!/bin/sh
PATH="$PATH:./"
arecord -c 1 -f S8 -r 8000 -t raw 2>/dev/null | audio
