#include <ps2dev.h>  // to emulate a PS/2 device
PS2dev keyboard(19,18);  // PS2dev object (3:clock, 2:data)
unsigned long timecount = 0;

void setup()
{
  keyboard.keyboard_init();
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  //Handle PS2 communication and react to keyboard led change
  unsigned char leds;
  if(keyboard.keyboard_handle(&leds)) {
    //Serial.print('LEDS');
    //Serial.print(leds, HEX);
    digitalWrite(LED_BUILTIN, leds);
  }
  
  //Print letter every second
  if((millis() - timecount) > 1000) {
    keyboard.keyboard_mkbrk(0x16);
    Serial.print('.');
    timecount = millis();
  }  
}
