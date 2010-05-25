<?php

if(!isset($__PHP_SHELL__) || $$__PHP_SHELL__ != true) {
  $__PHP_SHELL__ = true;

  //READLINE Function
  $__STDIN__ = fopen("php://stdin", "rb");
  function readline($prompt="") {
    echo $prompt;
    return fgets($GLOBALS["__STDIN__"]);
  }
  //READLINE End

}

///CODE/////////////////////////////////////////////////

if($argc > 1) {
  array_shift($argv);
  //print_r($argv); //Debug
  include($argv[0]);
  die();
}

echo (
"PHP CONSOLE\n"
."Coded: Harvie 2oo7\n"
."Version: 0.3\n"
."--------------------------\n"
);

//$loop = 1;
$__prompt__ = "\n> ";
while(1) {
  eval(readline($__prompt__));
}

?>
