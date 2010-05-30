fm (Fast Move: minimalistic ncurses file system viewer)
=======================================================
Small, simple and efficient file system viewer for the console. It offers fast navigation, easy invocation of your common tools (`Emacs`/`Vim`/`Xpdf`/`less`/...) and instant access to your favorite shell. It also can use Linux inotify to update the file tree.

### How to compile
It should run in any modern UNIX-like with minimal effort. You just need to have installed `ncursesw`, `glib` and its development headers. Then just run:

		make

### Hacking
Internally the file system hierarchy is represented in a N-ary tree in which each node represents a file and its associated information. Additionally a double-linked list is used to represent the lines from screen. The `glib` is used for its N-ary trees and double-linked lists. Also some string manipulation functions are used for commodity. The rest is standard POSIX code.

### Screenshot
![fm screenshot](/imgs/fm-screenshot.png)

### Invocation
		Usage: fm [-h|-d] [<tree_root>]
		The root of the displayed tree is `tree_root'.
			-h: show this help
			-d: print on stderr inotify events debug info

### Keyboard commands
Most commands are based in text navigation of `Vim`.

Moving the cursor:

		j:			select next file.
		k:			select previous file.
		l:			open selected directory (open the directory twice to show dot files).
		h:			close current/selected directory.
		Ctrl + f:		move one page forward the cursor.
		Ctrl + b:		move one page backward the cursor.
		Ctrl + d:		move half page forward the cursor.
		Ctrl + u:		move half page backward the cursor.
		gg:			select first file.
		G:			select last file.
		zz:			center selected file on screen.
		H:			select first line from screen.
		L:			select last line from screen.
		}:			selects next file without going deeper in the hierarchy.
		{:			selects previous file without going deeper in the hierarchy.
		[[:			selects parent directory.
		f + <letter>:		selects next file that begins with <letter>.
		F + <letter>:		selects previous file that begins with <letter>.

Running external programs:

		l:			opens the selected file with text pager/PDF viewer/PS viewer/image viewer (based on the file name extension).
		s:			spawns the shell in the current directory (Use Ctrl + d to return).
		Enter:			starts the editor with selected file.

Miscellanea:

		q:			quits fm.
		Ctrl + l:		redraws the entire screen and reloads the content of the current working directory.
		Ctrl + g:		prints info from the selected like `ls -l`.
		Esc:			clear info window. Set command repetitions to one.

The default choices for the external programs are `vim`/`xpdf`/`gv`/`display`. They are configured from `fm.h`. Also the can be customized setting the environment variables `PAGER` and `EDITOR` from the shell.  The commands `h j k l` can be repeated several times with just one keystroke if a number is given first, just like `Vim` commands repetitions. The cursor can also be moved with the arrows from the keyboard.

### Project status
Version 1.1 finished, Linux inotify support added.

### License
The program is under the `GPLv3`.

### Feedback
Send me an email if you have any question about the code, you found a bug or you have a feature request. Send it to:

		jimenezrick@gmail.com
