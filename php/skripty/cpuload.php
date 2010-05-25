<?php
//CPULoad

function getmicrotime(){
   list($usec, $sec) = explode(" ",microtime());
   return ((float)$usec);
   } 
   
   
$c = 0;
$ind = 0;
while($c < 0.3) {    // max 0.3
  $a = getmicrotime();

////////////////////////////////////////////////////////////////
    /*
    for ($i=0; $i < 1000; $i++){
      $x = cos(sin(cos(sin(cos(sin(60))))));
    }
    */
    
    $desc = array(
   0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
   1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
   2 => array("pipe", "r") // stderr is a file to write to
);

$proc[$ind] = proc_open("proctest.exe", $desc, $pipes);
$ind = ($ind + 1);

////////////////////////////////////////////////////////////////

  $b = getmicrotime();
  $c = round(($b - $a), 10);
  echo($c."\n");
  
  }

echo("end");
system("taskkill /f /im proctest.exe");
echo("OK");
?>
