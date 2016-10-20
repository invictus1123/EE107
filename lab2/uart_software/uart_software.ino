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

volatile uint16_t rx_data_buf;
volatile uint8_t rx_buf_counter;

/* Timer interrupt for uart tx and rx*/
ISR(TIMER1_COMPA_vect) {
    /* Uart tx */
    if (tx_buf_counter != 0){
        gpio_set_out(B00000001, tx_data_buf|B00000001); // Set GPIO to LSB of data buffer
        tx_data_buf >> 1; // Shift out LSB of data buffer
        tx_buf_counter -= 1; // Decrement counter
    }

    /* Uart rx */
    if (rx_buf_counter != 0){
        // TODO
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
    /* Check if counter is 0 (TODO: some error recovery?)*/
    if (tx_buf_counter != 0) {
        return; // Error, buffer is not empty, should never happen
    }

    /* Setup timer interrupt to perform uart tx*/
    tx_data_buf = (1<<9) + (data<<1); // Load data with start and end bit to tx buffer
    tx_buf_counter = 10; // Reset counter to 10 (10 bits to transfer)
    timer_reset();       // Reset timer

    /* Wait until tx_data is sent before ending function */
    while (tx_buf_counter != 0);
}

inline uint8_t uart_software_rx_byte(){
    /* Setup timer interrupt to perform uart rx*/
    rx_buf_counter = 10;
    timer_reset();

    /* Wait until rx data is receieved completely before reading */
    while (rx_buf_counter != 0);

    /* Output rx data*/
    return (uint8_t) (rx_data_buf>>1);
}

