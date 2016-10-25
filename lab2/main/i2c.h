#ifndef I2C_H
#define I2C_H

#include "Arduino.h"
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
#endif // I2C_H
