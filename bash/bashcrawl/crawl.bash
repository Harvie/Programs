#!/bin/bash

CURL_PATH="curl";
CURL_TIMEOUT=1;
#CURL_ARGS=--get --location --connect-timeout "$CURL_TIMEOUT" --max-time "$CURL_TIMEOUT" --compressed --stderr curl.debug;
    
#        $cmd = "$curl --url \"$url\" $args";
        
    
grab_urls() {
	if [[ $( "$CURL_PATH" "--url" "$1" --get --location --connect-timeout "$CURL_TIMEOUT" --max-time "$CURL_TIMEOUT" --compressed --stderr curl.debug) =~ \(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~\&=\ ?\.a-z0-9\/]*\) ]]; then
		for i in $( seq 0 $[ ${#BASH_REMATCH}-1 ] ); do
			# echo $i;
  		if [ "${BASH_REMATCH[i]}" != "" ]; then
  			echo "${BASH_REMATCH[i]}";
  		fi;
  	done | sort -u;
	fi;
}

grab_urls http://www.abclinuxu.cz/
