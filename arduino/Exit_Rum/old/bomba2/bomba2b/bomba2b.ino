/*
*/

//Nastaveni bomby

#define TIME_COUNTDOWN 600


//Prirazeni pinu

#define PIN_CABLE_CHECK 2

#define PIN_KEY_CLK 3 //IRQ!
#define PIN_KEY_DATA 4

#define PIN_LED_R 5
#define PIN_LED_G 6
#define PIN_LED_B 7

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

void setup() {
  Serial.begin(9600);
  Serial.println("Keyboard Test:");
  
  keyboard.begin(PIN_KEY_DATA, PIN_KEY_CLK);
  
  display.begin(ADDR_DISPLAY);
  display.clear();
  //display.writeDisplay();

  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_COIL_LO, OUTPUT);
  pinMode(PIN_COIL_HI, OUTPUT);
  pinMode(PIN_CABLE_CHECK, INPUT_PULLUP);
}

void loop() {
  if (keyboard.available()) {
    
    // read the next key
    char c = keyboard.read();
    
    // check for some of the special keys
    if (c == PS2_ENTER) {
      Serial.println();
    } else {
      Serial.print(c);
    }

    if(digitalRead(PIN_CABLE_CHECK)) c+=16;
      display.print(c, HEX);
      display.drawColon(c%2);
      //display.setBrightness(random(0,15));
      display.setBrightness(0);
      //display.clear();
      display.writeDisplay();
      

    if(c%2) {
        analogWrite(PIN_LED_R, random(0,255));   // turn the LED on (HIGH is the voltage level)
        analogWrite(PIN_LED_G, random(0,255));   // turn the LED on (HIGH is the voltage level)
        analogWrite(PIN_LED_B, random(0,255));   // turn the LED on (HIGH is the voltage level)
        digitalWrite(PIN_COIL_HI, HIGH);   // turn the LED on (HIGH is the voltage level)
    } else {
        digitalWrite(PIN_COIL_LO, HIGH);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(PIN_LED_R, LOW);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(PIN_LED_G, LOW);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(PIN_LED_B, LOW);   // turn the LED on (HIGH is the voltage level)
    }
        delay(random(1,15));                       // wait for a second
        digitalWrite(PIN_COIL_LO, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(PIN_COIL_HI, LOW);    // turn the LED off by making the voltage LOW

          if(c == '.') {
            int d = random(100,1000);
            for(int i = 0; i < 1000; i++) {
          
      digitalWrite(PIN_COIL_LO, HIGH); //digitalWrite(11, HIGH);
      delayMicroseconds(d/2);
      digitalWrite(PIN_COIL_LO, LOW); //digitalWrite(11, LOW);
      delayMicroseconds(d/2);
          }
    }
  }


}
