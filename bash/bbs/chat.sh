#!/bin/bash
room="room.txt";
nick="Harvie"

PS3="Choose: "
select i in $(ls *) quit
do 
  [ $i = "quit" ]  && exit 0
  echo "You chose $i"
done 


H="$(( $LINES-10 ))";
W="$(( $COLUMNS-2 ))";

echo $W
echo $H
exit

chat_send_msg() {
  dialog --inputbox "Enter message" "$H" "$W" 2>>"1"
  if [ "$?" != 0 ]; then exit; fi;
  echo >>"$room"

}

while true; do
  dialog --tailbox "$room" "$H" "$W";
  if [ "$?" != 0 ]; then exit; fi;
  chat_send_msg "$room" "$nick";
done;
