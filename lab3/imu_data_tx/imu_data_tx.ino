#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include "uart_hardware.h"
#include "Wire.h"
static const int MAX_BUF_SIZE = 256;
int txsize = 0;
uint8_t txbuffer[MAX_BUF_SIZE] = {0};
static const uint8_t START_BYTE = 128; //(-128)
static const uint8_t STOP_BYTE = 127;  //(127)
static const int DATA_RATE_MS = 2000;

void encodeData(uint8_t* txbuffer,float data, bool acc_or_quat);
void imu_data_tx();

Adafruit_BNO055 bno = Adafruit_BNO055(55);
/* File: process_imu.c */
void setup() {
    uart_hardware_init();
    bno.begin();
}

void loop() { 
    imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Quaternion quat = bno.getQuat();
    txbuffer[txsize] = START_BYTE;
    txsize++;
    encodeData(txbuffer, acc.x(),true);
    encodeData(txbuffer, acc.y(),true);
    encodeData(txbuffer, acc.z(),true);
    encodeData(txbuffer, quat.w(),false);
    encodeData(txbuffer, quat.x(),false);
    encodeData(txbuffer, quat.y(),false);
    encodeData(txbuffer, quat.z(),false);
    txbuffer[txsize] = STOP_BYTE;
    txsize++;
    for(int i = 0; i < txsize; i++) {
//          Serial.println(txbuffer[i]);
        uart_hardware_tx_byte(txbuffer[i]);
    }
    txsize = 0;
    delay(DATA_RATE_MS);
}

void encodeData(uint8_t* txbuffer, float data, bool acc_or_quat) {
    //format of accelerometer data: Byte 1: integer part, absolute value. Byte 2: 2 Decimal Places, plus sign (1 = -) in MSB. 
    if(acc_or_quat) {
        uint8_t intPart = (int) data; //truncates the data;
        uint8_t decPart = (int) ( intPart == 0 && data < 0 ) ? (((data) - intPart)*100) : (abs((data) - intPart)*100);
        if(txsize < MAX_BUF_SIZE-1) {
            txbuffer[txsize] = intPart;
            txbuffer[txsize+1] = decPart;
            txsize+=2;
        } else {
            Serial.println("TX Buffer overflow!");             
        }
    } else {
      //format of quaternion data: Byte 1: first 2 decimal places. Byte 2: 3rd/4th decimal places, plus sign (1 = -) in MSB. 
        uint8_t dec12 = (int) ((data)*100);
        uint8_t dec34 = (int) ( dec12 == 0 && data < 0 ) ? (data*10000 - dec12*100) : (abs((data)*10000 - (dec12*100)));
        if(txsize < MAX_BUF_SIZE - 1) {
            txbuffer[txsize] = dec12; 
            txbuffer[txsize+1] = dec34;
            txsize+=2;
        } else {
            Serial.println("TX Buffer overflow!");
        }
    }
}
