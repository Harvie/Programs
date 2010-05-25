<?php

//FUNCTION IS_PRIME()

function is_prime ($num) {
  
  if($num < 2) { return (0); } 
  if ( ($num > 2) && (($num % 2) == 0) ) { return (0); }
  $i = 2;
  while ($i < $num) {
    if ( ($num % $i) == 0 ) { return (0); }
    $i++;
  }
  return (1);
}

//CODE

//Prvocisla: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, …
//echo(is_prime(1)."\n".(7%2)); //Debug

//Settings
$min = 1;
$max = 3000;

//Logic
$total_primes = 0;
while ($min <= $max) {
  if(is_prime($min)) {
  echo($min."\n"); //Show all primes
  $total_primes++; //Count all primes
  }
$min++;
}

//Output
echo("\nPrimes total: ".$total_primes."\n"); //Total number of primes

?>
