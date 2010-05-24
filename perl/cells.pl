#!/usr/bin/env perl
# encode *.cells file to ShapeCollection.java format
# usage cat spacefiller.cells | perl cells.pl

print 'NAMEHERE = new Shape("NameHere", new int[][] {'."\n";
my ($l,$c);
my ($x,$y) = (0,0);
while(<>) {
  $y=0;
	$x++;
  @l = split(//, $_);
	while($c = pop(@l)) {
		$y++;
		if("$c" eq 'O') { print("new int[] {$x, $y},\n"); }
	}
}
print "});\n";
