 GOTO POKR



POKR :
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

REM zadani polozek adresare
DO
   A$="NASOB HELP  POZDR NASO2 KONEC "
   D=5:X=5:Y=17:C=14
   GOSUB MENU
   ON MENU GOSUB NASOB,TEXT,NIC1,NASOB2,KONEC
LOOP

'-----------------------------------------------------------------------------
TEXT :
LOCATE 10,5
print "JESTLI ¦E NEUMÖTE NASOBIT:*KONEC*,!!!POZOR!!! NASOB2 ZNAMKUJE"
RETURN
'-----------------------------------------------------------------------------
NASOB :
SCREEN 0
CLS

COLOR 14,1
N=0
S=0
V=0

OPET:
N=N+1
CLS
LOCATE 4,23
PRINT "PROGRAM PRO ZKOUSENI MALE NASOBILKY"
A=INT(RND(5)*10+1)
B=INT(RND(4)*10+1)
LOCATE 10,5
PRINT "NAPIS VYSLEDEK" A "x" B "="
INPUT "    STISKNI ENTER";C
D=A*B
IF D=C THEN ANO
SOUND RND*10000+90,90
PRINT "    SPATNY VYSLEDEK"

S=S+1
GOTO VOLBA

ANO:
SOUND RND*100+90,90
PRINT "    SPRAVNE"
V=V+1
VOLBA:
LOCATE 18,10

PRINT "Z" N "POKUSU MAS" S "SPATNYCH A" V "DOBRYCH"
LOCATE 20,10
PRINT "POKUD CHCES POKRACOVAT ZMACKNI ENTER"
LOCATE 21,10
INPUT "POKUD CHCES SKONCIT NAPIS 1 A ZMACKNI 2 x ENTER";E
IF E<>1 THEN OPET
REM  pise chybu     sound rnd*100+12,13
RETURN
'-----------------------------------------------------------------------------
KONEC :
print "stlac klavesu Enter"
end
'-----------------------------------------------------------------------------
NIC1 :
A$="Zdravi Vas Tomas Mudrunka"
CALL ECHO (5,16,A$,13)
A$="Jak se mate?"
CALL ECHO (7,17,A$,13)
A$="Jak se Vam libi tento program?"
CALL ECHO (7,18,A$,13)

CALL ANYKEY
CALL CLWD
RETURN
'-----------------------------------------------------------------------------
NASOB2 :

SCREEN 0
CLS
COLOR 4,15
S=0
V=0


FOR I=1 TO 10
 rem   nechat pokud nebude vadit         CLS
  LOCATE 4,20
	PRINT "PROGRAM PRO ZKOUSENI MALE NASOBILKY - 10 PRIKLADU"

	A=INT(RND(5)*10+1)
	B=INT(RND(4)*10+1)
	LOCATE 10,5
          REM vymaze radek
          PRINT "                                     "
          PRINT "                                     "
          LOCATE 10,5
	PRINT "NAPIS VYSLEDEK" A "x" B "="
        INPUT "    STISKNI ENTER";C
	D=A*B

	IF D=C THEN ANO2
        SOUND RND*10000+55,55
	PRINT "    SPATNY VYSLEDEK"
	
	S=S+1
	GOTO VOLBA2
	
	ANO2:
        SOUND RND*100+55,55
	PRINT "    SPRAVNE"
	V=V+1
	VOLBA2:
   LOCATE 18,10
   PRINT "Z" I "POKUSU MAS" S "SPATNYCH A" V "DOBRYCH"
         REM zdrzeni programu
         FOR G=1 TO 10

         NEXT G
NEXT I

LOCATE 18,10
PRINT "Z" S+V "POKUSU MAS" S "SPATNYCH A" V "DOBRYCH"

IF S=>4 THEN T=5
IF S=3 THEN T=4
IF S=2 THEN T=3
IF S=1 THEN T=2
IF S=0 THEN T=1
LOCATE 20,10
PRINT "VYSLEDNA ZNAMKA" T

Input "Ukonci stiskem klavesy ENTER";A
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