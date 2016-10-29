/* File: uart_hardware.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file implements the driver
 * for the hardware serial port.
 * Functionality:
 * -Set up the Serial Port at 
 * 9600 baud, 8 data bits, one stop bit
 * and no parity bit.
 * -Transmit bytes of data over the bus
 * -Test implementation by typing 'a' to make LED light
 * -Test implementation by typing characters to print
 */

 void uart_hardware_init();
 inline void uart_hardware_tx_byte(uint8_t data);
 inline uint8_t uart_hardware_rx_byte();
 #define FOSC 16000000 //This appears to be the stock setting for the atmega328. Not actually equal to our IO clock rate.
 #define BAUD_RATE 9600
 #define UBRR FOSC/16/BAUD_RATE -1
void setup() {
  uart_hardware_init();
  pinMode(13,OUTPUT); 
}

void loop() {

  unsigned char ch = uart_hardware_rx_byte();
  uart_hardware_tx_byte(ch);
  if(ch =='a') {
      digitalWrite(13,HIGH);
  }
  delay(100);
  digitalWrite(13,LOW);
}

void uart_hardware_init() {
//    UBRR0H = (uint8_t) (UBRR>>8); //write the 4 most significant bits of the baud rate to the high bit register
//    UBRR0L = (uint8_t) UBRR;      //write the 8 least significant bits of the baud rate to the low bit register
    UBRR0 = UBRR;
    UCSR0C = B00000110; //set character size to 8 (011 on bits 3:1), and turn off parity check (0 on bits 5,4).   
    UCSR0B = B00011000; //(1<<RXEN0) | (1<<TXEN0) //turn on interrupt generation at the baud rate for tx and rx (bits 4 and 3 set to 1).
}

inline void uart_hardware_tx_byte(uint8_t data) {
    while( !(UCSR0A & (1<<UDRE0)) ); //if the expression is true, UDRE0 is 0, which means the transmission buffer is not empty    
    //wait until UDRE0 becomes 1 (transmission buffer becomes empty)
    UDR0 = data; //load the data into the transmission buffer to be transmitted
    while( !(UCSR0A & (1<<UDRE0)) ); //wait for the byte to be transmitted before returning
}

inline uint8_t uart_hardware_rx_byte() {
    while( !(UCSR0A & (1<<RXC0)) );//while the whole byte has not been received
    return UDR0;
}
