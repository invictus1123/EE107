/* File: uart_software.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 */
#define BAUD 9600
#define PERIOD_MS 1000/BAUD

#include "timer.h"
#include "gpio.h"

void uart_software_init();
inline void uart_software_tx_byte(uint8_t data);
inline uint8_t uart_software_rx_byte();

volatile uint16_t tx_data_buf;
volatile uint8_t tx_buf_counter;

volatile uint16_t rx_data_buf;
volatile uint8_t rx_buf_counter;

volatile int counter = 0;

extern void uart_hardware_init();
extern inline void uart_hardware_tx_byte(uint8_t data);
extern inline uint8_t uart_hardware_rx_byte();

/* Timer interrupt for uart tx and rx*/
ISR(TIMER1_COMPA_vect) {
    /* Uart tx */
    if (tx_buf_counter != 0){
        gpio_set_out(B00000001, tx_data_buf&B1); // Set tx GPIO to LSB of data buffer
        tx_data_buf = tx_data_buf >> 1; // Shift out LSB of data buffer
        tx_buf_counter -= 1; // Decrement counter
    }

    /* Uart rx */
    if (rx_buf_counter != 0){
        rx_data_buf |= (gpio_read(2) << 10 - rx_buf_counter); // Read 1 bit from rx GPIO
        tx_buf_counter -= 1;
    }
}

void setup() {
    Serial.begin(9600);
    uart_software_init();
    uart_hardware_init();
}

uint8_t temp_rx;
void loop() {
    temp_rx = uart_software_rx_byte();
    Serial.println(temp_rx);
    delay(10);
}

void uart_software_init(){
    gpio_set_mode(B00000001, 1);
    gpio_set_mode(B00000010, 0);
    gpio_set_out(B00000001, 1);
    timer_init();
    timer_set(PERIOD_MS);
}

inline void uart_software_tx_byte(uint8_t data){
    /* Check if counter is 0 (TODO: some error recovery?)*/
    if (tx_buf_counter != 0) {
        return; // Error, buffer is not empty, should never happen
    }

    /* Setup timer interrupt to perform uart tx*/
    tx_data_buf = (1<<9) + (data<<1); // Load data with start and end bit to tx buffer
    timer_reset();       // Reset timer    
    tx_buf_counter = 10; // Reset counter to 10 (10 bits to transfer)

    /* Wait until tx_data is sent before ending function */
    while (tx_buf_counter != 0);
}

inline uint8_t uart_software_rx_byte(){
    /* Setup timer interrupt to perform uart rx*/
    while(gpio_read(2) == 1); // Wait until start bit is detected on rx
    timer_reset();            // Reset timer
    rx_buf_counter = 10;      // Reset counter to 10 (10 bits to read)

    /* Wait until rx data is receieved completely before reading */
    while (rx_buf_counter != 0);

    /* Output rx data*/
    return (uint8_t) (rx_data_buf>>1);
}

