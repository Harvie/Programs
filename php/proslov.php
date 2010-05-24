#!/usr/bin/php
<?php
/* Nejnudnejsi prednaska na svete
 * Harvie 2oo7
 */
$celkem = 40;
$odstavec = 5;
$start = "Vážené dámy, vážení pánové,";

  $part1=Array(
   'Na druhé straně',
   'Stejně tak',
   'Nesmíme však zapomínat, že',
   'Tímto způsobem',
   'Závažnost těchto problémů je natolik zřejmá, že',
   'Každodenní praxe nám potvrzuje, že',
   'Pestré a bohaté zkušenosti jasně říkají, že',
   'Poslání organizace, zejména pak',
   'Ideové úvahy nejvyššího řádu a rovněž',
   'Naše dlouhodobé ambice, stejně jako',
   'Vzájemné postavení organizačních autorit zastávajících'
  );
  $part2=Array(
   'realizace plánovaných vytyčených úkolů',
   'rámec stavu a vývoje postavení',
   'stabilní a kvantitativní vzrůst a sféra naší aktivity',
   'navržená struktura organizace',
   'nový model organizační činnosti',
   'stálé, informačně-propagandistické zabezpečení naší práce',
   'další rozvoj různých forem činnosti',
   'upřesnění a rozvoj struktur',
   'komplexní analýza našich možností',
   'konzultace se širokým aktivem',
   'počátek každodenní práce na poli formování pozice'
  );
  $part3=Array(
   'hraje závažnou roli při utváření',
   'od nás vyžaduje analýzy',
   'vyžadují nalezení a jednoznačné upřesnění',
   'napomáhá přípravě a realizaci',
   'zvyšuje potřebu aplikace',
   'jednoznačně předurčují implementaci',
   'zabezpečuje širokému okruhu specialistů účast při tvorbě',
   've značné míře podmiňuje vytvoření',
   'umožňuje splnit významné úkoly na rozpracování',
   'umožňuje zhodnotit význam',
   'představuje pozoruhodný experiment prověrky',
   'vyvolává proces zavádění a modernizace',
   'přetváří strukturu vedení',
   'posiluje platnost',
   'vyžaduje návrh a realizaci',
   'vyzaduje rozšiřování logistických prostředků a',
   'pomáhá udržovat kumulativní progresi',
   'dostatečně oddaluje propad'
  );
  $part4=Array(
   'existujících finančních a administrativních podmínek.',
   'dalších směru rozvoje.',
   'systému masové účasti.',
   'možnosti nasazení veškerých dostupných prostředků.',
   'pozic jednotlivých účastníků k zadaným úkolům.',
   'nových návrhů.',
   'systému výchovy pracovníků odpovídajících aktuálním potřebám.',
   'směru progresivního rozvoje.',
   'odpovídajících podmínek aktivizace.',
   'modelu rozvoje.',
   'forem působení.',
   'tvorby nových zdrojů.'
   );


echo('Pocet vet v DB: '.sizeof($part1)*sizeof($part2)*sizeof($part3)*sizeof($part4)."\n\n");

echo("$start\n");
echo($part2[rand(0,sizeof($part2)-1)]." ");
echo($part3[rand(0,sizeof($part3)-1)]." ");
echo($part4[rand(0,sizeof($part4)-1)]." ");

for(;$celkem>1;$celkem--) {
  if($celkem%$odstavec == 0) echo("\n\n");
  echo($part1[rand(0,sizeof($part1)-1)]." ");
  echo($part2[rand(0,sizeof($part2)-1)]." ");
  echo($part3[rand(0,sizeof($part3)-1)]." ");
  echo($part4[rand(0,sizeof($part4)-1)]." ");
}

echo("\n\n");
