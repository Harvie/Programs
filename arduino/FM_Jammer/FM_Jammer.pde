/*
 * Blink
 *
 * The basic Arduino example.  Turns on an LED on for one second,
 * then off for one second, and so on...  We use pin 13 because,
 * depending on your Arduino board, it has either a built-in LED
 * or a built-in resistor so that you need only an LED.
 *
 * http://www.arduino.cc/en/Tutorial/Blink
 */

int Pin = 13;                // LED connected to digital pin 13
int

void setup()                    // run once, when the sketch starts
{
  pinMode(Pin, OUTPUT);      // sets the digital pin as output
}

void loop()                     // run over and over again
{
    delay(1);
    digitalWrite(Pin, HIGH);   // sets the LED onv
    digitalWrite(Pin, LOW);    // sets the LED offv
}
