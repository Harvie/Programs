find /etc/ | grep '\.pacnew$' | sed -e 's/\.pacnew$//g' | while read i; do
	echo meld "'$i'" "'$i.pacnew'";
done;
