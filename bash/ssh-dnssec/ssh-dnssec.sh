#!/bin/sh
drill="$(which drill)"
ssh="$(which ssh)"

check_ssh_cmdline() {
	while getopts "a:c:e:i:l:n:k:V:o:p:q:P:t:v:x:C:L:R:h" OPT; do
		if [ "$OPT" == 'h' ]; then
			echo "$0 help"; echo "
	This is DNSSEC wrapper for OpenSSH client which will simply prevent you
	from connecting to hosts with fraudent DNS records.

	You can use alias ssh='$0' (and you can add it to your ~/.bashrc)

	Command line options are just the same as for SSH, but you have to
	specify all the options before hostname and optional command. eg.:
		$0 -p2222 user@example.com		(good)
		$0 user@example.com -p2222		(baad)

	To test if $0 works as it's supposed to be working, you can try following:
		$0 user@badsign-a.test.dnssec-tools.org
		$0 user@rhybar.cz
		(both commands should fail with DNSSEC error)

"
			"$ssh" --help
			exit 0;
		fi;
	done
	shift $(($OPTIND -1));
	host="${1##*@}";
	echo "$drill -TD $host"
	out="$("$drill" -TD "$host")"; ret=$?;
	echo "$out" | grep -i NO.DNSKEY;
	return $ret;
}

if check_ssh_cmdline $@; then
	echo -e 'DNSSEC verification OK :-)\n'
	echo "ssh $@";
	"$ssh" $@;
else
	echo 'DNSSEC verification FAILED!'
	exit 1;
fi;
