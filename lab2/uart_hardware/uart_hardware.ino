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
 */

 void uart_hardware_init();
 inline void uart_hardware_tx_byte(uint8_t data);
 inline uint8_t uart_hardware_rx_byte();
 #define FOSC 8000000 //provides 0 error with 9600 baud
 #define BAUD_RATE 9600
 #define UBRR ( FOSC/(16*(BAUD_RATE)) )-1
void setup() {
  uart_hardware_init();
  pinMode(13,OUTPUT); 
}

void loop() {
  uart_hardware_tx_byte('a'); //transmit the character 'a' to the serial monitor
  uint8_t ch = uart_hardware_rx_byte(); //read a character (test: send 'a')
  uart_hardware_tx_byte(ch); //transmit back the character read to compare
}
void uart_hardware_init() {
    UBRR0H = (uint8_t) (UBRR>>8); //write the 4 most significant bits of the baud rate to the high bit register
    UBRR0L = (uint8_t) UBRR;      //write the 8 least significant bits of the baud rate to the low bit register
    UCSR0C = B00000011; //set character size to 8 (011 on bits 2:0), and turn off parity check (0 on bits 5,4).   
    UCSR0B = B00011000; //(1<<RXEN0) | (1<<TXEN0) //turn on interrupt generation at the baud rate for tx and rx (bits 4 and 3 set to 1).
}

inline void uart_hardware_tx_byte(uint8_t data) {
    while( !(UCSR0A & (1<<UDRE0)) ); //if the expression is true, UDRE0 is 0, which means the transmission buffer is not empty    
    //wait until UDRE0 becomes 1 (transmission buffer becomes empty)
    
    UDR0 = data; //load the data into the transmission buffer to be transmitted
}

inline uint8_t uart_hardware_rx_byte() {
    while( !(UCSR0A & (1<<RXC0)) );//while the whole byte has not been received
    //wait for the whole byte to be received into the register
    return UDR0;
}
