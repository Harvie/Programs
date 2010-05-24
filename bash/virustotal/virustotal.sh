#!/bin/bash

protocol='https'; #https?

#<p>The answer to your request is located <a href="/reanalisis.html?031765e5cda19bbc8deb2aba9385b5da1e2d2586488553a400cf2474f4c655df-1243110100">here</a>.</p>
#<p>The answer to your request is located <a href="/analisis/6cba671c2012a2bc8931b00808c3635953eb28e02737e33b4cfddeaa66fd415c-1243110122">here</a>.</p>
#curl --location -F "archivo=@good" -F "distribuir=1" 'http://www.virustotal.com/vt/en/recepcion';

echo "Uploading file..."
location=$(curl -# -F "archivo=@bad" -F "distribuir=1" "$protocol://www.virustotal.com/vt/en/recepcion" | grep -o '<a href=".*">' | cut -d '"' -f 2);

if [[ "$?" == "0" ]] && [[ "$location" =~ /(.*)[\?/](.*) ]]; then
	vthash="${BASH_REMATCH[2]}";
else
	echo hoops;
	exit 2;
fi;

vturl="$protocol://www.virustotal.com/analisis/$vthash";
echo $vturl;


#curl -d hash=$(md5sum bad) "https://www.virustotal.com/vt/en/consultamd5"
