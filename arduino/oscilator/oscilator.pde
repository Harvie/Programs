/* Pong controller (for use with hPong)
 * <~~Harvie 2oo8
 */
 
#define inpin 0
int min = 65535, max = 0;
int stadium_max = 70;
char offset = 0;
float val = 0, oldval = -255;
int sndPin = 12;

void setup()                    // run once, when the sketch starts
{
  stadium_max+=offset;
  Serial.begin(19200);
  pinMode(sndPin, OUTPUT);
}

void loop()                     // run over and over again
{
  val = analogRead(inpin);
  if(val<min) min=val;
  if(val>max) max=val;
  val=(((val-(min))/(max-(min)))*stadium_max)-offset;
  /*if(val!=oldval) {
    oldval = val;
    //Serial.print(min, DEC); Serial.print("-"); Serial.print(max, DEC); Serial.print("\n");
    //Serial.print(val, DEC); Serial.print("\n");
    //Serial.print(val, BYTE);
  }*/
  val=val;
  digitalWrite(sndPin, HIGH);
  delay(val);
  digitalWrite(sndPin, LOW);
  delay(val);
}
