/*Copy the code below and up load it to the I/O Board
then put some kind of Antenna into PIN 8...PIN 8!!!!
Then tune to AM 1337kHz.
Optional:LED in pin 13 to see what is broadcasting,
also a protoshield to add multiple antenna's.
Lol that code will do morse code saying "I like ice cream";
*********THE CODE****************************************************
*/
//start of code 
//WWW.JONNYBLANCH.WEBS.COM 
long millisAtStart = 0;
long millisAtEnd = 0;

const long period_broadcast = 8;	//period of shortest broadcast (256 port changes) 

#define LENGTH_DIT 64
//number of period_broadcasts in one 'dit', 
//all other lengths are scaled from this 
//Broadcasts on 1337 kHz 

const int length_dit = LENGTH_DIT;	//number of periods for dit 
const int pause_dit = LENGTH_DIT;	//pause after dit 
const int length_dah = 3 * LENGTH_DIT;	//number of persots for dah 
const int pause_dah = LENGTH_DIT;	//pause after dah 
const int length_pause = 7 * LENGTH_DIT;	//pause between words 

void dit(void);
void dah(void);
void pause(void);
void broadcast(int N_cycles);
void dontbroadcast(int N_cycles);

// ### INC ### Increment Register (reg = reg + 1) 
#define ASM_INC(reg) asm volatile ("inc %0" : "=r" (reg) : "0" (reg))

void setup() {

	Serial.begin(9600);
	DDRB = 0xFF;		//Port B all outputs 
	//Do one dit to determine approximate frequency 
	millisAtStart = millis();
	dit();
	millisAtEnd = millis();
	Serial.print(millisAtEnd - millisAtStart);
	Serial.print(" ");
	Serial.print((length_dit + pause_dit) * period_broadcast * 256 /
		     (millisAtEnd - millisAtStart) / 2);
	Serial.print("kHz ");
	Serial.println();
}

void loop() {
	dit();
	dit();
	delay(500);
	dit();
	dah();
	dit();
	dit();
	delay(250);
	dit();
	dit();
	delay(250);
	dah();
	dit();
	dah();
	delay(250);
	dit();
	delay(500);
	dit();
	dit();
	delay(250);
	dah();
	dit();
	dah();
	dit();
	delay(250);
	dit();
	delay(500);
	dah();
	dit();
	dah();
	dit();
	delay(250);
	dit();
	dah();
	dit();
	delay(250);
	dit();
	delay(250);
	dit();
	dah();
	delay(250);
	dah();
	dah();
	delay(2000);

}

void dit(void) {
	for (int i = 0; i < length_dit; i++)
	{

		broadcast(period_broadcast);

	}

	for (int i = 0; i < pause_dit; i++)
	{

		dontbroadcast(period_broadcast);

	}

}

void dah(void) {
	for (int i = 0; i < length_dah; i++)
	{

		broadcast(period_broadcast);

	}

	for (int i = 0; i < pause_dah; i++)
	{

		dontbroadcast(period_broadcast);

	}

}

void pause(void) {
	for (int i = 0; i < length_pause; i++)
	{

		dontbroadcast(period_broadcast);

	}

}

void broadcast(int N_cycles) {
	unsigned int portvalue;
	for (int i = 0; i < N_cycles; i++)
	{

		portvalue = 0;

		do {
			PORTB = portvalue;
			ASM_INC(portvalue);
		}
		while (portvalue < 255);
	}
}

void dontbroadcast(int N_cycles) {
	unsigned int portvalue;
	PORTB = 0x00;
	for (int i = 0; i < N_cycles; i++)
	{

		portvalue = 0;

		do {
			ASM_INC(portvalue);
			//add some assembly No OPerations to keep timing the same 
			asm volatile ("NOP");
		}
		while (portvalue < 255);
	}
}

//end of code 
