<?php
//Settings
$start = 0;
$stop = 1000;
$places = strlen($stop);
$tryes = 50; //Tryes to find better
$base = 10;
//Init
error_reporting(0);
srand(time());

/////////////////////////////////////////////////////////////////
//
function rada_add($name, $istr) { //Funkce pro pridani do rady -> !!! OPTIMALIZOVAT !!! <-
  if(!eregi($istr, $GLOBALS[$name])) {
    if( $istr[0] == $GLOBALS[$name][strlen($GLOBALS[$name])-1] ) {
        $istr = substr($istr, 1);
        $GLOBALS[$name] = $GLOBALS[$name].$istr;
      } elseif( $GLOBALS[$name][0] == $istr[strlen($istr)-1] ) { 
        $GLOBALS[$name] = substr($GLOBALS[$name], 1);
        $GLOBALS[$name] = $istr.$GLOBALS[$name];
      } else {

        //Nahodne na zacetek nebo konec
        if(rand(0,1)) { 
          $GLOBALS[$name] = $istr.$GLOBALS[$name];
        } else {
          $GLOBALS[$name] = $GLOBALS[$name].$istr;
        }
      
      }
  }
    //$GLOBALS[$name]=$GLOBALS[$name].$istr; //Debug
}

//
function rada_unique($name) { //Funkce pro vyhledani a smazani duplikatu (i potrebnych)
  $length = $GLOBALS["places"];
  $replacer = "%";

  for($i=0;$i<(strlen($GLOBALS[$name])-$length);$i++) {
  
    $sub = substr($GLOBALS[$name], $i, $length);
    for($x=0;$x<$length;$x++) {
      $GLOBALS[$name][($i+$x)] = $replacer;
    }
    
    $GLOBALS[$name] = str_replace($sub, "", $GLOBALS[$name]); //Smaze duplikaty
  
    $replace="";
    for($x=0;$x<$length;$x++) {
      $replace = $replace.$replacer;
    }
  
    $GLOBALS[$name] = str_replace($replace, $sub, $GLOBALS[$name]);
  }

}

//
function add_zeroes($num, $places) { //Zarovna cislo na $places mist pridanim nul na zacatek
  $str = "";
  for($i=0;$i<$places;$i++) {
    $str = $str."0";
  }
  //$num = base_convert($num, 10, $base); //BASE CONVERT -> !!! NOT WORKS YET !!! <-
  $str=$str.$num;
  $str = substr($str, strlen($str)-$places);
  return $str;
}

///CODE///////////////////////////////////////////
//Test
//die(add_zeroes(23, 5));
//////////////////////////////////////////////////
$try = $tryes;
$next = true;
while($next) { //MAIN LOOP
  $next = false;
  
  $rada = ""; //Vytvorime prazdnou radu
  for($i=$start;$i<=$stop;$i++) { //Pridame vsechna n-cisli
    $istr = add_zeroes($i, $places);
    rada_add("rada", $istr);
  }

  rada_unique("rada");
  
  $notfound = 0;
  for($i=$start;$i<=$stop;$i++) {
    $istr = add_zeroes($i, $places);
    if(!eregi($istr, $rada)) {
      $notfound++;
      //echo("Not found: $i\n");
      $next = true;
    }
  }
  
  echo("Total not found: $notfound\n"); //Debug
  
  if($try > 0) {
    if($notfound <= $minnotfound || !isset($minnotfound)) {
      echo("-minnf: $notfound\n"); //Debug
      $minnotfound = $notfound;
      $try = $tryes;
    } else {
      $try--;
    }
  } else {
    if($notfound <= $minnotfound || $notfound == 0) $next = false;
  }
  
} //MAIN LOOP

for($i=$start;$i<=$stop;$i++) { //Pridame vsechna zbyla n-cisli
    $istr = add_zeroes($i, $places);
    rada_add("rada", $istr);
  }

/////////OUTPUT///////////////////////////////////
echo("\n");
echo("Length: ".strlen($rada)." chars\n");
echo("Total not found: $notfound\n");
echo($rada);
echo("\n\n");
