<?php

$fp = FOpen ("gbook.txt", "w");
FWrite ($fp, "");
FClose ($fp);
echo "Soubor gbook.txt byl vyprazdnen.";

?>
