<?php

$fp = FOpen ("pocitadlo.txt", "r");
$pocet = FRead ($fp, 90); //90 znaku dlouhe cislo by melo byt dost
FClose ($fp);

$pocet = ($pocet + 1);

if (isset($HTTP_COOKIE_VARS['pricteno'])):
    echo ("pricteno = " . $HTTP_COOKIE_VARS['pricteno'] . " - Ji� P�i�teno - Dal�� p�i�ten� mo�no za hodinu od posledn�.<br />");
  else:
    $fp = FOpen ("pocitadlo.txt", "w");
    FWrite ($fp, $pocet);
    FClose ($fp);
    setcookie('pricteno', 'true', time()+3600); //sekundy - 3600 = 1hodina, 1000 000 = 11dni
  endif;

echo ("Navstev: " . $pocet);

?>
