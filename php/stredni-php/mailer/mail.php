<?php

if ( Mail($_POST["mail_prijemce"], $_POST["predmet"], $_POST["zprava"], "From: " . $_POST["mail_odesilatele"]) )
echo "Mail byl odeslan!";
else echo "Mail se nepodarilo odeslat!";
?>
