/*
 * Arduino TV-B-GONE
 * <~~Harvie 2oo8
 *
 * THX 2 Ladyada
 * http://www.ladyada.net/make/tvbgone/
 *
 */

//#include "NAcodes.c"
//#include "NAcodes2.c"
#define ledpin 13
//Maybe you will have to experiment also with this value
int d;
float i;

void fire_code(int outpin, long freq, fuck *code) { //kHz
  freq = 1000000/(freq*1000); //freq2millis
  for()
}

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

