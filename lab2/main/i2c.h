#ifndef I2C_H
#define I2C_H

#include "Arduino.h"
#define SLAVE_ADDRESS 0x28 //default address of the BNO055
#define READ_BIT B00000001 //write bit is 0
#define SCL_RATE
// TWSR status codes
#define START 0x08 //code signifying the START bit was sent
#define R_START 0x10 //code signifying the repeated start bit was sent
#define WRITE_ACK 0x18 //code signifying the slave acknowledged our write privilege
#define WRITE_NACK 0x20 //code signifying the slave did not acknowledge our write privilege
#define DATA_SENT 0x28 //code signifying the slave acknowledged and received the data we sent
#define DATA_NOT_SENT 0x30 //code signifying the slave acknowledged that it failed to get what we sent
#define READ_ACK 0x40 //code signifying the slave acknowledged our read privilege
#define READ_NACK 0x48 //code signifying the slave acknowledged our read privilege
#define DATA_RECEIVED 0x50 //code signifying we successfully received data and acknowledged
#define LAST_RECEIVED 0x58 //code signifying we successfully received the last byte we want and have acknowledged an end to the transaction


uint8_t I2C_read(uint8_t reg_address);

bool I2C_read_len(uint8_t reg_address, uint8_t* buf, uint8_t len);

bool I2C_initiate_read(uint8_t reg_address);

uint8_t I2C_getByte(bool moreDataLeft);

void handleError();
#endif // I2C_H
