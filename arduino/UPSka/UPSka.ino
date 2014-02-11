int resetPin =  8;


void setup() {
  Serial.begin(9600);
  Serial.println("\n");
  
  //Resetator test
  
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  delay(100);
  digitalWrite(resetPin, LOW); 
  
}
 
void loop() {
 
  Serial.print("{");
    Serial.print("\"voltage\":\"");
        Serial.print(map(analogRead(A7),0,1023,0,5000)/100.0);
        //Serial.print(analogRead(A0));
    Serial.print("\"");
  Serial.print("}\n");
        
  delay(500);
}
