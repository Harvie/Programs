#!/usr/bin/perl
use strict;
use warnings;

use Imager;
use Image::ObjectDetect;

my $file = '2.jpg';
#my $file = 'input.jpg';
my $image = Imager->new->read(file => $file);

my @cascades = (
	#'/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml',
	#'/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml',
	'/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml', #less sensitive
	#'/usr/share/opencv/haarcascades/haarcascade_frontalface_alt_tree.xml',
	#'/usr/share/opencv/haarcascades/haarcascade_profileface.xml',

	#'/usr/share/opencv/haarcascades/haarcascade_upperbody.xml',
	#'/usr/share/opencv/haarcascades/haarcascade_lowerbody.xml',
	#'/usr/share/opencv/haarcascades/haarcascade_fullbody.xml',

	#'/usr/share/opencv/haarcascades/haarcascade_mcs_eyepair_small.xml',
	#'/usr/share/opencv/haarcascades/haarcascade_mcs_eyepair_big.xml',
);

my @faces;
foreach(@cascades) {
	push (@faces, detect_objects($_, $file));
}

for my $face (@faces) {
    my $file2 = 'paroubek.jpg';
    my $image2 = Imager->new->read(file => $file2);
    $image2 = $image2->scale(xpixels => $face->{width}, ypixels => $face->
{height});
    $image->paste(left => $face->{x}, top => $face->{y}, src => $image2);
}

$image->write(file => 'output.jpg');

print "Content-type: text/html\n\n";
print "<head>\n";
print "<TITLE>face detect</TITLE>\n";
print "</HEAD>\n";
print "<BODY>\n";
print "<br>\n";
print "<img src='input.jpg'>\n";
print "<img src='face.jpg'>\n";
print "<img src='output.jpg'>\n";
print "</BODY>\n";
print "</HTML>\n";
