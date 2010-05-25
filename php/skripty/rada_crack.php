<?php
$start = 0;
$stop = 9999;
$length = 4;

$rada = "";
$notfoundlast = -3;
$error = 1;

while($error) {

for($i=$start;$i<=$stop;$i++) {
  //$i = base_convert($i, 10, 2);
  if(strpos($rada, $i) == 0) {
    $rada = "$rada"."$i";
    //echo($i);
  }
}

for($i=0;$i<(strlen($rada)-$length);$i++) {
  $sub = substr($rada, $i, $length);
  for($x=0;$x<$length;$x++) {
    $rada[($i+$x)] = "R";
    
  }
  $rada = str_replace($sub, "", $rada);
  
  $replace="";
  for($x=0;$x<$length;$x++) {
    $replace = $replace."R";
  }
  
  $rada = str_replace($replace, $sub, $rada);
}

//echo($rada);

//Test
//echo("\n\n");
$error = 0;
$notfound = 0;
for($i=$start;$i<=$stop;$i++) {
 if(!eregi($i, $rada)) {
   //echo("Not found: $i\n");
   $rada = $rada.$i;
   $error = 1;
   $notfound++;
 }
}
echo("Not found total: $notfound\n");
if($notfoundlast == $notfound) { $error = 0; }
$notfoundlast = $notfound;
}

for($i=$start;$i<=$stop;$i++) {
 if(!eregi($i, $rada)) {
   $rada = $rada.$i;
 }
}

echo("\n\n".$rada);
//system("pause");
