/* File: uart_software.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file implements the software serial port
 * using the Arduino SoftwareSerial library.
 * Functionality:
 * -Set up the serial port at 
 * 9600 baud, 8 data bits, one stop bit
 * and no parity bit.
 * -Transmit bytes of data over pins 8 (RX) and 9 (TX)
 * Verification:
 * TX: Sent data, and verified contents using DA
 * RX: Sent data to software serial using hardware serial.
 *     Lit LED when correct data was receieved.
 */
#include <SoftwareSerial.h>

/* Functions from uart_hardware.ino to test uart_software */
extern void uart_hardware_init();
extern inline void uart_hardware_tx_byte(uint8_t data);
extern inline uint8_t uart_hardware_rx_byte();

/* Functions from led.ino to test uart_software*/ 
extern void led_init();
extern void led_on();
extern void led_off();
extern void led_toggle();

void uart_software_init();
inline void uart_software_tx_byte(uint8_t data);
inline uint8_t uart_software_rx_byte();

/* Declare SoftwareSerial instance*/
SoftwareSerial soft_serial(8, 9); // RX, TX

void setup() {    
    uart_software_init();
    uart_hardware_init();
    led_init();
    led_off();
}

void loop() {
    uint8_t temp_rx;
  
    /*Test software uart TX by reading output bits on DA*/
//    uart_software_tx_byte(B00101011);

     /* - Test software uart RX
     * - Lights LED if byte is correctly received
     * - Requires hardware UART TX pin to be connected to
     *   pin 8 (software UART RX pin) by wire
     */
//    uart_hardware_tx_byte(B10101010);
//    temp_rx = uart_software_rx_byte();
//    if (temp_rx == B10101010){
//        led_on();
//    }
}

void uart_software_init(){
    soft_serial.begin(9600); // Begin software serial at 9600 baud
                             // This defaults to 1 start, 1 stop, no parity bits
}

inline void uart_software_tx_byte(uint8_t data){
    soft_serial.write(data); // Write 1 byte to serial TX
    soft_serial.flush();     // Flush the transmit buffer to ensure data 
                             // is transmitted before exiting function
}

inline uint8_t uart_software_rx_byte(){
    while (!soft_serial.available()); // Wait until there is data to read
    return soft_serial.read();        // Read 1 byte from serial RX and return
}

