/* hPong 0.3
 * <~~Harvie 2oo8
 * Released under Creative Commons
 *
 * Requirements:
 * - stty
 * - ansi terminal with resolution >= 80x24 characters
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//Settings
char pixchar = '#';

int turntime = 25; //Time of turn in ms (inverted speed)
int debugturntime = 0; //Turntime for debuging

unsigned char padsize = 1; //Size of pad
char padspeed = 2; //Speed of pad (pixels per keypress)

unsigned char ui_skill_l = 255; //Skill of UI = (0-255); 0=disabled, 255=godmode
unsigned char ui_skill_r = 200; //Skill of UI = (0-255); 0=disabled, 255=godmode
unsigned char debug = 0; //UI Debug mode

unsigned char music = 0; //Enable music? (Bool)
char music_cmd[] ="while true; do mplayer /usr/share/chiptunes/* >/dev/null 2>&1; done;";

unsigned char arduino = 0; //Enable Arduino? (Bool)
char arduinodev[] = "/dev/ttyUSB0";

//Stadium
unsigned char *stadium_bitmap[] = {
"#### hPong 0.3 ################################################",
"#                              |                              #",
"#                                                             #",
"#                              |                              #",
"#                                                             #",
"#                              |                              #",
"#                                                             #",
"#                              |                              #",
"#                                                             #",
"#                              |                              #",
"#                                                             #",
"#                              |                              #",
"#                                                             #",
"#                              |                              #",
"#                                                             #",
"#                              |                              #",
"########################################### <~~Harvie 2oo8 ####"};
char stadium_lines = 17; //15+2
char stadium_width = 63; //60+2+1

//Banners
unsigned char banner_start[] =
"\n\n\n\n\n\n\n\n\n\n\n"
"               ##         ####       by: <~~Harvie 2oo8\n"
"             ##           #   #\n"
"           ##     ##  ##  #### ###  ####   ###\n"
"         #######  ######  #   #   # #   # #   #\n"
"           ##     ##  ##  #    ###  #   #  ####\n"
"             ##   ##  ##                      #\n"
"               ## Press any key to start!  ###\n";

unsigned char banner_lose[] =
"\n\n\n\n\n\n\n\n\n\n\n\n\n"
"               ###  # #     ###  # #  #  ###  #\n"
"               # #  # #     #    # #  #   #   #\n"
"               # #  ###     ###  ###  #   #   #\n"
"               # #  # #       #  # #  #   #\n"
"               ###  # #     ###  # #  #   #   #\n"
"\n"
"               Score:        %d : %d\n"
"               (Press Any Key to continue...)\n";

//Numbers
char stadium_offset = 7;
char score_space[] = "                          ";
unsigned char *num_bitmaps[]={
"#### ## ## ####", //0
"  #  #  #  #  #", //1...
"###  #####  ###",
"###  ####  ####",
" # #  ###    # ",
"####  ###  ####",
"####  #### ####",
"###  # #  #  # ",
"#### ##### ####",
"#### ####  ####"}; //9

//Other Values
char stadium_max; //Set by init()

char ball_c = 1; //x
char ball_l = 0; //y
char ballmove_c = 1; //x
char ballmove_l = 1; //y

char player_l = 7; //Left
char player_r = 7; //Right

unsigned int score_l = 0;
unsigned int score_r = 0;

//Misc
char loop=0;
char temp;
FILE *musicfd = NULL;
FILE *arduinofd = NULL;

//Functions
void blank() {
  printf("\033[2J"); //Clear screen
  printf("\033[0;0H"); //L:C = 0:0
}

void quit() {
  if(arduino) fclose(arduinofd);
  ualarm(0, 0);
  system("stty icanon");
	blank();
  printf("hPong exited! Scrore was %d : %d\n<~~Harvie 2oo8\n", score_l, score_r);
  exit(255);
  if(music) pclose(musicfd);
}

void music_start() {
	if( (musicfd = popen(music_cmd, "r"))==NULL ) {
		printf("Cannot initialize music\n");
		quit();
	}
}

void win(char player) {
	switch(player) {
		case 'l':
			score_l++;
		  ball_l = 2; ball_c = stadium_width-3; ballmove_c = -1;
		break;
		
		case 'r':
			score_r++;
			ball_l = 2; ball_c = 3; ballmove_c = 1;
		break;
	}
	blank();
	printf(banner_lose, score_l, score_r);
	if(!debug) sleep(1);
	if(!arduino && (ui_skill_l == 0 || ui_skill_r == 0)) getchar();
	blank();
}

void reset_scr() {
  printf("\033[0;0H"); //L:C = 0:0
}

void pixel(char c, char line, char col) {
  printf("\033[%d;%dH", line, col);
  putchar(c);
  printf("\033[0;0H"); //L:C = 0:0
}

void draw_stadium() {
  unsigned char i, x, sl, sr;
  sl = score_l%10; sr = score_r%10;
	for(i=0;i<stadium_offset;i++) {
		printf("\033[K");
		if(i>=1 && i<=5) {
			x = (i-1)*3;
			printf("%s%c%c%c  .  %c%c%c", score_space,
				num_bitmaps[sl][x],	num_bitmaps[sl][x+1],	num_bitmaps[sl][x+2],
				num_bitmaps[sr][x],	num_bitmaps[sr][x+1],	num_bitmaps[sr][x+2]
			);
		}
		putchar('\n');
	}
	for(i=0;i<stadium_lines;i++) printf("%s\033[K\n", stadium_bitmap[i]);
	printf("\033[K");
}

void recalc() {
	//UI - follow ball
	if((unsigned char)rand() < ui_skill_l) { //Left
		if(player_l < ball_l) player_l++;
		if(player_l > ball_l) player_l--;
	}

  if((unsigned char)rand() < ui_skill_r) { //Right
	  if(player_r < ball_l) player_r++;
		if(player_r > ball_l) player_r--;
	}
	
	//Move ball
  ball_c += ballmove_c;
  ball_l += ballmove_l;
  
  //Pong (horizontal)
  if(ball_c == 1) { //left
  	if(abs(ball_l-player_l) <= padsize+1) {
  		ballmove_c = -ballmove_c;
  		ball_c++;
  	} else {
  		win('r');
  	}
  }
  
  if(ball_c == stadium_width-3) { //right
		if(abs(ball_l-player_r) <= padsize+1) {
			ballmove_c = -ballmove_c;
			ball_c--;
		} else {
			win('l');
		}
  }

  //Bounce ball (vertical)
	if(ball_l >= stadium_max || ball_l <= 0) ballmove_l =  -ballmove_l;
}

void redraw() {
	char i;
	//Clear
	reset_scr();

  //Stadium
  draw_stadium();
  
	//Check player pos limits	
	if(player_r < 0) player_r = 0;
	if(player_r > stadium_max) player_r = stadium_max;

	if(player_l < 0) player_l = 0;
	if(player_l > stadium_max) player_l = stadium_max;	
  
  //Pads
  for(i=-padsize;i<padsize+1;i++) pixel(pixchar, stadium_offset+2+i+player_l, 2);
  for(i=-padsize;i<padsize+1;i++) pixel(pixchar, stadium_offset+2+i+player_r, stadium_width-1);
  
  //Ball
  pixel(pixchar, stadium_offset+2+ball_l, 2+ball_c);
  fflush(stdout);
}

void splash() {
	blank();
	puts(banner_start);
	getchar();
	blank();
}

void control(unsigned char key) {
	switch(key) {
		case 'A': player_r-=padspeed; break; //A = Arrow UP
		case 'B': player_r+=padspeed; break; //B = Arrow DOWN
		case 'q': case 'Q':
			loop=0;
			quit();
		break;
		case 'e': case 'E': player_l-=padspeed; break; //e key
		case 'd': case 'D': player_l+=padspeed; break; //D key
	}

	redraw();
}

void alarm_handle(int signo) {
	recalc();
	redraw();
	if(signo == SIGTERM || signo == SIGINT || signo == SIGQUIT) quit();
}

void init() {
  //Blank screen
	blank();
	//Init random generator
	srand(time(NULL));
  //Disable both input buffers
  setbuf(stdin, NULL);
  system("stty -icanon");
  //Handle alarm (timer)
	signal(SIGALRM, alarm_handle);
	signal(SIGINT, alarm_handle); signal(SIGTERM, alarm_handle); signal(SIGQUIT, alarm_handle);
	//Compute lower sprite position in stadium
	stadium_max = stadium_lines - 3;
	//Debug mode
	if(debug) turntime = debugturntime;
	//Open Arduino
	if( arduino&&((arduinofd=fopen(arduinodev,"rb"))==NULL) ) {
		printf("Cannot open %s\n", arduinodev);
		exit(255);
	}
}

//Main... Short & simple ;)
int main() {
  init();
	splash();

	//Music
	if(music) music_start();

	loop = 1;
	ualarm(turntime*1000+1, turntime*1000+1);
  while(loop) {
    if(arduino) player_r = getc(arduinofd); //Arduino
    else control(getchar()); //Keyboard
  }
  quit();
}

