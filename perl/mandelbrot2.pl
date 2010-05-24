#!/usr/bin/env perl
# Mandelbrot set ASCII visualisation
# http://en.wikipedia.org/wiki/Mandelbrot_set
# Copylefted by: Harvie 2oo9

#                                                                           
#                                     aa                                    
#                                     aa                                    
#                                     aa                                    
#                                     aa                                    
#                                  aaaccaaa                                 
#                                 aaai  iaaa                                
#                            aabaab        baabaa                           
#                             aab            baa                            
#                            aad              daa                           
#                             aab            baa                            
#                            aaaaad        daaaaa                           
#                         aaacccl            lcccaaa                        
#                     dbaaab                      baaabd                    
#                    aab                              baa                   
#                    aab                              baa                   
#                aaaaah                                haaaaa               
#          ba aaaak   c                                c   kaaaa ab         
#            aabc                                            cbaa           
#               aacabab                                babacaa              
#                    aae          CODING ZEN          eaa                   
#                     aaa                            aaa                    
#                      ae  n          cc          n  ea                     
#                      aaa  ab aa afa    afa aa ba  aaa                     
#

use strict;
use warnings;

#my @chars=split(//, ' abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890');
#my @chars=split(//, ' .-:*+=%@#');
my @chars=split(//, ' .`-_\':,;^=+/\"|)\\<>)iv%xclrs{*}I?!][1taeo7zjLunT#JCwfy325Fp6mqSghVd4EgXPGZbYkOA&8U$@KHDBWNMR0Q');

my $max_iteration = 140;
my $zoom = 65;

for($max_iteration=0;$max_iteration<1000;$max_iteration++) {

print "\033[0;0H";
print "ASCII Mandelbrot Set Visualisation (Harvie 2oo9) - Iterations: $max_iteration     \n";
#for(my $x=-1;$x<=1.1;$x+=0.1) { for(my $y=-2;$y<=1;$y+=0.04) {
for(my $i=-2;$i<=2;$i+=5/$zoom) { for(my $j=-2.1;$j<=2.1;$j+=2/$zoom) {

  my $x0 = $i;
  my $y0 = $j;

  my $x = 0;
  my $y = 0;

  my $iteration = 0;
 
  while ( $x*$x + $y*$y <= (2*2)  && $iteration < $max_iteration ) {
    my $xtemp = $x*$x - $y*$y + $x0;
    $y = 2*$x*$y + $y0;
    $x = $xtemp;
    $iteration++;
  }

	my $color; 
	if ( $iteration == $max_iteration ) {
    $color = 0;
  } else {
		$color = int( (($iteration/$max_iteration)*@chars) + .5);
	}
	#print "$color\n";
	print $chars[$color];


} print "\n"; }

}
