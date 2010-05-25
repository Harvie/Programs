<?php
//Game of life 0.3 (2D Cellular automata example implementation)
//Coded: Harvie 2oo7
/*
 *  The Rules (THX2John Conway):
 *    For a space that is 'populated':
 *      Each cell with one or no neighbors dies, as if by loneliness. 
 *      Each cell with four or more neighbors dies, as if by overpopulation. 
 *      Each cell with two or three neighbors survives. 
 *    For a space that is 'empty' or 'unpopulated':
 *     Each cell with three neighbors becomes populated. 
 *
 *  http://www.bitstorm.org/gameoflife/           -Online\offline java based version
 *  http://www.bitstorm.org/gameoflife/lexicon/   -Nice lexicon of lifes, that you can test on this simulator
 *
 *  $life contains array of strings, where every character is one cell
 *  there is also function to load life from file (for more lifes see lexicon)
 *  i included two lifes life.txt - simple small life and glider.txt - the most classical of all, it become symbol of hackership
 *
 *  for better experience please resize console window to size of life, that you are running.
 *
 *  PHP+AsciiART == Rox!!!
 */

///FUNCTIONS////////////////////////////////////////////////////

//
function life_load($file) {
  $life = file($file);
  foreach ($life as $lnum => $line) {
    $life[$lnum] = trim($line);
  }
  return $life;
}

//
function life_randomize($lines, $chars) {
  $pop = $GLOBALS["populated"];
  $unp = $GLOBALS["unpopulated"];
  $life = "";
  $i = 0;
  while($lines > $i) {
    $line = "";
    $ichars = $chars;
    while($ichars > 0) {
      $line = $line.rand(0,1);
      $ichars--;
    }
    $line = str_replace("0", $unp, $line);
    $line = str_replace("1", $pop, $line);
    $life[$i] = $line;
    $i++;
  }
  return $life;
}

//
function life_print($life) {
  foreach($life as $line) {
      $line = str_replace($GLOBALS["populated"], $GLOBALS["prpop"], $line);
      $line = str_replace($GLOBALS["unpopulated"], $GLOBALS["prunp"], $line);
      echo($line."|\r\n");
  }
}

//
function life_neighs($life, $line, $char) {
  @$neighs = "";
  @$neighs = $neighs.$life[$line][$char+1];
  @$neighs = $neighs.$life[$line][$char-1];
  @$neighs = $neighs.$life[$line+1][$char+1];
  @$neighs = $neighs.$life[$line+1][$char-1];
  @$neighs = $neighs.$life[$line+1][$char];
  @$neighs = $neighs.$life[$line-1][$char+1];
  @$neighs = $neighs.$life[$line-1][$char-1];
  @$neighs = $neighs.$life[$line-1][$char];
  return $neighs;
}

//
function life_neighs_num($life, $line, $char) {
  $neighs = life_neighs($life, $line, $char);
  $neighs = ereg_replace("(\.| |0|_|-)", "", $neighs); //Unpopulated chars: . 0_-
  $neighs = strlen($neighs);
  //echo($neighs); //Debug
  return $neighs;
}

//
function life_next($life) {

  $pop = $GLOBALS["populated"];
  $unp = $GLOBALS["unpopulated"];
  $old_life = $life;
  
  foreach($life as $lnum => $line) {
    //echo($lnum); //Debug
    for($i = 0;$i < strlen($line);$i++) {
      $neigh_num =  0;
      $neigh_num = life_neighs_num($old_life, $lnum, $i);
      //echo($lnum."-".$i."=".$neigh_num."\n"); //Debug
      if($old_life[$lnum][$i] == $pop) {
          if($neigh_num == 2 || $neigh_num == 3) { $life[$lnum][$i] = $pop; } else { $life[$lnum][$i] = $unp; }
        }
      else
        {
          if($neigh_num == 3) { $life[$lnum][$i] = $pop; }
        }
    }
  }
  return $life;
  
}

//
function cls() {
  //Clear screen
  for($i = 0; $i < 20;$i++) echo("\r\n");
}

///CODE///////////////////////////////////////////////////////////////////////////////

//Settings
$life = life_load("spacefiller.cells"); //Load life
//$life = life_randomize(30, 60); //Randomize life
$populated = "#"; //File format populated
$unpopulated = "."; //File format unpopulated
$prpop = "Û"; //chr(219); //Print chars populated
$prunp = " "; //Print chars unpopulated
$sleep = 1; //sleep in seconds
$usleep = 200000; //sleep in microseconds

/*
$i = 0;
while($i < 220) { echo($i."-".chr($i)."\n"); $i++; }
*/

//Run
$backup_life = $life;
$old_life = "";
$generation = 1;
while($old_life != $life) { //While moving
  $old_life = $life;
  echo("Generation: ".$generation."\r\n");
  life_print($life);
    //cls(); //Shifting
  $life = life_next($life);
  //sleep($sleep);
  //usleep($usleep);
  $generation++;
}


?>
