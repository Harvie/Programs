/*
 * MIDIUSB_buzzer.ino
 *
 * Author: Paulo Costa
 */ 

#include <MozziGuts.h>
#include <MIDIUSB.h>
#include <pitchToFrequency.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <mozzi_midi.h>
#include <ADSR.h>

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

ADSR <CONTROL_RATE, AUDIO_RATE> envelope;

const char* pitch_name(byte pitch) {
  static const char* names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  return names[pitch % 12];
}

int pitch_octave(byte pitch) {
  return (pitch / 12) - 1;
}

void noteOn(byte channel, byte pitch, byte velocity) {
  aSin.setFreq(pitchFrequency[pitch]); 
  envelope.noteOn();

  Serial.print("Note On: ");
  Serial.print(pitch_name(pitch));
  Serial.print(pitch_octave(pitch));
  Serial.print(", channel=");
  Serial.print(channel);
  Serial.print(", velocity=");
  Serial.println(velocity);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  //aSin.setFreq(0);
  envelope.noteOff();

  Serial.print("Note Off: ");
  Serial.print(pitch_name(pitch));
  Serial.print(pitch_octave(pitch));
  Serial.print(", channel=");
  Serial.print(channel);
  Serial.print(", velocity=");
  Serial.println(velocity);
}

void controlChange(byte channel, byte control, byte value) {
  Serial.print("Control change: control=");
  Serial.print(control);
  Serial.print(", value=");
  Serial.print(value);
  Serial.print(", channel=");
  Serial.println(channel);
}

void setup() {
  Serial.begin(115200);
    envelope.setADLevels(255,48);
  envelope.setTimes(150,200,700,200); // 700 is so the note will sustain 0.7 seconds unless a noteOff comes

  aSin.setFreq(440); // default frequency
   startMozzi(CONTROL_RATE);
}
void updateControl(){
  envelope.update();
}


AudioOutput_t updateAudio(){
  return MonoOutput::from16Bit(envelope.next() * aSin.next()); // return an int signal centred around 0
}

void loop() {
  audioHook(); 
  midiEventPacket_t rx = MidiUSB.read();
  switch (rx.header) {
    case 0:
      break; //No pending events
      
    case 0x9:
      noteOn(
        rx.byte1 & 0xF,  //channel
        rx.byte2,        //pitch
        rx.byte3         //velocity
      );
      break;
      
    case 0x8:
      noteOff(
        rx.byte1 & 0xF,  //channel
        rx.byte2,        //pitch
        rx.byte3         //velocity
      );
      break;
      
    case 0xB:
      controlChange(
        rx.byte1 & 0xF,  //channel
        rx.byte2,        //control
        rx.byte3         //value
      );
      break;
      
    default:
      Serial.print("Unhandled MIDI message: ");
      Serial.print(rx.header, HEX);
      Serial.print("-");
      Serial.print(rx.byte1, HEX);
      Serial.print("-");
      Serial.print(rx.byte2, HEX);
      Serial.print("-");
      Serial.println(rx.byte3, HEX);
  }
}
