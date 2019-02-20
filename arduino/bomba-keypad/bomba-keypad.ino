/*
 * PS2 pinout cinskej kabel: data cerveny (18), clock bily (19), 5V cerny, GND zluty
 */


#include <ps2dev.h>

PS2dev keyboard(19, 18); //clock, data

#include <LedControl.h>

LedControl lc=LedControl(16,14,15,2); //datain, clk, load, number of chips

#include <Keypad.h>

int keyledPin = 3;

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void make_break_kb(byte data)
{
// makes and breaks the key
    keyboard.write(data);
    delay(50);
    keyboard.write(0xF0);
    delay(50);
    keyboard.write(data);
    delay(50);
}

char scancodes[] = {0x45, 0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x7C, 0x7C}; //Scancodes for numbers 0-9, *, #

void setup(){
  Serial.begin(9600);

    /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,15); //0 - 15
  /* and clear the display */
  lc.clearDisplay(0);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
    analogWrite(keyledPin, random(0, 255));
    unsigned char numkey = customKey-0x30;
    if(numkey < 10) {
      lc.setDigit(0,0,numkey,false); //addr, digit, value, decimalpoint
      lc.setDigit(0,7,numkey,false); //addr, digit, value, decimalpoint
      make_break_kb(scancodes[numkey]);
    }
  }

  if (customKey == '#') {
    lc.clearDisplay(0);
    analogWrite(keyledPin, 0);
    make_break_kb(scancodes[10]);
  }
}
