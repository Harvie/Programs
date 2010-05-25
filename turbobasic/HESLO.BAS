cls
open "heslo.dta" For random  AS #1
c$=input$(137,#1)
b$=mid$(C$,129,7)
input "Zadej vstupni heslo"; a$
   if a$=b$ then end
 shell "date>>HESLO.TXT"
 shell "K>>HESLO.TXT"
 shell "KONEC"
end
