//  *****************************************************************************************************************
//  *                                                                                                               *
//  *                                         SpikenzieLabs.com                                                     *
//  *                                                                                                               *
//  *                                   Very Simple Serial to MIDI DEMO                                             *
//  *                                                                                                               *
//  *****************************************************************************************************************
//
// BY: MARK DEMERS 
// May 2009
// VERSION: 0.1
//
// DESCRIPTION:
// Demo sketch to play notes from middle C in the 4th octave up to B in the 5th octave and then back down.
//
//
// HOOK-UP:
// 1. Plug USB cable from Arduino into your computer.
//  
//
// USAGE:
// 1. Install and Set-up Serial MIDI Converter from SpikenzieLabs
// 2. Open, compile, and upload this sketch into your Arduino.
// 3. Run Serial MIDI Converter in the background.
// 4. Launch your music software such as Garage Band or Ableton Live, choose a software instrument and listen to the music.
//
//
// LEGAL:
// This code is provided as is. No guaranties or warranties are given in any form. It is up to you to determine
// this codes suitability for your application.
//

int note = 0;     

void setup() 
{
  Serial.begin(57600);                                       // Default speed of the Serial to MIDI Converter serial port
}

void loop() 
{

  for(int note=60; note<=83; note++)                        // Going Up
  {
    MIDI_TX(144,note,127);                                  // NOTE ON
    delay(100);

    MIDI_TX(128,note,127);                                  // NOTE OFF
    delay(100);
  }

  for(int note=82; note>=61; note--)                       // Coming Down
  {
    MIDI_TX(144,note,127);                                  // NOTE ON
    delay(250);

    MIDI_TX(128,note,127);                                  // NOTE OFF
    delay(250);
  }

}


void MIDI_TX(unsigned char MESSAGE, unsigned char PITCH, unsigned char VELOCITY) 
{
  Serial.print(MESSAGE);
  Serial.print(PITCH);
  Serial.print(VELOCITY);
}


