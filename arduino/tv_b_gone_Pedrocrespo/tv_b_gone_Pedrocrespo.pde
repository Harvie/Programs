/* Arduino TV-B-Gone
*
* A recreation of the TV-B-Gone kit on an Arduino Duemilanove.
* By Daedalus12, mtbf0, westfw and Pedrocrespo
*
*/

#include <avr/pgmspace.h>

struct codeElement {
  uint32_t onTime;   // duration of "On" time
  uint32_t offTime;  // duration of "Off" time
};

struct powercode {
  uint32_t freq; // frequency
  struct codeElement codes[100];  // on/off codes, supposed to be 100 as maximum
};
  
    int  i = 0; // we use this for iterating
    long cfreq; // we'll store each frequency here when needed
  
const struct powercode sonyCode PROGMEM = {   
  37470, 
  {{245, 60},
   {123, 60},
   {61 , 60},
   {123, 60},
   {61 , 60},
   {123, 60},
   {61 , 60},
   {61 , 60},
   {123, 60},
   {61 , 60},
   {61 , 60},
   {61 , 60},
   {61 , 2759},
   {245, 60},
   {123, 60},
   {61 , 60},
   {123, 60},
   {61 , 60},
   {123, 60},
   {61 , 60},
   {61 , 60},
   {123, 60},
   {61 , 60},
   {61 , 60},
   {61 , 60},
   {61 , 0}}
};
const struct powercode panasonicCode PROGMEM = {   
   36130, 
   {{358,179},
   {44, 45},
   {44, 135},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 135},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 135},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 135},
   {44, 45},
   {44, 135},
   {44, 135},
   {44, 135},
   {44, 135},
   {44, 45},
   {44, 45},
   {44, 135},
   {44, 45},
   {44, 135},
   {44, 135},
   {44, 135},
   {44, 135},
   {44, 45},
   {44, 135},
   {44, 7720},
   {358, 180},
   {44, 45},
   {44, 135},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 135},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 135},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 45},
   {44, 135},
   {44, 45},
   {44, 135},
   {44, 135},
   {44 ,135},
   {44 ,135},
   {44, 45},
   {44, 45},
   {44, 135},
   {44, 45},
   {44, 135},
   {44, 135},
   {44, 135},
   {44, 135},
   {44, 45},
   {44, 135},
   {44,  0}}
};

const struct powercode sony2Code PROGMEM = {

  37470,

  { {245,   60},

    {123,   60},

    {123,   60},

    {123,   60},

    {123,   60},

    {61,   60},

    {123,   60},

    {61,   60},

    {123,   60},

    {61,   60},

    {61,   60},

    {61,   60},

    {61,   2636},

    {246,   60},

    {123,   60},

    {123,   60},

    {123,   60},

    {123,   60},

    {61,   60},

    {123,   60},

    {61,   60},

    {123,   60},

    {61,   60},

    {61,   60},

    {61,   60},

    {61,   0}  }

};



const struct powercode sony3Code PROGMEM = {

  74940,

  { {250,     63},

    {121,     63},

    { 60,     63},

    {121,     63},

    { 60,     63},

    {121,     63},

    { 60,     63},

    { 60,     63},

    {121,     63},

    { 60,     63},

    { 60,     63},

    { 60,     63},

    { 60,   2819},

    {250,     63},

    {121,     63},

    { 60,     63},

    {121,     63},

    { 60,     63},

    {121,     63},

    { 60,     63},

    { 60,     63},

    {121,     63},

    { 60,     63},

    { 60,     63},

    { 60,     63},

    { 60,      0}

  }

};
PROGMEM const powercode *powerCodes[] = {
  &sonyCode, &panasonicCode,  &sony2Code, &sony3Code
}; // select the codes you like

int num_codes = (sizeof(powerCodes)/sizeof(*powerCodes)); // count how many codes are in the list

void setup() {
  DDRD = _BV(DDD6);  // sets Pin 6 as output
}   

void loop(){
    for(i=0;i<num_codes;i++){ //Iterate between powercodes
       cfreq = pgm_read_word(pgm_read_word(&(powerCodes[i]))+0); // store the frequency
       unsigned char compareVal = (F_CPU/cfreq - 1)/2; // transform it to cpu cycles
       TCCR0A = 0;     // make sure timer is off before updating OCR0A
       OCR0A = compareVal; // update timer
       for (int arrayStep = 0; pgm_read_word(pgm_read_word(&(powerCodes[i]))+(8*arrayStep)+8)!=0; arrayStep++){
         transmitCode(pgm_read_word(pgm_read_word(&(powerCodes[i]))+(8*arrayStep)+4),pgm_read_word(pgm_read_word(&(powerCodes[i]))+(8*arrayStep)+8)); // transmit the code using function described below
       }
   delayMicroseconds(250);// Avoid interferring a powercode and the next one if frequencies are similar
  }
}

void transmitCode(int onTime, int offTime){
//  transmits one element of the POWER code
  TCNT0 = 0;                              // reset timer 0
  TCCR0A = _BV(COM0A0) | _BV(WGM01);      // set up timer 0
  TCCR0B = _BV(CS00);                     // turn on timer 0
  delayMicroseconds(10*onTime);           // wait for onTime
  TCNT0 = 0;                                // stop timer
  TCCR0A = 0;                               // clear register A
  TCCR0B = 0;                               // clear register B
  PORTD = ~_BV(PORTD6);                     // turn off the LED 
  delayMicroseconds(10*offTime);            // wait for offTime
}
