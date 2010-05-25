cls
screen 9
screen 0
color 0,15
cls
locate 2,2
input "Jaka je hmotnost telesa  (g) " ;hmotnost
cls
locate 2,2
input "Jaky je objem telesa  (cm3 - ml) " ;objem

screen 9
cls
locate 20,20
print "POCITAM!"
locate 2,2
print "TENTO PROGRAM NAPROGRMOVAL TOMAS MUDRUNKA."
delay 2
screen 0
hustota=hmotnost/objem
cls

print "hmotnost...."hmotnost " g"
print "objem......."objem    " cm3"
print "hustota.....? g/cm3"
print "컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴"
print "hustota = m:V"
print "hustota = "hmotnost" g:"objem " cm3"
print "hustota = "hustota" g/cm3"
print ""
print "Hustota daneho telesa je "hustota" g/cm3."
print ""
print ""
print ""
print ""
print ""
print ""
print ""
print ""
print "TENTO PROGRAM NAPSAL.... TOMAS MUDRUNKA"
PRINT "SPOLECNOST.............. X-TOM"
input "Mas uz tabulku opsanou " ;konec$


end