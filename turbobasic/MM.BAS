cls
open "c:\util\mm.dta" For random  AS #1
c$=input$(137,#1)
b$=mid$(C$,129,7)
input "Zadej vstupni heslo"; a$
   if a$=b$ then end
 shell "rem date>>c:\util\mm.TXT"
 shell "rem time>>c:\util\mm.TXT"
 shell "blokne PC"
end
