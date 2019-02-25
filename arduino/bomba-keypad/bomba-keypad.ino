/*
 * PS2 pinout cinskej kabel: data cerveny (18), clock bily (19), 5V cerny, GND zluty
 */


#include <ps2dev.h>

PS2dev keyboard(19, 18); //clock, data

#include <LedControl.h>

LedControl lc=LedControl(16,14,15,2); //datain, clk, load, number of chips
unsigned char dispbuf[32];

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

char scancodes[] = {0x45, 0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x7C, 0x7C}; //Scancodes for numbers 0-9, *, #

void cleardisp(char val) {
  for(int i = 0; i<8; i++) dispbuf[i] = val;
}

void drawdisp(int shift) {
  lc.clearDisplay(0);
  for(int i = 0; i<8; i++) {
    if(shift) dispbuf[i] = dispbuf[i+1];
    if(dispbuf[i] != '_') lc.setDigit(0,7-i,dispbuf[i],false); //addr, digit, value, decimalpoint
  }
}

void intro() {
  cleardisp('_');
  for(int i = 0; i<100; i++) {
    analogWrite(keyledPin, i);
    if(i >= 20*1) dispbuf[3] = dispbuf[4] = 0;
    if(i >= 20*2) dispbuf[2] = dispbuf[5] = 0;
    if(i >= 20*3) dispbuf[1] = dispbuf[6] = 0;
    if(i >= 20*4) dispbuf[0] = dispbuf[7] = 0;
    drawdisp(0);
    delay(20);
  }
}

void outro() {
  cleardisp(0);
  for(int i = 100; i>=0; i--) {
    analogWrite(keyledPin, i);
    if(i <= 20*1) dispbuf[3] = dispbuf[4] = '_';
    if(i <= 20*2) dispbuf[2] = dispbuf[5] = '_';
    if(i <= 20*3) dispbuf[1] = dispbuf[6] = '_';
    if(i <= 20*4) dispbuf[0] = dispbuf[7] = '_';
    drawdisp(0);
    delay(20);
  }
}


void setup(){
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,15); //0 - 15
  /* and clear the display */
  lc.clearDisplay(0);
  
  intro();
  cleardisp('_'); drawdisp(0);
  
  // send the keyboard start up
  keyboard.keyboard_init();
  
  Serial.begin(9600);



  pinMode(LED_BUILTIN, OUTPUT);
}
  
void loop(){
  unsigned char leds;
  if(keyboard.keyboard_handle(&leds)) {
    //Serial.print('LEDS');
    //Serial.print(leds, HEX);
    digitalWrite(LED_BUILTIN, leds);
  }
  
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
    //analogWrite(keyledPin, random(0, 255));
    analogWrite(keyledPin, 255);
    unsigned char numkey = customKey-0x30;
    if(numkey < 10) {
      //Send PS2
      keyboard.keyboard_mkbrk(scancodes[numkey]);
      
      //Single digit
      //lc.setDigit(0,7,numkey,false); //addr, digit, value, decimalpoint

      //Scroll out display
      dispbuf[8] = numkey;
      drawdisp(1);
    }
  }

  if (customKey == '*') {
    //Send PS2
    keyboard.keyboard_mkbrk(scancodes[10]);
    
    //analogWrite(keyledPin, 0);
    outro();

    cleardisp('_');
    drawdisp(0);
  }

  if (customKey == '#') {
    keyboard.keyboard_mkbrk(0x5A); //enter
  }
}
