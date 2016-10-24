/* File: I2C_impl.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file provides a basic implementation for transmitting
 * and receiving data on the I2C bus.
 * Functionality: 
 * -Read one or more bytes of data from a specified register on another device
 */
 #define SLAVE_ADDRESS 0x29 //default address of the BNO055
 #define READ_BIT B00000001 //write bit is 0
 // TWSR status codes
 #define START 0x08 //code signifying the START bit was sent
 #define R_START 0x10 //code signifying the repeated start bit was sent
 #define WRITE_ACK 0x18 //code signifying the slave acknowledged our write privilege
 #define DATA_SENT 0x28 //code signifying the slave acknowledged and received the data we sent
 #define READ_ACK 0x40 //code signifying the slave acknowledged our read privilege
 #define DATA_RECEIVED 0x50 //code signifying we successfully received data and acknowledged
uint8_t I2C_read(uint8_t reg_address);
bool I2C_read_len(uint8_t reg_address, uint8_t* buf, uint8_t len);
bool I2C_initiate_read(uint8_t reg_address);
uint8_t I2C_getByte();
void handleError();

void setup() {
}

void loop() {
}
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

uint8_t I2C_getByte() {
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
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); //transmit stop condition
    return data;
}

uint8_t I2C_read(uint8_t reg_address) {
    uint8_t data = 0;
    if(!I2C_initiate_read(reg_address)) {
        handleError();
    }
    data = I2C_getByte();
    return data;
}

bool I2C_read_len(uint8_t reg_address, uint8_t *buf, uint8_t len) {
    I2C_initiate_read(reg_address);
    for(int i = 0; i < len; i++) {
        buf[i] = I2C_getByte();
    }
}

void handleError() {
  //print to serial 
}
