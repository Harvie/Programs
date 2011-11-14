#!/bin/bash
gen_line() {
	echo -e '\t"'$(traceroute -q1 -w2 -z500 "$1" 2>/dev/null | cut -d ' ' -f-4 | grep -C 1 -o '[0-9a-zA-Z\.-\_]*' | grep -v '^[0-9]*$' | sed -e ':a;N;$!ba;s/\n/" -> "/g')'"' |
		#grep -v '*' |
		 grep '\->' | grep -v '""'
}
export -f gen_line;

echo 'strict digraph G {'
time cat ~/Temp/hosts/hostsbrevnov/hosts | sort -u | grep ^[0-9] | cut -f 1 |
	xargs -n 1 -P 200 bash -c 'gen_line "$@"' --
echo '}'
