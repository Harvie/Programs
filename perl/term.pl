#!/usr/bin/env perl

           use Term::UI;
           use Term::ReadLine;

           my $term = Term::ReadLine->new('brand');

           my $reply = $term->get_reply(
                           prompt => 'What is your favourite colour?',
                           choices => [qw|blue red green|],
                           default => blue,
           );

           my $bool = $term->ask_yn(
                               prompt => 'Do you like cookies?',
                               default => 'y',
                       );


           my $string = q[some_command -option --no-foo --quux='this thing'];

           my ($options,$munged_input) = $term->parse_options($string);


           ### don't have Term::UI issue warnings -- default is '1'
           $Term::UI::VERBOSE = 0;

           ### always pick the default (good for non-interactive terms)
           ### -- default is '0'
           $Term::UI::AUTOREPLY = 1;

           ### Retrieve the entire session as a printable string:
           $hist = Term::UI::History->history_as_string;
           $hist = $term->history_as_string;


