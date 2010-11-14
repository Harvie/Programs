/*
 * Bit-bang sound
 * <~~Harvie 2oo8
 */

#define sndout 13

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

float get_tone(char tone, char octave) { //Return tone frequency or 0
  if(octave+tone<=0) return 0;
  float freq;
  switch (tone) { //THX2MS GW-BASIC Reference 4freqs muhaha ;D
    case 'c': case 'C': freq=130.81; break;
    case 'd': case 'D': freq=146.83; break;
    case 'e': case 'E': freq=164.81; break;
    case 'f': case 'F': freq=174.61; break;
    case 'g': case 'G': freq=196; break;
    case 'a': case 'A': freq=220; break;
    case 'b': case 'B': freq=246.94; break;
    default: return 0;
  }
  return (freq*pow(2,octave-1));
}

void play_melody(char sndpin, char *melody, char octave, int duration) {
  for(char i=0;melody[i]!=0;i++) sound(sndpin, get_tone(melody[i], octave), duration);
  return;
}

void setup() { // run once, when the sketch starts
  Serial.begin(115200);
}

void play_drum(char sndpin, char drum) {
  char c; int i; float f;
  switch (drum) {
    //BassDrums
    case 'b': for(f=0;f<30;f+=4) {sound(sndpin, 60, 20); delay(f);} break;
    case 'B': for(i=0;i<=150;i+=10) sound(sndpin, i, 10); break;
    case 'd': for(i=150;i>=0;i-=10) sound(sndpin, i, 10); break;
    case 'D': for(i=100;i>=40;i-=10) sound(sndpin, i, 6); break;
    //Snares
    case 's': for(f=0;f<1;f+=0.1) sound(sndpin, sin(f)*1000, 5); break;
    case 'S': for(f=0;f<1;f+=0.1) sound(sndpin, sin(f)*1000+(rand()/100), 5); break;
    case 'z': for(f=0;f<1;f+=0.15) { sound(sndpin, sin(f)*1000+(rand()/1000), 5); delay(f*10); } break;
    //Crashes
    case 'c': for(f=1.2;f<1.5;f+=0.01) { sound(sndpin, 700+(rand()/500), 2); delay(1/sin(f)); } break;
    case 'C': for(f=1;f<1.3;f+=0.01) { sound(sndpin, (70+(rand()/500))*(f/0.1), 2); /*delay(1/sin(f));*/ } break;    
    //Ambient
    case 'r': for(i=0;i<300;i++) sound(sndpin, rand()*100, 5); break;
    //Intros
    case 'i': for(i=0;i<150;i++) sound(sndpin, i*100, 5); break;
    case 'I': for(f=0;f<1;f+=0.01) {sound(sndout, sin(f)*550, 10); sound(sndout, cos(f)*400, 10); } break;
    //Outros
    case 'o': for(f=1;f>0.3;f-=0.01) { sound(sndpin, sin(f)*1000+(rand()/100), 5); delay(40/f); } break;
    default: delay(100);
  }
}

void play_rythm(char sndpin, char *rythm) {
  for(char i=0;rythm[i]!=0;i++) play_drum(sndpin, rythm[i]);
  return;
}

void loop() { // run over and over again
  //delay(10000); return;//Silence! I'll kill you!
  //while(1) sound(sndout, rand()/20, 10);
  //int i=0; while(1) sound(sndout, i++, 10);
  //float i=0; while(1) sound(sndout, sin(i+=0.01)*600, 10);
  //float i=0; while(1) { sound(sndout, sin(i+=0.01)*550, 10); sound(sndout, cos(i)*400, 10); }
  //sound(sndout, 440, 30000);
  
  //play_drum(sndout, 'D'); delay(1000); return;
  //play_drum(sndout, Serial.read()); return;
  
  /*
  play_rythm(sndout, "r  iI ");
  play_melody(sndout, "c c c cc dd d eee    ", 1, 100);
  play_rythm(sndout, "B   D   B   d s B   D s B   d c B s D C B   d S B   D S B  b s d So  ");
  */
  
  sound(sndout, analogRead(0), 10); return;
  
/*
  char rythm[]=
"b "
"D D D d D d "
"D D D d D d "
"i "
"D D D d D d "
"D D D d D d "
"I "
"D s D S D d "
"D D z d D d "
"ddDDb "
"D D D d D s "
"D D D d D s "
"o";

  play_rythm(sndout, rythm);
*/
  /*int octave, duration;
  char melody[]="CCDEECCFFAAGGE-CCDEGGEFEDDCC-CCDEECCFFAAGGE-CCDEGGEFEDDC"; octave=1; duration=300; //Bob Marley's Redemption song - intro ;D
  play_melody(sndout, melody, octave, duration);*/
  delay(4000);
}

