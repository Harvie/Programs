#!/bin/bash
BROWSER='epiphany';
BROWSER='firefox' 
BROWSARGS='-width 300 -height 150';
DIALOG='gdialog';

lastfm-play() {
	URL="http://www.last.fm/webclient/popup/?radioURL=lastfm%3A%2F%2Fartist%2F$1%2Fsimilarartists&resourceID=undefined&resourceType=undefined";
	"$BROWSER" $BROWSARGS "$URL";
}

tmpfile="/tmp/lastfm-$$.tmp";
"$DIALOG" --backtitle 'Last.fm tuner by <~~Harvie 2oo8' --title 'Search' --inputbox "Artist" 0 0 2>"$tmpfile";
if [ "$?" != 1 ]; then
	lastfm-play "$(cat "$tmpfile")";
fi;	            
             
