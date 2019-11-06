/*
 * Exit Rum
 * (c) Tomas 'Harvie' Mudrunka 2016
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TM1637Display.h>

#define TIMEOUT 300
#define BEEP_FREQ 3000
#define BEEP_LEN 50

#define P_NASLAP 12
#define P_PIEZO 35
#define P_RELAY_1 9
#define P_RELAY_2 8
#define P_LEDSEG_CLK 2
#define P_LEDSEG_DIO 3
#define P_LEDSTRIP 11

TM1637Display display(P_LEDSEG_CLK, P_LEDSEG_DIO);
LiquidCrystal_I2C lcd(0x3F, 16, 2); //i2c addr, chars, lines

#define JBASE 38
#define JCOUNT 16

#define J_A 0
#define J_B 2
#define J_C 4
#define J_D 6
#define J_E 8
#define J_F 10
#define J_G 12
#define J_H 14
#define J_1 1
#define J_2 3
#define J_3 5
#define J_4 7
#define J_5 9
#define J_6 11
#define J_7 13
#define J_8 15

boolean jumpers_read[JCOUNT][JCOUNT] = {0}, jumpers_secret[JCOUNT][JCOUNT] = {0};

void jumper_store(int a, int b, boolean c) {
  jumpers_secret[min(a,b)][max(a,b)]=c;
}

void sound(char sndpin, float freq, float duration) { //Play bit-bang sound
  if(duration<=0) return; if(freq<=0) { delay(duration); return; }
  freq=((1000000/2)/freq); //Convert freq to delay (us)
  duration*=1000; //Convert duration to us
  pinMode(sndpin, OUTPUT);
  for(;duration>0;duration-=2*freq) {
    digitalWrite(sndpin, HIGH); delayMicroseconds(freq);
    digitalWrite(sndpin, LOW);  delayMicroseconds(freq);
  }
  pinMode(sndpin, INPUT); //Close pin to avoid noise (optional)
}

int check_jumpers() {
  int i, j, check = 0, fail = 0;
  
  for(i=0;i<JCOUNT;i++) {
    pinMode(JBASE+i, OUTPUT);
    digitalWrite(JBASE+i, LOW);
    for(j=0;j<JCOUNT;j++) if(i<j) {
      pinMode(JBASE+j, INPUT_PULLUP);
      jumpers_read[i][j] = !digitalRead(JBASE+j);
      if(jumpers_read[i][j]) {
        check++;
        Serial.print(JBASE+i, DEC);
        Serial.print("->");
        Serial.print(JBASE+j, DEC);
        if(jumpers_secret[i][j]) {
          Serial.print("!");
        } else {
          fail++;
        }
        Serial.print(", ");
      }
    }
    pinMode(JBASE+i, INPUT_PULLUP);
  }
  Serial.println();
  check = fail ? -check : check;
  Serial.println(check, DEC);
  return check;
}

void setup() {
  jumper_store(J_A, J_5, true);
  jumper_store(J_B, J_4, true);
  jumper_store(J_C, J_7, true);
  jumper_store(J_D, J_2, true);
  jumper_store(J_E, J_6, true);
  jumper_store(J_F, J_8, true);
  jumper_store(J_G, J_1, true);
  jumper_store(J_H, J_3, true);

  pinMode(P_NASLAP, INPUT_PULLUP);
  pinMode(P_RELAY_1, OUTPUT);
  pinMode(P_RELAY_2, OUTPUT);
  pinMode(P_LEDSTRIP, OUTPUT);

  Serial.begin(9600);
  lcd.begin();
}

#define seconds_display(seconds) ((((seconds)/60)*100)+((seconds)%60))

// the loop function runs over and over again forever
void loop() {
  Serial.println("Inicializace systemu...");

  lcd.backlight();
  lcd.clear();
  lcd.print(" (c) SPOJE.NET");
  lcd.setCursor(0,1);
  lcd.print("Tomas@MudrunkaCZ");

  display.setBrightness(0x0f);
  display.setColon(true);
  display.showNumberDec(2323);

  delay(5000);

  lcd.noDisplay();
  lcd.noBacklight();
  display.setBrightness(0x00);

  display.setBrightness(0);
  display.showNumberDec(0);

  analogWrite(P_LEDSTRIP, 0);
  digitalWrite(P_RELAY_1, HIGH);
  digitalWrite(P_RELAY_2, HIGH);

  sound(P_PIEZO, BEEP_FREQ, BEEP_LEN);
  delay(BEEP_LEN);
  sound(P_PIEZO, BEEP_FREQ, BEEP_LEN);
  Serial.println("Cekam na aktivaci...");
  while(digitalRead(P_NASLAP));

  Serial.println("Sekvence aktivovana!");

  display.setBrightness(0x0f);
  for(int i=0;i<70;i++) {
    sound(P_PIEZO, random(500,10000), random(10,100));
    display.showNumberDec(random(1000,9999));
    analogWrite(P_LEDSTRIP, random(0,255));
  }
  sound(P_PIEZO, BEEP_FREQ, 2000);

  Serial.println("Odpocet aktivovan!");
  int timer = TIMEOUT;
  while(check_jumpers() != 8) {
    Serial.print("Zbyva: ");
    Serial.print(timer, DEC);
    Serial.println(" sekund");
    if(timer <= 0) {
      digitalWrite(P_RELAY_1, LOW);
    }
    display.setColon(timer%2);
    display.showNumberDec(seconds_display(timer));
    analogWrite(P_LEDSTRIP, map(timer%5, 0, 4, 255, 10));
    digitalWrite(P_RELAY_2, timer%2);
    if(timer>0) {
      timer--;
      sound(P_PIEZO, BEEP_FREQ, BEEP_LEN);
      delay(1000-BEEP_LEN);
    } else {
      analogWrite(P_LEDSTRIP, random(0,255));
      sound(P_PIEZO, random(500,10000), random(100,1000));
    }
  }

  Serial.println("Odpocet zastaven.");
  display.setColon(true);
  display.showNumberDec(seconds_display(timer));
  analogWrite(P_LEDSTRIP, 0);
  digitalWrite(P_RELAY_1, HIGH);
      
  //Muhammad Ismáil Árifi
  lcd.clear();
  lcd.print("  Muhammad");
  lcd.setCursor(0,1);
  lcd.print("  Ismail Arifi");
  lcd.backlight();
  lcd.display();

  Serial.println("Cekam na reset...");
  //while(digitalRead(P_NASLAP));
  while(check_jumpers() != 0) delay(1000);
}
