<?php
//Patcher generator 0.3
//Harvie 2oo7
/*
This will parse output from my HexCmp tool and
create the code, than can be directly pasted to
my BinPatcher source.
Making of patcher was never easier.
*/

///SETINGS///////////////////////////////////////////////////////
$proc = "hexcmp orig.exe crac.exe"; //HexCmp command
$fp = "bin"; //FILE pointer to handle with

///CODE//////////////////////////////////////////////////////////
$pp = popen($proc, "r");
while(!feof($pp)) {
  $line = trim(fgets($pp));
  
  if(ereg("Difference", $line)) {
    $offset = explode("H: ", $line);
    $offset = $offset[1];
    echo("fseek($fp, $offset"."L, SEEK_SET); //Seek to $offset\n");
  }
  
  if(ereg("\\\\x.", $line)) {
    $bytes = substr_count($line, "\\x");
    echo("  fwrite(\"$line\", $bytes, 1, $fp); //Patch $bytes bytes\n");
  }

}

///EXAMPLE///////////////////////////////////////////////////////
/*
  //Example output from HexCmp:
Difference @ D: 222313 H: 0x36469
\x90\x90\x90\x90\x90
Lenght: 5 Bytes

Difference @ D: 317430 H: 0x4d7f6
\x13\x37\xc0\xde
Lenght: 4 Bytes

  //Will be turned to something like this:
fseek(bin, 0x36469L, SEEK_SET); //Seek to 0x36469
  fwrite("\x90\x90\x90\x90\x90", 5, 1, bin); //Patch 5 bytes
fseek(bin, 0x4d7f6L, SEEK_SET); //Seek to 0x4d7f6
  fwrite("\x13\x37\xc0\xde", 4, 1, bin); //Patch 4 bytes

//Just compile ;))
*/
?>
