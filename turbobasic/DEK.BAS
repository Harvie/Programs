SCREEN 0
COLOR 0,15
GOTO pet

pet:
CLS
LOCATE 10,5
input "NORMA SPOTREBY = 8.2 => Enter; NEBO ZAPIS JINOU";S1
IF S1=0 THEN S1= 8.2
CLS
LOCATE 10,5
INPUT "POCET UJETYCH km OD ZACATKU MESICE";K
CLS
LOCATE 10,5
INPUT "STAV BENZINU - BLOKY NA ZAC. DEKADY";B
CLS
LOCATE 10,5
INPUT "STAV BENZINU -  NADRZ NA ZAC. DEKADY";B4
CLS
LOCATE 10,5
INPUT "CERPANE BLOKY  OD ZAC. MESICE - SL.9";B1
CLS
LOCATE 10,5
INPUT "STAV TACHOMETRU NA ZAC. DEKADY";O
CLS
PRINT SPC(32) "DEKADA"
PRINT "==============================================================================="
PRINT "       SLOUPEC 6   !   SLOUPEC 7   !      SLOUPEC 9      !      BENZIN     "
PRINT "   TACH.KONEC DNE  ! KmDen   KmCelk! ZAC  DOPL  CELK     !  BLOKY   NADRZ   "
PRINT "================================================================================"
R=8:K2=0:B6=0
FOR I =1 TO 9: R=R+1
LOCATE 20,1
PRINT
LOCATE 20,1
INPUT "UDEJ POCET UJETYCH km ZA DEN";K1
IF K1=0 THEN konec
K=K1+K:B5=B1+B5:A=(K1/100)*S1:A1=A-INT(A)
IF A1< = .7 THEN raz
IF A1> .7 THEN dva

raz:
B3=INT(A)
GOTO tri

dva:
B3=INT(A)+1
GOTO tri

tri:
LOCATE 20,1:PRINT"                                                            "
LOCATE 20,1
PRINT"SPOTREBA/NADRZ:";B3;"/";B4;"=>DOPLNENI NADRZE?":INPUT B2
LOCATE 20,1:PRINT "                                                           "
LOCATE 21,1:PRINT "                                                           "
B4=B4-B3+B2:B=B-B2:B5=B1+B2
O2=O+K1
LOCATE R-2,9:PRINT O2
LOCATE R-2,22:PRINT K1
LOCATE R-2,30:PRINT K
LOCATE R-2,39:PRINT B1
LOCATE R-2,44:PRINT B2
LOCATE R-2,49:PRINT B5
LOCATE R-2,70:PRINT B4
LOCATE R-2,62:PRINT B
LOCATE R-2,20:PRINT "!"
LOCATE R-2,36:PRINT "!"
LOCATE R-2,58:PRINT "!"
O=O2
B1=B1+B2:K2=K2+K1:B6=B6+B3
NEXT
GOTO konec

konec:
S=S1*(K2/100)
LOCATE 20,1:PRINT " NORMOVANA SPOTREBA:";S;"                "

LOCATE 20,30:PRINT"                    "

LOCATE 21,2:PRINT"SKUTECNA SPOTREBA:";B6;"                                      "
LOCATE 22,2:PRINT"UJETE km V DEKADE:";K2;"                                      "
LOCATE 24,15
INPUT "KONEC = Enter ; OPAKOVANI = 1  ";A
IF A=1 GOTO pet
   END