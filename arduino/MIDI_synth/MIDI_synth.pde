/*
 * Simple monophonic MIDI synthesizer :-)
 */

#define MIDI_NOTE_OFF 128
#define MIDI_NOTE_ON  144
#define MIDI_CONTROL  176
#define MIDI_PITCH_BEND 224

#define sndout 13

double base_a4=440; //set A4=440Hz
double note_to_freq(double n) {
  if( n>=0 && n<=119 ) {
    return base_a4*pow(2,(n-57)/12);
  } else {
     return -1;
  }
}


void setup() {
  Serial.begin(19200);
  pinMode(sndout, OUTPUT);
}

void loop() {
  int dela=0;
  
  while(1) {
    
  if(Serial.available() >= 3) {
    double command=0, channel=0, pitch=0;
    command = Serial.read();
    channel = Serial.read();
    pitch = Serial.read();
    if(command == MIDI_NOTE_ON && pitch > 0) dela=((1000000/2)/note_to_freq(pitch));
    if(command == MIDI_NOTE_OFF || pitch == 0) dela = 0;
    
    Serial.println(note_to_freq(pitch), DEC);
  }
  
  if(dela > 0) digitalWrite(sndout, HIGH); delayMicroseconds(dela);
  digitalWrite(sndout, LOW);  delayMicroseconds(dela);
  
  }
 
}
