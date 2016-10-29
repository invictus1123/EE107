/* File: I2C_impl.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file provides a basic implementation for transmitting
 * and receiving data on the I2C bus.
 * Functionality: 
 * -Read one or more bytes of data from a specified register on another device
 */
 /*I2C_read_len() testing: The register 0x00 was chosen as the start of a 6 byte read. 
 Since the BNO will continue to read down memory when multiple bytes are requested, we read the first 4 registers
 to see if they matched the values of the first 4 registers (register IDs, which are fixed) in the BNO. These values should read 
 0xA0, 0xFB, 0x32, 0x0F, which is what the program outputs.
 */
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

void setup() {
    Serial.begin(9600);
}

void loop() {
  uint8_t buf[4];
    I2C_read_len(0x00,buf,4); //read the BNO055 chip ID register
    for(int i = 0; i < 4; i++ ) {
        Serial.println(buf[i],HEX);
    }
    delay(1000);
}

bool I2C_write(uint8_t reg, uint8_t data) {
        TWCR = (1<<TWINT) |(1<<TWSTA) | (1<<TWEN); //set TWINT to clear interrupt, start condition, enable acknowledge bits, and enable I2C.
    while( !(TWCR & (1<<TWINT)) ) {}
    if( ((TWSR & 0xF8)!= START) && ((TWSR & 0xF8) !=R_START) ){//check TWSR to see if the start bit was transmitted (code 0x08)
        handleError(START);
        TWCR = 0;
        return false;
    } 
    TWDR = (SLAVE_ADDRESS<<1); //slave address + write bit (0)
    TWCR = (1<<TWINT) | (1<<TWEN); //set interrupt bit and enable bit to start transmission of slave address
    while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
    if((TWSR & 0xF8) != WRITE_ACK) { //if we got acknowedgement to write(status code 0x18), continue
        handleError(WRITE_NACK);
        TWCR = 0;
        return false;
    }
    TWDR = reg; //write to the sensor the register we want to modify
    TWCR = (1<<TWINT) | (1<<TWEN); 
    while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
    if((TWSR & 0xF8) != DATA_SENT) {//if we sent data and got acknowledgement (status code 0x28), continue
        handleError(DATA_NOT_SENT);
        TWCR = 0;
        return false;
    }
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);     
    while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
    if((TWSR & 0xF8) != DATA_SENT) {//if we sent data and got acknowledgement (status code 0x28), continue
        handleError(DATA_NOT_SENT);
        TWCR = 0;
        return false;
    } 
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); //end transmission
    return true;
}
/* Write to the sensor the register we want to read from. Postcondition: data sent and acknowledgement received, pending repeated start */
bool I2C_initiate_read(uint8_t reg_address) {
     /* Set bits TWINT (7), TWSTA(5), and TWEN(2) set to 1
     * TWINT: Interrupt flag, which generates an interrupt 
     * TWSTA: Flag that results in the start condition being written
     * TWEN: Enables I2C interface.
     */
    TWCR = (1<<TWINT) |(1<<TWSTA) | (1<<TWEN); //set TWINT to clear interrupt, start condition, enable acknowledge bits, and enable I2C.
    while( !(TWCR & (1<<TWINT)) ) {}
    if( ((TWSR & 0xF8)!= START) && ((TWSR & 0xF8) !=R_START) ){//check TWSR to see if the start bit was transmitted (code 0x08)
        handleError(START);
        TWCR = 0;
        return false;
    } 
    TWDR = (SLAVE_ADDRESS<<1); //slave address + write bit (0)
    TWCR = (1<<TWINT) | (1<<TWEN); //set interrupt bit and enable bit to start transmission of slave address
    while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
    if((TWSR & 0xF8) != WRITE_ACK) { //if we got acknowedgement to write(status code 0x18), continue
        handleError(WRITE_NACK);
        TWCR = 0;
        return false;
    }
    TWDR = reg_address; //write to the sensor the register we want to read
    TWCR = (1<<TWINT) | (1<<TWEN); 
    while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
    if((TWSR & 0xF8) != DATA_SENT) {//if we sent data and got acknowledgement (status code 0x28), continue
        handleError(DATA_NOT_SENT);
        TWCR = 0;
        return false;
    }
    return true;
}

uint8_t I2C_getByte(bool moreDataLeft) {
    uint8_t data = 0;
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA); //transmit repeated start
    while( !(TWCR & (1<<TWINT)) ) {}
    if(((TWSR & 0xF8)!= R_START) && ((TWSR & 0xF8) !=START)){ //check we sent a repeated start
        handleError(R_START);
    }
    TWDR = (SLAVE_ADDRESS<<1) | READ_BIT; //slave address + read bit (1)
    TWCR = (1<<TWINT) | (1<<TWEN); //set interrupt bit and enable bit to start transmission of slave address
    while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
    if((TWSR & 0xF8) != READ_ACK) { //if we got acknowedgement to read(status code 0x40), continue
        handleError(READ_NACK);
    }
    if(moreDataLeft) {
        TWCR = (1<<TWINT) | (1<<TWEN);
        while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement
        if((TWSR & 0xF8) != DATA_RECEIVED) {//if we got data and sent acknowledgement (status code 0x50), continue
            handleError(DATA_RECEIVED);
        }
    } else {
        TWCR = (1<<TWINT) | (1<<TWEN);
        while( !(TWCR & (1<<TWINT)) ) {} //wait for acknowledgement send 
        if((TWSR & 0xF8) != LAST_RECEIVED) {//if we got data and sent acknowledgement (status code 0x50), continue
            handleError(LAST_RECEIVED);
        }   
    }
    data = TWDR;
    if(!moreDataLeft) {
        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); //transmit stop condition
    }
    return data;
}

uint8_t I2C_read(uint8_t reg_address) {
    uint8_t data = 0;
    if(!I2C_initiate_read(reg_address)) {
        Serial.println("Read encountered an error");
        return -1;
    }
    data = I2C_getByte(false);
    return data;
}

bool I2C_read_len(uint8_t reg_address, uint8_t *buf, uint8_t len) {
    for(int i = 0; i < len-1; i++) {
        if(!I2C_initiate_read(reg_address)) {
        Serial.println("Read encountered an error");
        }
        buf[i] = I2C_getByte(false);
        reg_address++;
    }
//    buf[len-1] = I2C_getByte(false);
}

void handleError(uint8_t error) {
  switch(error) {
      case READ_NACK:     Serial.println("Did not get acknowledgement for read privilege");break;
      case DATA_RECEIVED: Serial.println("Did not successfully receive and acknowledge data");break;
      case WRITE_NACK:    Serial.println("Did not get acknowledgement for write privilege");break;
      case DATA_NOT_SENT: Serial.println("Did not successfully send data and get acknowledgement");break;
      case START:         Serial.println("Start condition not sent"); break;
      case R_START:       Serial.println("Repeated start condition not sent"); break;
      default:break;
  }
}
