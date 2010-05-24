Xwarppointer allows you to do manipulation of the pointer on the screen.

The first mode of operation switches from screen to screen which is described
below. The rest is self explanatory, just run xwarppointer with no arguments
and you will get usage.

If you have a multihead display, with multiple display/screens instead of one
giant virtual screen, this mode is useful. To use it, you should bind it to
some key sequence in your window manager. The program then allows you to warp
the pointer to a different screen, so you can move the mouse easily between
montiors with just a single keystroke (keeping the same X and Y positions).

The usage is:

   xwarppointer screen <#>	This will warp the pointer to the screen you
				specify.

   xwarppointer screen -        This will warp the pointer to the screen before
				the current one. If on the first screen, it
				will wrap to the last.

   xwarppointer screen +        This will warp the pointer to the screen after
				the current one.  If on the last screen, it
				will wrap to the first.




Example usage:

Using FVWM, I have the following line in my FVWM config file:

Key F11		A       N      Exec xwarppointer screen +

This will cause F11 with no modifiers in any part of the screen to run
"xwarppointer screen +", thus causing the pointer to go to the next montior.
Once the cursor hits the last montior, when I hit F11 again, it goes back to
the first.


                                             _________________
                                             nirva@ishiboo.com
