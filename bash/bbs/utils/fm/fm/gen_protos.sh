#!/bin/sh

TYPES='void|char|int|File|gboolean|GNode|GList|WINDOW'

egrep -h '^('$TYPES').*;$' *.c | sed 's/^/extern /'
echo

for file in *.c; do
	echo '/* ' $file ' */'
	egrep '^('$TYPES').*(\)|,)$' $file | grep -v '^int main(.*)' | sed 's/)$/);/'
	echo
done
echo '#endif'
