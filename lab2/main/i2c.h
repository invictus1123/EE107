#ifndef I2C_H
#define I2C_H
/* File: I2C_impl.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file provides a basic implementation for transmitting
 * and receiving data on the I2C bus.
 * Functionality: 
 * -Read one or more bytes of data from a specified register on another device
 */
 /*I2C_read_len() testing: The register 0x00 was chosen as the start of a 4 byte read. 
 Since the BNO will continue to read down memory when multiple bytes are requested, we read the first 4 registers
 to see if they matched the values of the first 4 registers (register IDs, which are fixed) in the BNO. These values should read 
 0xA0, 0xFB, 0x32, 0x0F, which is what the program outputs. See I2C_impl.ino for this test.
 */
 
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
bool I2C_write(uint8_t reg, uint8_t data);
bool I2C_read_len(uint8_t reg_address, uint8_t* buf, uint8_t len);
bool I2C_initiate_read(uint8_t reg_address);
uint8_t I2C_getByte();
void handleError(uint8_t error);

#endif // I2C_H
