#include "Wire.h"
#include "Adafruit_BMP085.h"
#include <dht.h>

//#define EOL ";"

Adafruit_BMP085 bmp;
dht DHT;
#define DHT11PIN 2

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}

void setup() {
  Serial.begin(9600);
  bmp.begin();  
  
  Serial.println("\n");
}
 
void loop() {
  int chk = DHT.read11(DHT11PIN);
  if(chk != DHTLIB_OK) Serial.println("DHTLIB ERROR!");
  
  Serial.print("{");
    Serial.print("\"config\":{");
      Serial.print("\"airquality\":{\"label\":\"Air Quality\",\"unit\":\"Relative\"},");
      Serial.print("\"temp\":{\"label\":\"Temperature\",\"unit\":\"°C\"},");
      Serial.print("\"pressure\":{\"label\":\"Pressure\",\"unit\":\"Pa\"},");
      Serial.print("\"humidity\":{\"label\":\"Humidity\",\"unit\":\"% RH\"},");
      Serial.print("\"analog\":{\"label\":\"ADC\",\"unit\":\"Relative\"}");
    Serial.print("},");
    Serial.print("\"data\":{");

      Serial.print("\"airquality\":[");
        
        Serial.print("{\"label\":\"Air polution (MQ-2)\",\"value\":\"");
        Serial.print(analogRead(A1));
        Serial.print("\"}");
        
      Serial.print("],");
    

/*    
      Serial.print("\"analog\":[");
      
        Serial.print("{\"label\":\"ADC A0 (antenna?)\",\"value\":\"");
        Serial.print(analogRead(A0));
        Serial.print("\"}");
        
        Serial.print(",{\"label\":\"ADC A2 (phototransistor?)\",\"value\":\"");
        Serial.print(analogRead(A2));
        Serial.print("\"}");
        
      Serial.print("],");
*/  
  
      Serial.print("\"temp\":[");
      
        Serial.print("{\"label\":\"BMP085 Temperature\",\"value\":\"");
        Serial.print(bmp.readTemperature());
        Serial.print("\"}");
        
        Serial.print(",{\"label\":\"DHT11 Temperature\",\"value\":\"");
        Serial.print((float)DHT.temperature);
        Serial.print("\"}");

      Serial.print("],");
  
  
      Serial.print("\"pressure\":[");
      
        Serial.print("{\"label\":\"BMP085 Pressure\",\"value\":\"");
        Serial.print(bmp.readPressure());
        Serial.print("\"}");
    
      Serial.print("],");
      
      
      Serial.print("\"humidity\":[");
      
        Serial.print("{\"label\":\"DHT11 Humidity\",\"value\":\"");
        Serial.print((float)DHT.humidity);
        Serial.print("\"}");
    
      Serial.print("]");
 
    Serial.print("}");
  Serial.print("}\n");
  delay(2000);
}
