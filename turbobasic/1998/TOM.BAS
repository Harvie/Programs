START:
    CLS
    COLOR 14,1
    SCREEN 1
                       LOCATE 2,2
                       INPUT "  ZADEJTE STARTOVACÍ PIN  " ;A
                       IF A<>007 THEN GOTO START
                REM ZDE LZE DOPLNIT PROGRAM DO KTEREHO LZE VSTOUPIT PINEM 007
   STOP
   END