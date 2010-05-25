 GOTO POKR

POKR :
SCREEN 8 : CLS
COLOR 14,1
RESTORE BARVY
BARVY :

    DATA 8,7,15,15,7,8,9,6,9,6,2,0,1,3,5,7,9,9,7,5,3,1,0
FOR A=1 TO 23
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
   A$="TELEFON  DIR    exit   HELP   LOAD "
   D=5:X=5:Y=17:C=14
   GOSUB MENU
   ON MENU GOSUB TELEFON,DIR,KONEC,HELP,LOAD
LOOP

'-----------------------------------------------------------------------------
TELEFON :
print "ahoj"
RETURN
'-----------------------------------------------------------------------------
DIR :
CALL ANYKEY
CALL CLWD
RETURN
RETURN
'-----------------------------------------------------------------------------
KONEC :
print "stla‡ kl vesu Enter"
end
'-----------------------------------------------------------------------------
HELP :
A$="Jste uzivatelem pocitacove site C&P NETWORK. Jste vybaven harddiskem se"
CALL ECHO (5,16,A$,13)
A$="softwarovym vybavenim. Mate k dispozici modem, takze muzete navazat"
CALL ECHO (7,17,A$,13)
A$="spojeni s jinym uzivatelem C&P NETWORK."
CALL ECHO (7,18,A$,13)


CALL ANYKEY
CALL CLWD
RETURN
'-----------------------------------------------------------------------------
LOAD :
FOR R=0 TO 4000
NEXT R
CLS
COLOR 14,1
FOR A=0 TO 2000
Q=RND*55
T=RND*55
CIRCLE (Q,T),50
FOR L=0 TO 5000
NEXT L
NEXT A
LOCATE 8,23
PRINT " KONEC "
STOP
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