#!/bin/sh
pulses_per_liter='4380'
beer_liters='0.5'
beer_price='23'
currency='Kč'

title='Beer-O-Meter'
accounts='./accounts'
totals="$accounts/.totals"
backend='./audio.sh'
tmp="/tmp/beertmp-$$";
dialog=$(which dialog);

rst() { echo -ne "\033[0;0H"; }

calc() {
	precise="$(echo 'scale=10; '"$@" | bc)"
	echo 'scale=2; '"$precise/1" | bc
}

beer_calc() {
	pulses="$1"
	litres=$(calc "$pulses/$pulses_per_liter")
	beers=$(calc "($pulses/$pulses_per_liter)/$beer_liters")
	price=$(calc "(($pulses/$pulses_per_liter)/$beer_liters)*$beer_price")
	echo $beers beers '('$price $currency, $litres l, $pulses pulses')'
}

beer_pulses() {
	echo $(wc -c "$accounts/$1" | cut -d ' ' -f 1)
}

beer_stat() {
	beer_calc "$(beer_pulses "$1")"
}

add_account() {
	"$dialog" --inputbox "New account name" 0 0 2>"$tmp"
	[ $? = 0 ] && touch "$accounts/$(cat "$tmp")";
}

servis_menu() {
	$dialog --menu "$title servis" 0 0 0 new "Novy stamgast" exit "Konec party" 2>"$tmp"
	option="$(cat "$tmp")"

	case $option in
		new)
			add_account
			;;
		exit)
			clear
			echo "=== Totals ($totals) ==="
			echo
			cat "$totals"
			echo
			exit
			;;
	esac
}

beer_menu() {
	echo -n > "$totals"
	ls -1 --group-directories-first "$accounts" | while read i; do
		echo -n "$i"; echo -ne "\x00";
		echo -n $(beer_stat "$i");  echo -ne "\x00"
		echo -e "$i\t$(beer_stat "$i")" >> "$totals"
	done | xargs -0 $dialog --menu "$title stamgasti" 0 0 0
}

tap() {
	stamgast="$1"

	pulses_start=$(beer_pulses "$stamgast")
	clear
	while true; do
		pulses_current=$(beer_pulses "$stamgast")
		pulses_diff=$[ $pulses_current - $pulses_start ]

		rst
		echo == Cepuje stamgast $stamgast, ukonci ctrl+c ==
		echo "CURRE: $(beer_calc $pulses_diff)        ";
		echo "TOTAL: $(beer_calc $pulses_current)     ";
		sleep 0.5;
	done &
	"$backend" | tee -a "$accounts/$stamgast" >/dev/null
	kill $!
}

main_menu() {
	while true; do
		beer_menu 2>"$tmp"
		[ "$?" = "0" ] && {
			tap "$(cat "$tmp")"
			true
		} || servis_menu
	done
}

main_menu
