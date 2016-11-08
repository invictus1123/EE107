#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include "uart_hardware.h"
#include "Wire.h"
static const int MAX_BUF_SIZE = 256;
int txsize = 0;
uint8_t txbuffer[MAX_BUF_SIZE] = {0};

void encodeData(uint8_t* txbuffer,float data, bool acc_or_quat);
void imu_data_tx();

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
    uart_hardware_init();
    bno.begin();
}

void loop() { 
    imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    imu::Quaternion quat = bno.getQuat();
    encodeData(txbuffer, acc.x(),true);
    encodeData(txbuffer, acc.y(),true);
    encodeData(txbuffer, acc.z(),true);
    encodeData(txbuffer, quat.w(),false);
    encodeData(txbuffer, quat.x(),false);
    encodeData(txbuffer, quat.y(),false);
    encodeData(txbuffer, quat.z(),false);
    for(int i = 0; i < txsize; i++) {
        uart_hardware_tx_byte(txbuffer[i]);
    }
}
bool isPositive(uint8_t secondByte) {
    return (secondByte < 128);
}
void encodeData(uint8_t* txbuffer, float data, bool acc_or_quat) {
    //format of accelerometer data: Byte 1: integer part, absolute value. Byte 2: 2 Decimal Places, plus sign (1 = -) in MSB. 
    if(acc_or_quat) {
        uint8_t intPart = (int) abs(data); //truncates the data;
        uint8_t decPart = (int) ((abs(data) - intPart)*100);
        if(txsize < MAX_BUF_SIZE-1) {
            txbuffer[txsize] = abs(intPart);
            if(data<0) {
                decPart = decPart | (B10000000);
            }
            txbuffer[txsize+1] = decPart;
            txsize+=2;
        } else {
            Serial.println("TX Buffer overflow!");             
        }
    } else {
      //format of quaternion data: Byte 1: first 2 decimal places. Byte 2: 3rd/4th decimal places, plus sign (1 = -) in MSB. 
        uint8_t dec12 = (int) (abs(data)*100);
        uint8_t dec34 = (int) (abs(data)*10000 - abs((dec12*100)));
        if(txsize < MAX_BUF_SIZE - 1) {
            txbuffer[txsize] = dec12; 
            if(data<0) {
                dec34 = dec34 | (B10000000);
            }
            txbuffer[txsize+1] = dec34;
            txsize+=2;
        } else {
            Serial.println("TX Buffer overflow!");
        }
    }
}

