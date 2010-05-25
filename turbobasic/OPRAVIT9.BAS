

CLS
SCREEN 0
COLOR 14,1

start:
INPUT ; A$
IF A$="1" THEN GOTO ANO
IF A$="2" THEN GOTO NE
IF A$="3" THEN GOTO NEVIM
IF A$="4" THEN GOTO KONEC

ANO:
LOCATE 2,2
PRINT "ANO          "
GOTO START

NE:
LOCATE 2,2
PRINT "NE           "
GOTO START

NEVIM:
LOCATE 2,2
PRINT "NEVIM        "
GOTO START

KONEC:
LOCATE 2,2
PRINT "STLAC LIBOVOLNOU KLAVESU                                                   "
END