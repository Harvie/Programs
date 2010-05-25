S1=0
S2=0
V=0
CLS
                     FOR O=1 TO 20
COLOR 0,15
SCREEN 0
LOCATE 2,2
                                      INPUT " SČÍTANEC= " ;S1
LOCATE 23,23
                                      INPUT "      " ;K$
IF K$="X" THEN GOTO KONEC
LOCATE 3,2
 PRINT " + "
LOCATE 4,2
                                      INPUT " SČÍTANEC= " ;S2
LOCATE 5,2
 PRINT " = "
V=S1+S2
LOCATE 6,2
PRINT V
                     NEXT O
GOTO KONEC
KONEC:
END