#!/bin/bash
#This script generates directory structure based on
#Dewey Decimal System Categories list copypasted from wikipedia...
#Generated directory structure can be converted to JSON tree for further use

ddslist='dds.txt'
ddsdir='./dds'

rm -r "$ddsdir"

dds_get_path() {
	for i in {1..3}; do
		id=$(
			echo -n "$1" | grep -Eo "[0-9]{$i}" | head -n 1;
			#echo 000 | grep -Eo "[0-9]{$(( 3-$i ))}" | head -n 1;
		)
		zeros=000
		echo "${id}${zeros:$i}";
	done
}

dds_get_category() {
	cat dds.txt | grep ^"$1" | head -n "$2" | tail -n 1
}

dds_get_category_name() {
	dds_get_category "$1" "$2" | cut -d ' ' -f 2- | grep -o '[a-zA-Z\[].*'
}

dds_add_category() {
	path="$2"
	level=1;
	dds_get_path "$1" | while read id; do
		name="$(dds_get_category_name "$id" "$level")";
		path+="/$id - $name";
		[ $level -eq 3 ] && {
			mkdir -p "$path";
			echo "$path";
		}
		level=$(( $level+1 ));
	done
}

time for i in {000..999}; do
	dds_add_category "$i" "$ddsdir";
done
