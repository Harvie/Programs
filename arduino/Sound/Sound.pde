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

void sound_test() { //Check da' sound ;))
  sound(sndout, 440, 500);
  sound(sndout, get_tone('a', 1), 500);
  return;
}

void play_melody(char sndpin, char *melody, char octave, int duration) {
  for(char i=0;melody[i]!=0;i++) sound(sndpin, get_tone(melody[i], octave), duration);
  return;
}

void stupnice_priklad() {
  char oktava = 1;
  int delka = 500;
  char stupnice[]="cdefgab";
  
  play_melody(sndout, stupnice, oktava, delka);
  sound(sndout, get_tone(stupnice[0], oktava+1), delka); //C
}

void setup() { // run once, when the sketch starts

}

void loop() { // run over and over again
  //delay(10000); return;//Silence! I'll kill you!
  //--->Fun with sound (uncomment one):
  //sound_test();
  //stupnice_priklad();
  //while(1) sound(sndout, rand()/20, 10);
  //int i=0; while(1) sound(sndout, i++, 10);
  //float i=0; while(1) sound(sndout, sin(i+=0.01)*600, 10);
  //float i=0; while(1) { sound(sndout, sin(i+=0.01)*550, 10); sound(sndout, cos(i)*400, 10); }
  //sound(sndout, 440, 30000);

  int octave, duration;
  //You can use lot of more old siemens/ericson monotone ringtones found on net, google or http://www.gsmarena.com/ringlist.php3?idMaker=3
  char melody[]="CCDEECCFFAAGGE-CCDEGGEFEDDCC-CCDEECCFFAAGGE-CCDEGGEFEDDC"; octave=1; duration=300; //Bob Marley's Redemption song - intro ;D
  //char melody[]="GGGGEEEEEEEEEE" "EEEEEEEEGGCCCC" "DEEEEEEEE----FFFF" "FFFFFFFEEEE" "EEEEDDDD" "EEDDDDGGGGEEEE" "EEEEEEEEEEEEEE" "GGCCCCDEEEEEEEE----" "FFFFFFFFFFF" "EEEEEEGG" "GGFFDDCCCC"; octave=2; duration=100; //Jingle Bells
  //char melody[]="D-F-GG-D-F-GGG--D-F-GG-F-DD"; octave=2; duration=180; //Depp Purple - Smoke on the water //alternative: "cpdpfppcpepffppcpdpfppdpc"
  //char melody[]="CC-E-F-AGG-E-C-AFFFGG--FFFGAPCCCC"; octave=3; duration=180; //The Simpsons theme
  //char melody[]="CFGG-GG-GG-GC-------CFGGG-GG-GG-GD"; octave=2; duration=300; //X-Files theme
  //char melody[]="CDCEE--EE-EE----EEDEGG--EDECC"; octave=2; duration=300; //Beatles - Hard days night
  //char melody[]="HHHH--AAGGAAHHAAGGAAGG--GG-----HHHH--GG--AAHHGGAA--"; octave=2; duration=300; //Offspring - Why don't you get a job
  //char melody[]="AA-AEEGGAA-ADDBBAA-AEEGGAA-ADDBB"; octave=2; duration=300; //Off Spring - Pretty Flyv
  //char melody[]="AAgacfaAgadDdcAgacfaAgadDdc AgaaffGv"; octave=2; duration=200; //Eiffel 65 - Blue
  //char melody[]="HHHHHHAAAAAAGGAAA-HHHHHH" "AAAAAAFFGGEE-HHHHHHAAAAAA" "AAAAAA-HHHHHHAAAAAAFFGGEE"; octave=2; duration=300; //Bob Dylan - Knockin On Heavens Door
  //char melody[]="bBbBpFFpbepdpcpbBbBpFpBepdpcpbBbBp FpbepdpbepCC"; octave=1; duration=200; //Star Wars
  //char melody[]="f p c p d p e p f p c p d p e p f p c p d p e p F p p g c e g e G p g c f g f GG"; octave=2; duration=100; //NBA
  
  play_melody(sndout, melody, octave, duration);
  delay(2000);
}

