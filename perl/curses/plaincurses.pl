#!/usr/bin/env perl
use Curses;

initscr;
END{endwin;}

printw("Normalni text\n");
attron(A_BOLD);
printw("Tucny text\n");
attron(A_UNDERLINE);
printw("Tucny a podtrzeny text\n");
attron(A_BLINK);
printw("Tucny, podtrzeny a blikajici text\n");
attron(A_STANDOUT);
printw("Jeste navic zvyrazneny text\n");
attroff(A_BLINK);
printw("Vypiname blikani\n");
attrset(A_BLINK | A_BOLD);
printw("Nyni je text pouze blikajici a tucny\n");

getch;
