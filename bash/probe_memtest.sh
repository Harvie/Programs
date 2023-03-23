#!/bin/sh

prefix="RAM STATUS:"
unsure_ret=0

[ "$1" = "-f" ] && unsure_ret=1

get_meminfo() {
	field="$1"
	file="/proc/meminfo"
	grep ^"$field": "$file" | grep -o '[0-9]*' ||
		echo "$prefix Field $field not found in $file" >&2
}

memtest=$(get_meminfo EarlyMemtestBad)
ecccorr=$(get_meminfo HardwareCorrupted)

[ -z "$memtest" -o -z "$ecccorr" ] && {
	echo "$prefix Memtest not enabled in kernel"
	exit $unsure_ret
}

echo "$prefix EarlyMemtestBad:$memtest HardwareCorrupted:$ecccorr"
[ $memtest -gt 0 -o $ecccorr -gt 0 ] && {
	echo "$prefix Corrupted"
	exit 255
}
echo "$prefix OK"
exit 0
