#!/bin/bash
#KISS YouTube video downloader
#<~~Harvie 2oo8

echo "Harvie's automatic YouTube downloader";
echo " Now go and watch something on YouTube...";

download_video() {
 	VIDFILE="$2"."flv";
 	DLURL="$1";
 	
 	echo Downloading captured video to "$VIDFILE" ...;
 	if [[ -a "$VIDFILE" ]]; then
 		echo File "$VIDFILE" already exists!
 	else
 		curl --url "$DLURL" -o "$VIDFILE" --location -s  2> /dev/null && echo "->" "$VIDFILE" complete! &
 		# wget "$DLURL" -O "$VIDFILE" 2> /dev/null && echo "->" "$VIDFILE" complete! &
 		echo Download started at background!
 	fi;
}

#remove "-p" option for ngrep to download videos wieved from other computers...
ngrep $@ -p port 80 | while read line; do

  if [[ "$line" =~ "GET /get_video\?video_id=(.*)&t=(.*) HTTP" ]]; then
  	DLURL="http://www.youtube.com/get_video?video_id=${BASH_REMATCH[1]}&t=${BASH_REMATCH[2]}";
  	WURL="http://www.youtube.com/watch?v=${BASH_REMATCH[1]}"

		echo ---------------------------------------------------;
		echo;
  	echo Captured video download URL: "$DLURL";
  	echo Captured video watch URL: "$WURL";
		echo Getting video info...;

	  curl "$WURL" 2> /dev/null | grep "<title>" | while read iline; do
	  	if [[ "$iline " =~ "<title>YouTube - (.*)</title>" ]]; then
	  		VIDTITLE=$( echo "${BASH_REMATCH[1]}" );
  			echo Captured video title: "$VIDTITLE";
  			download_video "$DLURL" "$VIDTITLE";
  			break;
  		fi;
  	done;
  fi;

done;
