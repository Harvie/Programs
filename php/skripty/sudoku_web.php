<form action="?" method="POST">
<textarea rows="9" cols="9" name="sudokupost">43 6 5 89
56     23
   2 3   
7 5 6 9 4
   9 1   
2 9 7 5 1
   4 8   
32     56
89 5 6 12</textarea>
<br /><input type="submit" value="Vyresit">
</form>

<?php
//Sudoku

//
function sudoku_print ($sudo) {
  $sudo = str_replace(" ", ".", implode("\n", $sudo) );
  $sudo = explode("\n", $sudo);
  print_r($sudo);
}

//X line
//Y row

//
function sudoku_pos ($sudo, $x, $y) {
  return($sudo[$x-1][$y-1]);
}
//
function sudoku_row ($sudo, $y) {
  $row = "";
  foreach ($sudo as $line) {
    $row = ($row.$line[$y-1]);
  }
  return $row;
}
//
function sudoku_line ($sudo, $x) {
  return $sudo[$x-1];
}
//
function sudoku_square ($sudo, $x, $y) {
  if ( $x == 1 || $x == 2 || $x == 3 ) { $x = 1; }
  if ( $x == 4 || $x == 5 || $x == 6 ) { $x = 4; }
  if ( $x == 7 || $x == 8 || $x == 9 ) { $x = 7; }
  if ( $y == 1 || $y == 2 || $y == 3 ) { $y = 1; }
  if ( $y == 4 || $y == 5 || $y == 6 ) { $y = 4; }
  if ( $y == 7 || $y == 8 || $y == 9 ) { $y = 7; }
  //echo("\n$x\n$y\n");
  $square = "";
  $x--;
  $y--;
  $y0 = $y;
  
  $i2 = 0;
  while( $i2 < 3 ){
    $i1 = 0;
    $y = $y0;
    while ( $i1 < 3 ){
      $square = $square.$sudo[$x][$y];
      $y++;
      $i1++;
      //echo("\n$x-$y");
    }
    $i2++;
    $x++;
  }
  //echo("\n".$square);
  return ($square);
}

//
function sudoku_candidates ($sudo, $x, $y) {
  $cands = "123456789";
  $donenums = ( sudoku_line($sudo, $x).sudoku_row($sudo, $y).sudoku_square($sudo, $x, $y) );
  //echo $donenums;
  //echo "\n\n";
  //foreach ( $donenums as $del ) {
  $indx = strlen($donenums)-1;
  while ($indx >= 0) {
    $del = $donenums[$indx];
    //echo("!".$del);
    $cands = str_replace($del, "", $cands);
    $indx--;
  }
  //echo "\n\n".$cands;
  return $cands;
}

//
function sudoku_compute($sudoku) {

  $sudoku_old = "";
  while( $sudoku_old != $sudoku ) {
    $sudoku_old = $sudoku;

    $is = sizeof($sudoku)-1;
    while ($is >= 0) {
      $line = $sudoku[$is];
  
      $i = strlen($line)-1;
      while ($i >= 0) {
        //echo($line[$i]);
    
        $cands = sudoku_candidates ( $sudoku, ($is+1), ($i+1) );
        if ( strlen($cands) == 1 && !is_numeric($sudoku[$is][$i]) ) {
          $sudoku[$is][$i] = $cands;
        }
    
        $i--;
      }
      //echo("\n");
  
      $is--;
    }
  }
  return($sudoku);
}

///CODE///////////////////////////////////////////
/*
$sudoku = array(
  "43 6 5 89",
  "56     23",
  "   2 3   ",
  "7 5 6 9 4",
  "   9 1   ",
  "2 9 7 5 1",
  "   4 8   ",
  "32     56",
  "89 5 6 12"
  );
*/

sudoku = explode ("\n",$_POST["sudokupost"]);


echo("Zadani:\n");
sudoku_print($sudoku);
echo("\nVysledek:\n");
sudoku_print(sudoku_compute($sudoku));

?>
