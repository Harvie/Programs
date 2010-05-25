<?php
//Tic-Tac-Toe
//<- Harvie 2oo7
/*
Inspired by WarGames ;D
*/

echo("Tic-Tac-Toe (<- Harvie 2oo7)\n");
echo("Inspired by WarGames ;D\n");

//Settings////////////////////////////////////////////////////////
  //Skin:
    $cross = "X";
    $circl = "O";
    $versep = "|";
    $horsep = "-";
    $ttt = " 123456789";
  //First player:
    $p = false; //false - O; true - X;
  //Artificial inteligency:
    $ai = 3; /* AI (very poor == rand(1,9) ;)
    Settings:
    0 - Circle
    1 - Cross
    2 - Disabled
    3 - PCvsPC
    */

//Functions//////////////////////////////////////////////////////
//
function ttt_print($ttt) {
  $v = $GLOBALS["versep"];
  $h = $GLOBALS["horsep"];

  echo("\n");
  echo($ttt[7].$v.$ttt[8].$v.$ttt[9]."\n");
  echo("$h$h$h$h$h\n");
  echo($ttt[4].$v.$ttt[5].$v.$ttt[6]."\n");
  echo("$h$h$h$h$h\n");
  echo($ttt[1].$v.$ttt[2].$v.$ttt[3]."\n");
  echo("\n");
}

//
function ttt_match($ttt, $place, $player) {
  $x = $GLOBALS["cross"];
  $o = $GLOBALS["circl"];
  
  if( ($place>0 && $place<10) && ($ttt[$place] != $x && $ttt[$place] != $o) &&  is_numeric($place) ) {
    $ttt[$place] = $player;
  } else { $GLOBALS["err"] = true; }
  return $ttt;
}

//
function ttt_check($ttt) {
  $x = $GLOBALS["cross"];
  $o = $GLOBALS["circl"];
  //Horizontal
  if( $ttt[7] == $ttt[8] && $ttt[8] == $ttt[9] ) { return 1; }
  if( $ttt[4] == $ttt[5] && $ttt[5] == $ttt[6] ) { return 1; }
  if( $ttt[1] == $ttt[2] && $ttt[2] == $ttt[3] ) { return 1; }
  //Vertical
  if( $ttt[7] == $ttt[4] && $ttt[4] == $ttt[1] ) { return 1; }
  if( $ttt[8] == $ttt[5] && $ttt[5] == $ttt[2] ) { return 1; }
  if( $ttt[9] == $ttt[6] && $ttt[6] == $ttt[3] ) { return 1; }
  //Diagonal
  if( $ttt[7] == $ttt[5] && $ttt[5] == $ttt[3] ) { return 1; }
  if( $ttt[1] == $ttt[5] && $ttt[5] == $ttt[9] ) { return 1; }
  //Nobody won
  for($i=1;$i<strlen($ttt);$i++) {
    if($ttt[$i] != $x && $ttt[$i] != $o) { return 0; }
  }
  return -1;
}

//Code///////////////////////////////////////////////////////////

//Init///////////////////////////////////////////////////////////
$__STDIN__ = fopen("php://stdin", "rb");
$err = false;
$time = @explode(",", trim(@shell_exec("echo %time%"))); @srand(time()*$time[1]); //print_r($time); //Debug

//Go/////////////////////////////////////////////////////////////
ttt_print($ttt);
while(1) {
  if($p) { $player = $cross; } else { $player = $circl; }
  
  echo("$player:");
  
  if( ($p == $ai || $ai == 3) && $ai != 2 ) {
    $c = rand(1,9);
    echo($c);
  } else {
    $c = "nan";
    while( !is_numeric($c) ) { $c = fgetc($__STDIN__); }
    //echo($c); //Debug    
  }
  
  $ttt = ttt_match($ttt, $c, $player); if($err) { $err = false; echo("-"); continue; }
  elseif( ($p == $ai || $ai == 3) && $ai != 2 ) { echo("\n"); }
  
  ttt_print($ttt);
  
  if(ttt_check($ttt) == -1) { echo("!!! Nobody won !!!\n"); break; }
  if(ttt_check($ttt)) { echo("!!! Player $player won !!!\n"); break; }

  $p = !$p;
}

//system("pause");
