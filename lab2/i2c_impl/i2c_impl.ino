/* File: I2C_impl.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file provides a basic implementation for transmitting
 * and receiving data on the I2C bus.
 * Functionality: 
 * -Read one or more bytes of data from a specified register on another device
 */
 #define SLAVE_ADDRESS 0x28 //default address of the BNO055
 #define READ_BIT B00000001 //write bit is 0
 #define START 0x08 //the code signifying the START bit was sent
uint8_t I2C_read(uint8_t reg_address);
uint8_t I2C_read_len(uint8_t reg_address, uint8_t* buf, uint8_t len);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
uint8_t I2C_read(uint8_t reg_addres) {
    /* Set bits TWINT (7), TWSTA(5), and TWEN(2) set to 1
     * TWINT: Interrupt flag, which generates an interrupt 
     * TWSTA: Flag that results in the start condition being written
     * TWEN: Enables I2C interface.
     */
    TWCR = B10100100;
    while(!(TWCR & (1<<TWINT))) {}
    if((TWSR & 0xF8)!=START){} //check TWSR to see if the start bit was transmitted
    TWDR = SLAVE_ADDRESS | READ_BIT; 
    
}
