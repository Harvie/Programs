#!/bin/bash

function shell_commands() {
	case "$1" in
                "exit") exit ;;
                "logout") exit ;;
                "clear") clear ;;

                "help") echo Try \"man hhgame\"... ;;
                "man") echo Which man page\? ;;
                "man hhgame") more man/hhgame.txt ;;
                "man game") more man/game.txt ;;

                "lsgames") ls games ;;
                
                "winners") more var/log/winners ;;

                *) echo hhgame: $1: command not found
        esac
}

function game_commands() {
        case "$1" in
                "exit") exit ;;
                "logout") echo bash: logout: not login shell: use \`exit\' && continue ;;
		"abort") echo Such early\? && exit ;;
        esac
}

