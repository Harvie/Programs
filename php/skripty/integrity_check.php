<?php

function file_chceck($file, $cycles = 4) {
  $md5 = md5_file($file);
  for(;$cycles>0;$cycles--) {
    $last_md5 = $md5;
    if( ($md5 = md5_file($file)) != $last_md5 ) { return 0; }
  }
  return 1;
}
