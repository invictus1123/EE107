/* File: I2C_impl.cpp
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file provides a basic implementation for transmitting
 * and receiving data on the I2C bus.
 * Functionality:
 * -Read one or more bytes of data from a specified register on another device
 */
 #include "i2c.h"

/* Write to the sensor the register we want to read from. Postcondition: data sent and acknowledgement received, pending repeated start */
bool I2C_initiate_read(uint8_t reg_address) {
     /* Set bits TWINT (7), TWSTA(5), and TWEN(2) set to 1
     * TWINT: Interrupt flag, which generates an interrupt
     * TWSTA: Flag that results in the start condition being written
     * TWEN: Enables I2C interface.
     */
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); //set TWINT to clear interrupt, start condition, and enable I2C.
    while( !(TWCR & (1<<TWINT)) ) {}
    if((TWSR & 0xF8)!=START){//check TWSR to see if the start bit was transmitted (code 0x08)
        handleError();
    }
    TWDR = (SLAVE_ADDRESS<<1); //slave address + write bit (0)
    TWCR = (1<<TWINT) | (1<<TWEN); //set interrupt bit and enable bit to start transmission of slave address
    while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
    if((TWSR & 0xF8) != WRITE_ACK) { //if we got acknowedgement to write(status code 0x18), continue
        handleError();
    }
    TWDR = reg_address; //write to the sensor the register we want to read
    TWCR = (1<<TWINT) | (1<<TWEN);
    while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
    if((TWSR & 0xF8) != DATA_SENT) {//if we sent data and got acknowledgement (status code 0x28), continue
        handleError();
    }
    return true;
}

uint8_t I2C_getByte(bool moreDataLeft) {
    uint8_t data = 0;
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA); //transmit repeated start
    while( !(TWCR & (1<<TWINT)) ) {}
    if((TWSR & 0xF8)!=R_START){ //check we sent a repeated start
        handleError();
    }
    TWDR = (SLAVE_ADDRESS<<1) | READ_BIT; //slave address + read bit (1)
    TWCR = (1<<TWINT) | (1<<TWEN); //set interrupt bit and enable bit to start transmission of slave address
    while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
    if((TWSR & 0xF8) != READ_ACK) { //if we got acknowedgement to read(status code 0x40), continue
        handleError();
    }
    data = TWDR;
    while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
        if((TWSR & 0xF8) != DATA_RECEIVED) {//if we got data and sent acknowledgement (status code 0x50), continue
        handleError();
    }
    if(!moreDataLeft) {
        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); //transmit stop condition
    }
    return data;
}

uint8_t I2C_read(uint8_t reg_address) {
    uint8_t data = 0;
    if(!I2C_initiate_read(reg_address)) {
        handleError();
    }
    data = I2C_getByte(false);
    return data;
}

bool I2C_read_len(uint8_t reg_address, uint8_t *buf, uint8_t len) {
    if(!I2C_initiate_read(reg_address)) {
        handleError();
    }
    for(int i = 0; i < len-1; i++) {
        buf[i] = I2C_getByte(true);
    }
    buf[len-1] = I2C_getByte(false);
}

void handleError() {
  //print to serial
}