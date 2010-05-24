@echo off

echo Enabling IPv6 over Teredo tunnel support...
echo You will need at least Windows XP with SP2 for this!
%SystemRoot%\system32\netsh interface ipv6 install
%SystemRoot%\system32\netsh interface ipv6 set teredo client

echo Now you can check your settings...
%SystemRoot%\system32\ipconfig

echo Done!
echo If failed, you will need at least Windows XP with SP2 for this!
pause

rem DISABLE:
rem %SystemRoot%\system32\netsh interface ipv6 uninstall

