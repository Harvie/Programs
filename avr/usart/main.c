#include <avr/io.h>
#define F_CPU 1000000UL //1MHz default internal RC oscillator setting
//#include <avr/iotn2313.h>
//#include <avr/interrupt.h>
//util/delay.h and util/setbaud.h

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define LED PD5

int main (void)
{
  DDRD |= (1 << LED);

   char ReceivedByte;

   UCSRB |= (1 << RXEN) | (1 << TXEN);   // Turn on the transmission and reception circuitry
   //UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes (not in attiny2313?)
   UCSRC |= (1 << UCSZ0) | (1 << UCSZ1);
	//Hint: stty -F /dev/ttyUSB1 9600 sc7

   UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
   UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register

   for (;;) // Loop forever
   {
      while ((UCSRA & (1 << RXC)) == 0) {}; // Do nothing until data have been received and is ready to be read from UDR
      ReceivedByte = UDR; // Fetch the received byte value into the variable "ByteReceived"

		  PORTD ^= (1 << LED); //Toggle LED after each received byte

      while ((UCSRA & (1 << UDRE)) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
      UDR = ReceivedByte; // Echo back the received byte back to the computer
   }
}

