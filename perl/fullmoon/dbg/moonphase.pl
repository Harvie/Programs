#!/usr/bin/env perl
use strict;
use warnings;
use Astro::MoonPhase;

###
use locale;
#use POSIX qw(strftime localtime);

#use I18N::Langinfo qw(langinfo ABDAY_1 YESSTR NOSTR);

#I18N::Langinfo->import(qw(langinfo CODESET));
#my $codeset = langinfo(CODESET()); # note the ()
###

my $seconds_since_1970 = time();

      my ( $MoonPhase,
          $MoonIllum,
          $MoonAge,
          $MoonDist,
          $MoonAng,
          $SunDist,
          $SunAng )  = phase($seconds_since_1970);

          my @info = phase($seconds_since_1970);
					print "@info\n";

my @phases = phasehunt();
    print "New Moon      = ", scalar(localtime($phases[0])), "\n";
    print "First quarter = ", scalar(localtime($phases[1])), "\n";
    print "Full moon     = ", scalar(localtime($phases[2])), "\n";
    print "Last quarter  = ", scalar(localtime($phases[3])), "\n";
    print "New Moon      = ", scalar(localtime($phases[4])), "\n";


    my @name = ("New Moon", "First quarter", "Full moon", "Last quarter");
    my ($phase, @times) = phaselist(time(), (time()+60*60*24*365));

    while (@times) {
      printf "%-14s= %s\n", $name[$phase], scalar localtime shift @times;
      #printf "%-14s= %s\n", $name[$phase], scalar shift @times;
      $phase = ($phase + 1) % 4;
    }

