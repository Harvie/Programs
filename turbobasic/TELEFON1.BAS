start:
CLS
COLOR 14,1
SCREEN 0
LOCATE 2,2
INPUT " ZADEJTE VSTUPN÷ PIN " ;A
IF A<>4545 THEN START

 GOTO POKR

POKR :
CLS
SCREEN 8 : CLS
COLOR 14,1
RESTORE BARVY
BARVY :

    DATA 8,7,15,15,7,8
FOR A=1 TO 6
   READ C
   LINE (A,A)-(640-A,A),C
   LINE (A,106-A)-(640-A,106-A),C
   LINE (A,A)-(A,106-A),C
   LINE (640-A,A)-(640-A,106-A),C
   LINE (A,110+A)-(640-A,110+A),C
   LINE (A,200-A)-(640-A,200-A),C
   LINE (A,110+A)-(A,200-A),C
   LINE (640-A,110+A)-(640-A,200-A),C
NEXT A

DO
   A$="POZDRAV1  POMOC2  TELEFON3   HELP4  ESC Q"
   D=5:X=5:Y=17:C=14
   GOSUB MENU
   ON MENU GOSUB TELEFON,DIR,LOAD,HELP,KONEC
LOOP

'-----------------------------------------------------------------------------
TELEFON :
print "HLAVNE KDYZ POZDRAVITE:DOBRY DEN TADY----- -----."
RETURN
'-----------------------------------------------------------------------------
DIR :
LOCATE 8,23
PRINT "PRAVE STE SE DOVOLALI NA TEL.¨.:158"
LOCATE 10,10
print "TADY POLICIE H.L.M.PRAHY"
RETURN
'-----------------------------------------------------------------------------
KONEC :
print "stlaü kl†vesu*ENTER*"
END
'-----------------------------------------------------------------------------
HELP :
A$="ZKOUSKA OVLADANI TELEFONU"
CALL ECHO (5,16,A$,13)
A$="STISKN∑TE TELEFON1 NEBO POMOC"
CALL ECHO (7,17,A$,13)
A$="VIDITE U¶ TELEFONUJETE PO SITI MUDRU’KANET"
CALL ECHO (7,18,A$,13)


CALL ANYKEY
CALL CLWD
RETURN
'-----------------------------------------------------------------------------
LOAD :
PRINT " DOBRY DEN DOVOLALI JSTE SE NA TEL.:56 18 243 "
LOCATE 9,23
PRINT " TADY TOMAS MUDRUNKA "


RETURN
'-----------------------------------------------------------------------------


END
'-----------------------------------------------------------------------------
'                    podprogram ECHO
' postupny tisk textu obsazeneho v retezcove promenne A$
' na souradnice X a Y
' barvou C

SUB ECHO(X,Y,A$,C)
   COLOR C
   LOCATE Y,X
   FOR A=1 TO LEN(A$)
      PRINT MID$ (A$,A,1);
      SOUND 1200,.2
      SOUND 900,.2
      SOUND 600,.2
      FOR I=1 TO 200
      NEXT I
   NEXT A
END SUB

'--------------------------------------------------------------------------

MENU :
   ' PODPROGRAM 'MENU'

   ' X a Y jsou textove souradnice menu-okna
   ' A$ obsahuje text okna
   ' D je pocet polozek (radku) v menu
   ' W=1 - pouze vypis okna , W=0 - vypis i vyber z menu
   ' C je barva okna
   ' v promenne MENU podprogram vraci cislo zvolene polozky

   L=LEN (A$)/D : MENU = 1 : M = MENU+.0001
   COLOR C,9:LOCATE Y,X
   PRINT CHR$(201);
   FOR A=1 TO L+2 : PRINT CHR$(205); : NEXT A
   PRINT CHR$ (187);
   FOR A= 1 TO LEN(A$) STEP L
      LOCATE Y+A/L+1,X
      PRINT CHR$(186);" ";MID$(A$,A,L);" ";CHR$(186);
   NEXT A
   LOCATE Y+A/L+1,X: PRINT CHR$(200);
   FOR A=1 TO L+2 : PRINT CHR$(205); : NEXT A
   PRINT CHR$(188);
   IF W=1 THEN W=0 : RETURN

  DO
   I$ = INKEY$
   IF LEN(I$)=2 THEN IF ASC(MID$(I$,2,1))=72 THEN MENU = MENU - 1
   IF LEN(I$)=2 THEN IF ASC(MID$(I$,2,1))=80 THEN MENU = MENU + 1
   IF MENU < 1     THEN MENU = D
   IF LEN(I$)=1 THEN IF ASC(I$)=13 THEN EXIT LOOP
   IF MENU > D THEN MENU = 0 : EXIT LOOP	
   IF MENU <> M THEN LOCATE Y+M,X+L+2 : PRINT " ";:LOCATE Y+M,X+1 : PRINT " ";
   IF MENU <> M THEN LOCATE Y+MENU,X+L+2:COLOR 11:PRINT CHR$(174);:LOCATE Y+MENU,X+1:PRINT CHR$(175);:COLOR C
   M = MENU
  LOOP
   I=MENU
   W=1:C=0:GOSUB MENU
   MENU = I : COLOR 15
RETURN
'-----------------------------------------------------------------------------
SUB CLWD
   'Clear window
   FOR A=120 TO 190
      LINE (8,A)-(632,A),0
   NEXT A
END SUB
'-----------------------------------------------------------------------------
SUB ANYKEY
   DO
      IF INKEY$<>"" THEN EXIT LOOP
      C=C+1:IF C>15 THEN C=1
      COLOR C : LOCATE 24,55:PRINT "STLAC LIBOVOLNOU KLAVESU";
   LOOP
END SUB
'-----------------------------------------------------------------------------

