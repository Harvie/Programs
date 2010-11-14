/*
 * Arduino IR-Jammer
 * <~~Harvie 2oo8
 *
 * You can look for couple of old TV remote controlls
 * to extract some IR LEDs...
 *
 * Each TV should have LED of specific "color"
 * (even you can't see it). So its better to connect
 * more LEDs with different light frequency (color).
 * Specialy if jammer is working only from small distance.
 *
 */

#define ledpin 13
//Maybe you will have to experiment also with this value
int bitlen = 5;
int d;
float i;

void setup() {
  pinMode(ledpin, OUTPUT);
}

void loop() {
  for(d=1000/100;d<=1000/20;d++) { // 1000/frequency in kHz
  //You maybe have to experiment with ^^^ this ^^^ values
  //Frequency sweeping: 100->20 kHz works, i saw 60->1 too
    for(i=0;i<bitlen;i+=0.5) {
      digitalWrite(ledpin, HIGH); delayMicroseconds(d/2);
      digitalWrite(ledpin, LOW ); delayMicroseconds(d/2);
    }
    delayMicroseconds(d*bitlen);
  }
}

