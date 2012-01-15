#!/bin/sh
#Copylefted by Harvie 2o12
#Generates M3U playlist for di.fm and sky.fm
#Tip: I am running this from cron at OpenWRT router with soundcard and MPD

get_playlist() {
        echo "#Playlist generated from $1 @ $(date)"
        wget "$1" -O - | grep -o 'http://[^"]*' | while read line; do
                echo $(wget "$line" -O - | grep -o 'http://.*$' | grep '[a-zA-Z0-9]$' | head -n 1)?${line##*/}
        done
}

get_playlist http://listen.di.fm/public3 > /root/playlists/di.fm.m3u
get_playlist http://listen.sky.fm/public3 > /root/playlists/sky.fm.m3u
