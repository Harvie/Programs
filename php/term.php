#!/usr/bin/php
<?php
/*
 * Name: term.php
 * Description: Linux terminal functions for PHP
 * Author: <-Harvie 2oo7
 * THX2Root.cz: http://www.root.cz/clanky/barvy-pro-shell/ (see for more help)
 */

//Font
function term_font($atr1 = 0, $atr2 = -1) { //Set color of BG/FG and font
	if($atr2 >= 0) { echo("\033[".trim($atr1).';'.trim($atr2).'m'); }
	else { echo("\033[".trim($atr1).'m'); }
}
//Screen clearing
function term_cls() { echo("\033[2J"); } //Clear screen
function term_free_line() { echo("\033[K"); } //Delete line from cursor to end
//Movement
function term_return() { echo("\r"); } //Move cursor to 1st col
function term_tab() { echo("\t"); } //Print TAB
function term_newline() { echo("\n"); } //Move cursor to next line
function term_up($n) { echo("\033[".$n.'A'); } //Move cursor $n rows up
function term_down($n) { echo("\033[".$n.'B'); } //Move cursor $n rows down
function term_right($n) { echo("\033[".$n.'C'); } //Move cursor $n cols right
function term_left($n) { echo("\033[".$n.'D'); } //Move cursor $n cols left
//Position
function term_pos($row, $col) { echo('\033['.$row.';'.$col.'H'); } //Move cursor at $row and $col
function term_fpos($row, $col) { echo('\033['.$row.';'.$col.'f'); } //Move cursor at $row and $col
//Position saving
function term_pos_save() { echo("\033[s"); } //Save cursor possition (maybe not supported)
function term_pos_restore() { echo("\033[u"); } //Restore cursor possition (maybe not supported)
//Info about terminal
function term_name() { return getenv('TERM'); } //Returns terminal name as string
function term_color_name() { return getenv('COLORTERM'); } //Returns color terminal name as string
function term_shell() { return getenv('SHELL'); } //Returns running shell as string
function term_directory() { return getenv('PWD'); } //Returns working directory as string
function term_language() { return getenv('LANG'); } //Returns local settings (langue.codepage) as string
//Showcase
function term_demo() { //Print demonstration
	term_font();
	echo("DEMOnstration of PHPTerm for Unixs' terminals\n".
		term_name().' - '.term_color_name().' ('.term_shell().
		")\n\t\t\t\t<-Harvie 2oo7\n\n");
	for($i = 0; $i <= 8; $i++) {
		for($x = 30; $x <= 37; $x++) {
			term_font($i,$x); echo("A");
		}
		term_font();
		for($y = 40; $y <= 47; $y++) {
			term_font($y); echo("A");
			for($x = 30; $x <= 37; $x++) {
				term_font($i,$x); echo("A");
			}
		}
		term_font();
		echo("\n");
	}
	term_font(); echo("\n");
}


term_demo();