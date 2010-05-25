<?php

/* Example for password "czISc.fWfjsQQ" ("admin"):
 * $salt = ("cz"); //First two characters of password
 * $hash = ("ISc.fWfjsQQ"); //Rest of password
 */


$salt = ("cz"); //First two characters of password
$hash = ("ISc.fWfjsQQ"); //Rest of password
$pass = ("admin"); //Debug only (Contains known password)

//////////////////////////////////////////////////////////////////////////////

//READLINE Function
function readline($prompt="") {
  echo $prompt;
  
  $t = "";
  $c = "";
  
  while(1) {
    $c = fread(STDIN, "1");
    if ($c == "\n") {    // && ($c == "\r") //May be later on oher systems
      return trim($t);
    } else {
      $t = ($t.$c);
    }
  }

   return $t;

}
//READLINE End

//CODE
$loop = 1;
while($loop) {

$try = readline();

//$try = $pass; //Debug only
$crypt = crypt($try, $salt);
  if ( $crypt == ($salt.$hash) ) {
    echo ("\nSalt: $salt\nHash: $hash\nPass: $try\n\n");
  }

}

?>
