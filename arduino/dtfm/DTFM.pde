/*
 * DTFM
 * <~~Harvie 2oo8
 */
 
/* 
DTMF keypad frequencies (with sound clips)
      1209 Hz 1336 Hz 1477 Hz 1633 Hz
697 Hz 	1 	2 	3 	A
770 Hz 	4 	5 	6 	B
852 Hz 	7 	8 	9 	C
941 Hz 	* 	0 	# 	D

DTMF event frequencies
Event 	          Low frequency High frequency
Busy signal 	        480 Hz 	620 Hz
Dial tone 	        350 Hz 	440 Hz
Ringback tone (US) 	440 Hz 	480 Hz
*/

#define dtfmpin 13

void dtfm_tone(char outpin, float low, float high) {
  int duration = 300;
  char pin;
  long pos, startmillis = millis();
  while((pos = millis()-startmillis) <= duration) {
    pin = false;
    if( sin((pos*low)/1000*(2*PI)) > 0) pin = true;
    if( sin((pos*high)/1000*(2*PI)) > 0) pin = true;
    
    if(pin) digitalWrite(outpin, HIGH); else digitalWrite(outpin, LOW);
  }
}

int dtfm_key(char outpin, char key) {
  switch (key) {
    case '1': dtfm_tone(outpin, 697, 1209); break;
    case '2': dtfm_tone(outpin, 697, 1336); break;
    case '3': dtfm_tone(outpin, 697, 1477); break;
    
    case '4': dtfm_tone(outpin, 770, 1209); break;
    case '5': dtfm_tone(outpin, 770, 1336); break;
    case '6': dtfm_tone(outpin, 770, 1477); break;
    
    case '7': dtfm_tone(outpin, 852, 1209); break;
    case '8': dtfm_tone(outpin, 852, 1336); break;
    case '9': dtfm_tone(outpin, 852, 1477); break;
    
    case '*': dtfm_tone(outpin, 941, 1209); break;
    case '0': dtfm_tone(outpin, 941, 1336); break;
    case '#': dtfm_tone(outpin, 941, 1477); break;
                               
    default: return 0;
  }
  return 1;
}

void dtfm_dial(char outpin, char *keys) {
  for(char i=0;keys[i]!=0;i++) { dtfm_key(outpin, keys[i]); delay(100); }
}

void setup()                    // run once, when the sketch starts
{
  pinMode(dtfmpin, OUTPUT);      // sets the digital pin as output
  //dtfm_dial(dtfmpin, "739136794");
  dtfm_tone(dtfmpin, 1, 10);
}

void loop()                     // run over and over again
{
delay(10000);
}
