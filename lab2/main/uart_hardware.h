#ifndef UART_HARDWARE_H
#define UART_HARDWARE_H

#include "Arduino.h"

#define FOSC 16000000 //provides 0 error with 9600 baud
#define BAUD_RATE 9600
#define UBRR FOSC/16/BAUD_RATE -1

void uart_hardware_init();
void uart_hardware_tx_byte(uint8_t data);
uint8_t uart_hardware_rx_byte();
void uart_hardware_tx_str(String str);

#endif // UART_HARDWARE_H
