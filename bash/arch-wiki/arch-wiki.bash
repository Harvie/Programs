#!/bin/bash
#arch-wiki-docs-search
#requires arch-wiki-docs package

wiki_title='Archers Guide to Galaxy - DO NOT PANIC!';
html_docs='/usr/share/doc/arch-wiki/html/';

search="harvie";
search=$(zenity --entry --title="$wiki_title" --text='Enter regular expression to search');

file=$(
	fgrep -Ri "$search" "$html_docs" | cut -d ':' -f 1 | sort -u | while read file; do
		title=$(fgrep -m 1 -i '<title>' "$file" | head -n 1 | cut -d '>' -f 2- | cut -d '<' -f 1);
		echo -ne "FALSE\x00$title\x00$file\x00";
	done | xargs -0 zenity --width=800 --height=300 --list --title="$wiki_title" --text='Select page to view' --radiolist --column 'X' --column 'Page' --column 'File' --print-column=3;
);

echo retu: $?;
echo file: $file;


