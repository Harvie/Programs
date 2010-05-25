<?php

//READLINE
function readline($prompt="") {
  echo $prompt;
  
  $t = "";
  $c = "";
  
  while(1) {
    $c = fread(STDIN, "1");
    if ($c == "\n") {    // && ($c == "\r") //May be later on oher systems
      return $t;
    } else {
      $t = ($t.$c);
    }
  }

   return $t;

}
//READLINE END

$find = readline("Co hledat: ");

while(1){
  $pos = "";
  $line = (" ".readline());
  $pos = strpos($line, $find);
  //echo($pos); //debug
  if ($pos == "")
    {} else { echo (trim($line)."\n"); }
}

?>
