mm() {
	for i in $(seq 0 10); do
		xdotool mousemove_relative -- $1 $2;
		sleep 0.00;
	done;
}

#sleep 5; xdotool mousedown 1; mm 2 1; mm 2 -1; mm -2 -1; mm -2 1; mm 0 2; mm 2 1; mm 0 -2; mm 0 2; mm 2 -1; mm 0 -2; xdotool mouseup 1
#sleep 5; xdotool mousedown 1; xdotool mousemove_relative 100 100; xdotool mouseup 1

down() { xdotool mousedown 1; }
up() { xdotool mouseup 1; }
mvs() { xdotool mousemove_relative -- $(bc<<<$1*$3) $(bc<<<$2*$3); }
mv() { mvs $1 $2 20; }

sleep 3;
down;
mv 2 1;
mv 2 -1;
mv -2 -1;
mv -2 1;
mv 0 1;
mv 2 1;
mv 0 -1;
mv 0 1;
mv 2 -1;
mv 0 -1;
up;
