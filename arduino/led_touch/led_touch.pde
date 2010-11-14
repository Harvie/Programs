//TOUCH SENSING BETA - LED TURNS ON WHEN LIGHT IS PRESENT
//BY: RICARDO DE LEMOS 1/17/2007
int led1 = 13;
int cath = 2; // negative
int ando = 3;  // positive

void setup()
 {
   pinMode(led1, OUTPUT);
   pinMode(cath, OUTPUT);
   pinMode(ando, OUTPUT);
   //Serial.begin(9600);
 }
 
int dela = 20;
int last = HIGH;
int shine = HIGH;

void loop()
 {
   //TURN SENSOR LED ON
   pinMode(ando, OUTPUT);
   if(shine) {
     digitalWrite(cath, LOW);
     digitalWrite(ando, HIGH);
     delay(100);
   }
   
   //REVERSE BIAS + CHARGE LED
   digitalWrite(cath, HIGH);
   digitalWrite(ando, LOW);
   //delay(dela);
   
   //READ LED CAP
   pinMode(ando, INPUT);
   delay(dela);
   int state = digitalRead(ando);
   
   //SWITCH
   if((state != last) && state == LOW) shine = !shine;
   last = state;
   
   //DEBUG LED
   if(state) {
     digitalWrite(led1,HIGH);
   } else {
     digitalWrite(led1,LOW);
   }
 }

 
