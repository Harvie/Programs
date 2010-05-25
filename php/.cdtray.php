<a href="?q=eject">Eject</a>
<a href="?q=close">Close</a>
<?php

if(isset($_GET["q"])) { 
    $q = $_GET["q"];
    if($q == "eject") system("eject");
    if($q == "close") {
      system("umount /cdrom");
      system("eject -t");
    }
}

?>
