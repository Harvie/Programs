<?php

$fp = FOpen ("pocitadlo.txt", "r");
$pocet = FRead ($fp, 90);
FClose ($fp);

$pocet = ($pocet + 1);

$fp = FOpen ("pocitadlo.txt", "w");
FWrite ($fp, $pocet);
FClose ($fp);

echo ("Navstev: " . $pocet);

?>
