/*
 * Exit Rum 2
 * (c) Tomas 'Harvie' Mudrunka 2018
 */

//Nastaveni bomby

#define DISARM_CODE "12345678"
#define TIME_COUNTDOWN 600
#define TIME_COUNTDOWN 15

//Prirazeni pinu

#define PIN_CABLE_CHECK 2

#define PIN_KEY_CLK 3		//IRQ!
#define PIN_KEY_DATA 4

#define PIN_LED_G 5
#define PIN_LED_B 6
#define PIN_LED_R 7

#define PIN_COIL_LO 10
#define PIN_COIL_HI 11

#define ADDR_DISPLAY 0x70

//////////////////////////////////////////////////////////////////

#include <PS2Keyboard.h>

//#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

PS2Keyboard keyboard;
Adafruit_7segment display = Adafruit_7segment();

#define cnt_sec(s) (s%60)
#define cnt_min(s) (s/60)
#define cnt_dec(s) (cnt_min(s)*100+cnt_sec(s))

void setup()
{
    //Pripravime periferie
    Serial.begin(9600);
    keyboard.begin(PIN_KEY_DATA, PIN_KEY_CLK);
    display.begin(ADDR_DISPLAY);

    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    pinMode(PIN_COIL_LO, OUTPUT);
    pinMode(PIN_COIL_HI, OUTPUT);
    pinMode(PIN_CABLE_CHECK, INPUT_PULLUP);
}

void loop()
{
    //Vynulujem periferie
    digitalWrite(PIN_COIL_LO, LOW);
    digitalWrite(PIN_COIL_HI, LOW);
    analogWrite(PIN_LED_R, 0);
    analogWrite(PIN_LED_G, 0);
    analogWrite(PIN_LED_B, 0);

    display.clear();
    display.setBrightness(15);
    display.writeDisplay();

    while (keyboard.available()) keyboard.read();

    //Pockame na pripojeni kabelu
    display.print(0xCAB1, HEX);
    display.writeDisplay();
    while (digitalRead(PIN_CABLE_CHECK))
	delay(100);

    //Pockame na odpojeni kabelu
    display.clear();
    display.writeDisplay();
    while (!digitalRead(PIN_CABLE_CHECK))
	delay(100);

    //Odpocitavame
    int guessed = 0, disarmed = 0;

    for (int cnt = TIME_COUNTDOWN; cnt >= 0 && !disarmed; cnt--) {
	display.print(cnt_dec(cnt), DEC);
	display.drawColon((cnt + 1) % 2);
	display.writeDisplay();

	analogWrite(PIN_LED_R, random(0, 3));
	analogWrite(PIN_LED_G, random(0, 3));
	analogWrite(PIN_LED_B, random(0, 3));

	tone(PIN_COIL_LO, 1000, 30);

	delay(1000);

	//Cteme klavesnici
	while (keyboard.available()) {
	    char c = keyboard.read();
	    if (c == PS2_ENTER) {
		Serial.println();
	    } else {
		Serial.print(c);
	    }
	    if (c == DISARM_CODE[guessed]) {
		guessed++;
		Serial.println("\nGOT!");
	    } else {
		guessed = 0;
		Serial.println("\nFAIL!");
	    }
	    if (DISARM_CODE[guessed] == 0) {
		disarmed = 1;
		Serial.println("\nDISARMED!");
	    }
	}
    }

    if (disarmed) {
	//Bomba byla deaktivovana
	tone(PIN_COIL_LO, 3000, 500);
	delay(500);
	tone(PIN_COIL_LO, 5000, 500);
	analogWrite(PIN_LED_R, 0);
	analogWrite(PIN_LED_G, 3);
	analogWrite(PIN_LED_B, 0);
	display.print(0xDEFD, HEX);
	display.writeDisplay();
	delay(1000);

    }

    char s = digitalRead(PIN_CABLE_CHECK);
    while (digitalRead(PIN_CABLE_CHECK) != s) {

	if (!disarmed) {

	    //Bomba vybouchla
	    int i;

	    analogWrite(PIN_LED_R, random(0, 255));
	    analogWrite(PIN_LED_G, random(0, 0));
	    analogWrite(PIN_LED_B, random(0, 255));

	    for (i = 0; i < 10; i++)
		display.writeDigitRaw(i, random(0, 255));
	    display.setBrightness(random(0, 15));
	    display.writeDisplay();

	    digitalWrite(PIN_COIL_HI, HIGH);
	    delay(20);
	    digitalWrite(PIN_COIL_HI, LOW);
	    int rnd = random(30, 100);
	    tone(PIN_COIL_LO, random(100, 8000), rnd);
	    delay(rnd);
	}
    }

}
