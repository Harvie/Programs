// Fading LED 
// by BARRAGAN <http://people.interaction-ivrea.it/h.barragan> 

int ledpin = 11;                           // light connected to digital pin 9
 
void setup() 
{ 
  pinMode(ledpin, OUTPUT);
} 
 
void loop() 
{ 
    analogWrite(ledpin, analogRead(0));           // sets the value (range from 0 to 255) 
} 
