<?php
$start = 0;
$stop = 99;
$length = strlen($stop);
$base = 10;

//Banner
//echo("Please wait, while computing...\n\n");

//init
srand(time());
$rada = "";
$notfoundlast = -3;
$error = 1;
$origstop = $stop;

while($error) {

$stop = $origstop+rand(0,10);

for($i=$start;$i<=$stop;$i++) {
  $bi = $i;
  $i = base_convert($i, 10, $base);
  if(strpos($rada, $i) == 0) {
    
    if( $i[0] == $rada[strlen($rada)-1] ) {
      $i = substr($i, 1);
      $rada = $rada.$i;
    } elseif( $rada[0] == $i[strlen($i)-1] ) { 
      $rada = substr($rada, 1);
      $rada = $i.$rada;
    } else {
      if(rand(0,1)) { 
        $rada = $i.$rada;
      } else {
        $rada = $rada.$i;
      }
    }
    
    //echo($i);
  $i = $bi;
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
  $bi = $i;
  $i = base_convert($i, 10, $base);
  if(!eregi($i, $rada)) {
    //echo("Not found: $i\n");
    
    if( $i[0] == $rada[strlen($rada)-1] ) {
      $i = substr($i, 1);
      $rada = $rada.$i;
    } elseif( $rada[0] == $i[strlen($i)-1] ) { 
      $rada = substr($rada, 1);
      $rada = $i.$rada;
    } else {
      if(rand(0,1)) { 
        $rada = $i.$rada;
      } else {
        $rada = $rada.$i;
      }
    }
    
    $error = 1;
    $notfound++;
  }
  $i = $bi;
}
//echo("Total not found: $notfound\n");
if($notfoundlast == $notfound) { $error = 0; }
$notfoundlast = $notfound;

}

for($i=$start;$i<=$stop;$i++) {
 if(!eregi($i, $rada)) {
   $rada = $rada.$i;
 }
}

echo("\n\n");
echo($rada);
//system("pause");

/*
0-99:8970684924807965464133322998867361605944383525181110958785777637155047403128219391753027262000142345666906
    :9084787636241342213919946403730181714887749279538935855325108368660504433128211675657061524298202326454759697280
    :9594403292141101847565345422718137390898872605749946682585030241916233631528355170969780912038434861646767793100
*/
