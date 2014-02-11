#define F_CPU 100000000UL // Sets up the default speed for delay.h

#include <avr/io.h>
#include <util/delay.h>

//#include <compat/deprecated.h>
/*
variable = PINx;  // Read a byte from a port, replace x with the port number
PORTx = variable; //Write a byte to a port
PORTx |= 1<<pin_number;  // Set a pin in a port
PORTx &= ~(1<<pin_number); // Clear a pin in a port
*/

void delay(unsigned int ms) {
  unsigned int i,j;
  for ( j=0; j<1000; j++)
    for ( i=0; i<ms; i++);
}

int main (void){

  //sbi(DDRD,PD5); /*enable port 5 for output*/
	DDRD |= 1<<5;

  while (1) {
		//cbi (PORTD, PD5);
		PORTD &= ~(1<<5);
		delay (500);
		//sbi (PORTD, PD5);
		PORTD |= 1<<5;
		delay (500);
  }

  return 0;
}
