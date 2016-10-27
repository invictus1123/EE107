#include <SoftwareSerial.h>

/* Functions from uart_hardware to test uart_software */
extern void uart_hardware_init();
extern inline void uart_hardware_tx_byte(uint8_t data);
extern inline uint8_t uart_hardware_rx_byte();

void uart_software_init();
inline void uart_software_tx_byte(uint8_t data);
inline uint8_t uart_software_rx_byte();

/* Declare SoftwareSerial instance*/
SoftwareSerial soft_serial(8, 9); // RX, TX

void setup() {
    Serial.begin(9600);
    uart_software_init();
    uart_hardware_init();
}

uint8_t temp_rx;
void loop() {
    uart_software_tx_byte(B10101010);
    delay(10);
    
    temp_rx = uart_software_rx_byte();
    Serial.println(temp_rx);
    delay(10);
    while(soft_serial.available()){
        soft_serial.read();
    }
    delay(100);
}

void uart_software_init(){
    soft_serial.begin(9600); // Begin software serial at 9600 baud
}

inline void uart_software_tx_byte(uint8_t data){
    soft_serial.write(data); // Write 1 byte to serial TX
}

inline uint8_t uart_software_rx_byte(){
    while (!soft_serial.available()); // Wait until there is data to read
    return soft_serial.read();        // Read 1 byte from serial RX and return
}

