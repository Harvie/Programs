<?php

$desc = array(
   0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
   1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
   2 => array("pipe", "r") // stderr is a file to write to
);

$proc = proc_open("proctest.exe", $desc, $pipes);
sleep(10);
proc_terminate($proc);
proc_close($proc);
proc_terminate($proc);

?>
