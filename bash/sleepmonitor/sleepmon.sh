#!/bin/sh
# Simple sleep monitor (Harvie 2012)
#
# You probably have soundcard with one output and input
# Take PIR motion sensor from security system
# Use PIR to switch connection between left channels of output and input (tips of 3.5mm jacks)
# Connect grounds of these two together (maybe you will not need it - depending on your soundcard)
# Power up PIR
# Launch this script
# Use alsamixer and some GUI recorder (like audacity) to tune volume to some usable level
# Place PIR facing to your bed and go sleep
# Wake up
# Enjoy your data

out=/tmp/sleepmon-"$(date +%F_%r)".wav
speaker-test -t sine &>/dev/null &
time arecord -r 4000 "$out"
kill $!
echo "Your file: $out"
