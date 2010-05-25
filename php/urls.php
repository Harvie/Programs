<?php
$url = 'http://soom.cz/';
preg_match_all('(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\ ?\.a-z0-9\/]*)',htmlspecialchars_decode(file_get_contents($url)), $urls);
//preg_match_all("~(http://[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_\~& =\?\.a-z0-9\/]*)~i", $vstup, $vystup);
//preg_match_all('(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\  ?\.a-z0-9\/]*)',$vstup, $vystup);
print_r($urls);