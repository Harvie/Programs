#!/bin/bash
#Harvie's textmode work accelerator

echo -n 'Loading TurboBASH...';

#settings
SENSIBLE-EDITOR='/usr/bin/nano';
CC='/usr/bin/gcc';
CPP='/usr/bin/g++';

#fcs

ec() {
  "$SENSIBLE-EDITOR" "$1".c;
	"$CC" -o "$1" "$1".c;
}

ecpp() {
  "$SENSIBLE-EDITOR" "$1".cpp;
	"$CPP" -o "$1" "$1".cpp;
}


#EOF
echo ' [DONE]';
