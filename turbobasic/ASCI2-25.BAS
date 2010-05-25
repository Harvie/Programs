CLS
FOR A=0 TO 255
COLOR 0,15
SCREEN 1
LOCATE 2,2
PRINT A "   " CHR$ (A)""

locate 5,5
input "Chcete znat dalsi znak? Jestli ne napiste x a stisknete enter." ;dalsi$
           REM POKR:
           REM LOCATE 5,5
           REM PRINT CHR$ (A)
           REM FOR I=0 TO 99999
           REM NEXT I
           NEXT A
END