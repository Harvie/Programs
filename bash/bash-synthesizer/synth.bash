#!/bin/bash
{
	for x in {0..100}; {
		for i in {-2..2}; {
			i=$[$i*$x];
			echo -ne "\x$i";
		}
	}
} | aplay # -r 44100
