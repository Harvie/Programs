#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);

int i;

void setup()
{
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	lcd.print("Rychta pyco!");

    display.setBrightness(0x0f);
  display.setColon(true);
  display.showNumberDec(23);  

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

}

void loop()
{
  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  display.setColon(true);
  display.showNumberDec(i);
  delay(1000);
  digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  display.setColon(false);
  display.showNumberDec(i++);
  delay(1000);
}
