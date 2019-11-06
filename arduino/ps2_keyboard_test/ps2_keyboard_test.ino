//Source: http://dduino.blogspot.com/2011/11/arduino-ps2-emulator-computer-control.html

//KBD stuff
#include "ps2dev.h"  // to emulate a PS/2 device
PS2dev keyboard(19,18);  // PS2dev object (2:data, 3:clock)
int enabled = 0;  // pseudo variable for state of "keyboard"
unsigned char c;  //char stores data recieved from computer for KBD

//Sonar stuff

void ack()
{
  //acknowledge commands
  while(keyboard.write(0xFA));
}

int keyboardcommand(int command)
{
  unsigned char val;
  switch (command)
  {
  case 0xFF: //reset
    ack();
    //the while loop lets us wait for the host to be ready
    while(keyboard.write(0xAA)!=0);
    break;
  case 0xFE: //resend
    ack();
    break;
  case 0xF6: //set defaults
    //enter stream mode
    ack();
    break;
  case 0xF5: //disable data reporting
    //FM
    enabled = 0;
    ack();
    break;
  case 0xF4: //enable data reporting
    //FM
    enabled = 1;
    ack();
    break;
  case 0xF3: //set typematic rate
    ack();
    keyboard.read(&val); //do nothing with the rate
    ack();
    break;
  case 0xF2: //get device id
    ack();
    keyboard.write(0xAB);
    keyboard.write(0x83);
    break;
  case 0xF0: //set scan code set
    ack();
    keyboard.read(&val); //do nothing with the rate
    ack();
    break;
  case 0xEE: //echo
    //ack();
    keyboard.write(0xEE);
    break;
  case 0xED: //set/reset LEDs
    ack();
    keyboard.read(&val); //do nothing with the rate
    Serial.print("LEDs: ");
    Serial.println(val, HEX);
    digitalWrite(LED_BUILTIN, val);
    ack();
    break;
  }
  return 0;
}

void setup()
{
  // send the keyboard start up
  while(keyboard.write(0xAA)!=0);
  delay(10);
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  //if host device wants to send a command:
  if(keyboard.available())
  {
    while(keyboard.read(&c));
    //delay(100);
    keyboardcommand(c);
    //delay(100);
    //c = 0;
  }
  

    delay(1000);

    keyboard.write(0x16);
    keyboard.write(0xF0);
    keyboard.write(0x16);
    Serial.print('.');
  
    //delay(1000);
  
  
}
