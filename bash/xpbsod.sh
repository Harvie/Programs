#!/bin/sh
# YABK - Yet Another BSOD Kit
# Simple BSOD implementation by Harvie

echo -ne "\033[1;37m" #white text
echo -ne "\033[44m"   #blue bg
echo -ne "\033[0;0H"  #Start of screen
echo -ne "\033[2J"    #cls

echo
echo
echo "

A problem has been detected and Windows has been shut down to prevent damage
to your computer.

PFN_LIST_CORRUPT

If this is the first time you've seen this error screen,
restart your computer. If this screen appears again, follow
these steps:

Check to make sure any new hardware or software is properly installed.
If this is a new installation, ask your hardware or software manufacturer
for any Windows updates you might need.

If problems continue, disable or remove any newly installed hardware
or software. Disable BIOS memory options such as caching or shadowing.
If you need to use Safe Mode to remove or disable components, restart
your computer, press F8 to select Advanced Startup Options, and then
select Safe Mode.


Technical information:

*** STOP: 0x0000004e (0x00000099, 0x00000000, 0x00000000, 0x00000000)


Beginning dump of physical memory
Physical memory dump complete.
Contact your system administrator or technical support group for further
assistance.

"

echo
echo
echo
echo -ne "\033[999B"
read
