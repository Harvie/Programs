#!/usr/bin/env perl
#http://en.wikipedia.org/wiki/RSA
#http://cs.wikipedia.org/wiki/RSA
use bignum; #We'll need bignum for real messages

sub rsa { #RSA (de)crypt function (message,modulus,exponent)
	my ($msg,$m,$e) = @_;
	($msg**$e)%($m); #TODO: reimplement using chinese remainder theorem (this is too slow!!!)
}

sub isprime { #Tell if number is prime or not (suboptimal)
	my ($num) = @_;
	if($num == 0) { return 1; }
	$num = abs($num);
	if($num % 2 == 0) { return 0; }
	for(my $i = 3; $i <= sqrt($num); $i+=2) {
		if($num % $i == 0) { return 0; }
	}
	return 1;
}

sub random_prime { #Generate random prime (suboptimal and insecure!)
	my ($max) = @_;
	for(my $i = int(rand($max)); $i => 17; $i--) {
		if(isprime($i)) { return $i; }
	}
	return 17;
}

#Generate key-pair:
my $strength = 100; #How strong should be the key (insecure!)
my ($p,$q) = (random_prime($strength),random_prime($strength)); #Generate some big random primes (eg: 61,53)
my $phi=($p-1)*($q-1); #TODO: implement Euler's totient function for study purposes
my $n = $p*$q; #modulus (public)
my $e = random_prime($phi); #e=encrypt exponent (public)
my $d = Math::BigInt->bmodinv(int($e), $phi); #d=decrypt exponent (PRIVATE!) (TODO: reimplement modinv() for study purposes)

#Example of usage:
print "\t[[[ Harvie's simple RSA demo ]]]\nfor study & testing purposes only (INSECURE!!!)\n\n";
print "=== KEYS ===\nPUB: ($n,$e)\nPRV: ($n,$d)\n";

my $msg=1337;
$msg=$n-1;
	print "=== TEST ===\nMSG: $msg\n";
my $enc=rsa($msg,$n,$e); #encrypt
	print "ENC: $enc\n";
my $dec=rsa($enc,$n,$d); #decrypt
	$test=($msg==$dec);
	print "DEC: $dec\nTST: ".$msg.($test?"=":"!=").$dec."\n";
	exit !$test;
