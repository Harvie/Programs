@echo off
set outfile=.\seznam_souboru.txt
@%SystemRoot%\system32\chcp 1250 >nul
echo Probiha ziskavani informaci...
echo Vypis souboru od uzivatele %USERNAME% > %outfile%
%SystemRoot%\system32\tree /F /A >> %outfile%
%SystemRoot%\system32\notepad %outfile%
echo HOTOVO!!! Vypis najdes v souboru %outfile%
echo Stiskni enter pro ukonceni...
pause >nul

