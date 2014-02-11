#include "Wire.h"
#include "Adafruit_BMP085.h"

#define EOL ";"

Adafruit_BMP085 bmp;
 
void setup() {
  Serial.begin(9600);
  bmp.begin();  
}
 
void loop() {
    Serial.print("label[temp]='Temperature'"EOL);
    Serial.print("unit[temp]='*C'"EOL);
    Serial.print("label[pressure]='Pressure'"EOL);
    Serial.print("unit[pressure]='Pa'"EOL);
    Serial.print("label[analog]='ADC'"EOL);
    Serial.print("unit[analog]='Relative'"EOL);
    
    Serial.print("label[analog0]='ADC A0 (antenna?)'"EOL);
    Serial.print("value[analog0]=");
    Serial.print(analogRead(A0));
    Serial.print(EOL);
    
    Serial.print("label[analog1]='ADC A1 (coil?)'"EOL);
    Serial.print("value[analog1]=");
    Serial.print(analogRead(A1));
    Serial.print(EOL);
    
    Serial.print("label[analog2]='ADC A2 (phototransistor?)'"EOL);
    Serial.print("value[analog2]=");
    Serial.print(analogRead(A2));
    Serial.print(EOL);
    
    Serial.print("label[temp1]='BMP085 Temperature'"EOL);
    Serial.print("value[temp1]=");
    Serial.print(bmp.readTemperature());
    Serial.print(EOL);
 
    Serial.print("label[pressure1]='BMP085 Presure'"EOL);
    Serial.print("value[pressure1]=");
    Serial.print(bmp.readPressure());
    Serial.print(EOL);
 
    Serial.print("\n");
    delay(1000);
}
