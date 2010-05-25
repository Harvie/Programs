program morse;
uses crt;
const
tab: array ['A'..'Z'] of string[6] = (
     '._','_...','_._.','_..','.','.._.','__.','....','____','..',
     '.___','_._','._..','__','_.','___','.__.','._.','...','_','.._',
     '..._','_.__','__..');
var
s:string;
i:byte;

begin
writeln ('***PROGRAM  *M*O*R*S*E*  ----  PREVEDE TEXT DO MORSEOVY ABECEDY*** ');
write   ('ZADEJ TEXT PRO PRýEVOD: '); READLN (s);

FOR i:= 1 TO LENGTH (s) DO
CASE s[i] OF
     'A'..'Z','a'..'z': WRITE (Tab[Upcase (s[i])],'|');
     ' '                WRITE ('|');
   END;
end.