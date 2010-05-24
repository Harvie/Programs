#!/bin/bash
id=76000;
pw='Beruska';

cookie_jar='/tmp/kybcacookies';
echo "loging in to kyberia";
curl -s -c "$cookie_jar" -b "$cookie_jar" --location --url 'http://kyberia.sk/id/'"$id" -d 'event=login&login='"$id"'&password='"$pw"'&login_type=id' >/dev/null 2>&1
for i in -30 -20 -10 -5; do 
  echo "kyberia k_wallet $i";
  curl -s -c "$cookie_jar" -b "$cookie_jar" --location --url 'http://kyberia.sk/id/'"$id" -d 'event=k_wallet&k_wallet='"$i" >/dev/null 2>&1
done;

