rem Enable Manual Crash Feature (Windows 2000) Popular

rem Microsoft Windows 2000 includes a feature that enables you to have the system stop responding and generate a Memory.dmp file (crash to blue-screen) by using a button combination on the keyboard.
rem This "feature" is disabled by default. To enable this feature, open your registry and find the key below.

rem Create a new DWORD value called "CrashOnCtrlScroll" and set it to "1" to enable keyboard initiated crashes.
rem After restarting the computer, you can generate a system to stop responding by holding down the right CTRL key and pressing the SCROLL LOCK key twice.
rem Note: Pressing left CTRL key does not generate the system to stop responding.

reg add "HKLM\SYSTEM\CurrentControlSet\Services\i8042prt\Parameters" /v "CrashOnCtrlScroll" /t "reg_dword" /d "00000001"
echo Now reboot and pres CTRL+SCROLL,SCROLL
pause

