echo pings:
for i in $(seq 0 255); do
	host=$(echo "$1" | sed -e "s/\*/$i/g");
	echo -ne "$host\r";
	ping -bc 1 -W 1 "$host" &>/dev/null &
	pids="$! $pids";
done;
echo
echo pongs:
wait $pids;
arp -a | grep -v incomplete
