/*
 * Bit-bang sound
 * <~~Harvie 2oo8
 */

#define sndout 13

char mixer;

void sound(char sndpin, float freq, float duration) { //Play bit-bang sound
  if(duration<=0) return; if(freq<=0) { delay(duration); return; }
  freq=((1000000/2)/freq); //Convert freq to delay (us)
  duration*=1000; //Convert duration to us
  pinMode(sndpin, OUTPUT);
  for(;duration>0;duration-=2*freq) {
    digitalWrite(sndpin, HIGH); delayMicroseconds(freq);
    digitalWrite(sndpin, LOW);  delayMicroseconds(freq);
  }
  pinMode(sndpin, INPUT); //Close pin to avoid noise (optional)
}

void setup() { // run once, when the sketch starts
  pinMode(sndout, OUTPUT);
  pinMode(sndout-1, INPUT);
}

void loop() { // run over and over again
  //delay(10000); return;//Silence! I'll kill you!
  //float i=0; while(1) { sound(sndout, sin(i+=0.01)*550, 10); sound(sndout, cos(i)*400, 10); }
  digitalWrite(sndout, digitalRead(sndout-1));
  /* int octave, duration;
  char melody[]="CCDEECCFFAAGGE-CCDEGGEFEDDCC-CCDEECCFFAAGGE-CCDEGGEFEDDC"; octave=1; duration=300; //Bob Marley's Redemption song - intro ;D
  play_melody(sndout, melody, octave, duration); */
  //delay(2000);
}

