/* File: uart_software.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 */
 
#include "timer.h"
#include "gpio.h"

void uart_software_init();
inline void uart_software_tx_byte(uint8_t data);
inline uint8_t uart_software_rx_byte();

volatile uint16_t tx_data_buf;
volatile uint8_t tx_buf_counter;
ISR(TIMER1_COMPA_vect) {
    if(tx_buf_counter != 0){
        gpio_set_out(B00000001, tx_data_buf|B00000001);
        tx_data_buf >> 1;
        tx_buf_counter -= 1;
    }
}

void setup() {
    
}

void loop() {
    
}

void uart_software_init(){
    gpio_set_mode(B00000001, 1);
    gpio_set_mode(B00000010, 0);
    gpio_set_out(B00000001, 1);
    timer_init();
    timer_set(1000/9600);
}

inline void uart_software_tx_byte(uint8_t data){
    if (tx_buf_counter != 0) {
        return; // Error, buffer is not empty, should never happen
    }
    
    tx_data_buf = (1<<9) + (data<<1);
    tx_buf_counter = 10;
    timer_reset();

    while (tx_buf_counter != 0);
}

inline uint8_t uart_software_rx_byte(){
    
}

