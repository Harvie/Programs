<?php

//READLINE Function
function readline($prompt="") {
  echo $prompt;
  
  /*
  $ifp = fopen("php://stdin", "r");  //PHP <= 4
  $line = fgets($ifp, 999); //PHP <= 4
  fclose($ifp); //PHP <= 4
  */
  
  $line = fgets(STDIN, 999); //PHP >= 5
  return trim($line);
}
//READLINE End

//Hobit-01.txt

//Settings:
$prompt = "\n> ";
$random_out = "Random judged: ";
$nextf_out = "\nNext file: ";
exec("chcp 1250"); //Only Windows - set CP1250 - national alphabets - needed for Cestina ;)

//CODE
//Init:
$game = ""; //Hra natazena v pameti
$file = ""; //Posledni otevreny soubor
$nextf = ""; //Soubor nacteny z odkazu
$pos = 2; //Pozice ve hre
$histpos = "1";
$hist[$histpos] = "START";


echo(
"OGB Engine 0.6\n".
"Harvie 2oo7\n".
"http://gbook.wz.cz/\n".
"-----------------------\n".
"Type h[ENTER] for help.\n"
);

while(1) {

  $in_line = readline($prompt); 
  $in_cmds = explode(" ", $in_line);
  $in_cmds[0] = strtolower($in_cmds[0]);
  
  //h - print help
  if ($in_cmds[0] == "h") {
    echo(
    "HELP:\n".
    "h - Show help\n".
    "f - Show files\n".
    "l file - Load file\n".
    "l URL - Load file from HTTP or FTP\n".
    "y - set CP 1250 ONLY WINDOWS!!! (for some national characters)\n".
    "# - Go to line #\n".
    "2 - Go to start\n".
    "p - Print history\n".
    "n - Load next file from link\n".
    "r - Reload file\n".
    "c - Close file\n".
    "q - Quit\n"
    );  
  }

  //l - load game
  if ($in_cmds[0] == "l") {
    if ( file_exists( trim($in_cmds[1]) ) || strpos(" ".$in_cmds[1], ":") )
      {
        if ( strpos(" ".$in_cmds[1], ":") ) {
          echo("DOWNLOADING...\n");
        }
        $game = file( trim($in_cmds[1]) );
        $file = trim($in_cmds[1]);
        $histpos++;
        $hist[$histpos] = "\nLOAD: ".trim($in_cmds[1])."\n";
        echo("LOADED: $in_cmds[1]\n");
        print_r(explode("|", $game[0]));
        echo("\n");
      }
      else
      {
        echo("CAN'T LOAD: $in_cmds[1]\n");
      }
  }
  
  //r - reload
  if ($in_cmds[0] == "r") {
    $histpos++;
    $hist[$histpos] = "RELOAD";
    $game = file($file);
  }
  
  //p - history
  if ($in_cmds[0] == "p") {
    echo( implode(";", $hist) );
  }
  
  //c - close
  if ($in_cmds[0] == "c") {
    $histpos++;
    $hist[$histpos] = "CLOSE\n";
    $game = ("");
  }
  
  //# - show line
  if (  is_numeric( trim($in_cmds[0]) )  ) {
    $pos = round(trim($in_cmds[0]));
    $posi = ($pos - 1);
    if(isset($game[$posi])) {

      //history
      $histpos++;
      $hist[$histpos] = $pos;

      //nextf
      if ( strpos( (" ".$game[$posi]), "%%" ) ) {
        $nextf = explode("%%", $game[$posi]);
        $nextf = explode("|", $nextf[1]);
        $nextf = $nextf[0];
      }
      
      //rnd
      $rnd = "";
      if ( strpos( (" ".$game[$posi]), "{" ) ) {
        $rnd = explode("{", $game[$posi]);
        $rnd = explode("}", $rnd[1]);
        $rnd = explode("\\\\", $rnd[0]);
        $rnd = $rnd[rand( 0, (sizeof($rnd) - 1) )];
        $rnd = ($random_out.$rnd);
      }
      
      //out
      echo("$pos: ". $game[$posi]);
      
      //rnd
      echo ($rnd);
      
      //nextf
      if( is_file($nextf) ) {
        echo($nextf_out.$nextf."\nType n[ENTER]2[ENTER] to go on.");
        }
    }
  }
  
  //f - files
  if ( $in_cmds[0] == "f" ) {
    echo("FILES:\n\n");
    $dfp=opendir('.');
    while (false!==($file = readdir($dfp))) {
      if ($file != "." && $file != "..") {
        echo "$file\n";
      }
    }
    closedir($dfp);
  }
  
  //y - CP 1250 (needed for Czech language)
  if ( $in_cmds[0] == "y" ) {
    system("chcp 1250"); //Only Windows
  }
  
  //i - info
  if ($in_cmds[0] == "i") {
    print_r(explode("|", $game[0]));
    echo("\n");
  }
  
  //n - nextfile
  if ($in_cmds[0] == "n" && is_file($nextf) ) {
    $game = file($nextf);
    $file = $nextf;
    $histpos++;
    $hist[$histpos] = "\n
    NEXTF: $nextf\n";
    $nextf = "";
  }
  
  //q - quit
  if ($in_cmds[0] == "q") {
    die("Exited by user at $file - $pos.\n");
  }

}

?>
