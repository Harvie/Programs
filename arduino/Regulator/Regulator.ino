/* Software voltage regulator using analog read and PWM
   Connect both voltage sensor and output pin to possitive side of capacitor
   Then connect negative side to ground
   Voltage should be approximatelly stabilized at AREF/2
   You can use some push-pull (totem pole) kind of buffer at output pin to supply more current
*/


#define SENSOR A0
#define OUTPIN 11

void setup()  { 
} 

void loop()  { 
  analogWrite(OUTPIN, map(analogRead(SENSOR), 0, 1023, 255, 0));
}


