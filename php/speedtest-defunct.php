<h1>SpeedTest</h1>
<?php
//this is not working because of caching...

if( isset($_GET["time"]) ) {
    $time = ((microtime() - $_GET["time"])/1000000)-1;
} else {
    for($i=0;$i<8000000;$i++) { echo("a"); }
}
?>