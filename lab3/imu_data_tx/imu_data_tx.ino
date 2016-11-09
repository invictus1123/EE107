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
    imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Quaternion quat = bno.getQuat();
    encodeData(txbuffer, acc.x(),true);
    encodeData(txbuffer, acc.y(),true);
    encodeData(txbuffer, acc.z(),true);
    encodeData(txbuffer, quat.w(),false);
    encodeData(txbuffer, quat.x(),false);
    encodeData(txbuffer, quat.y(),false);
    encodeData(txbuffer, quat.z(),false);
    for(int i = 0; i < txsize; i++) {
          Serial.println(txbuffer[i]);
//        uart_hardware_tx_byte(txbuffer[i]);
    }
    txsize = 0;
    delay(100);
/* Test Code for Data Encoding */
//    float ax = 0.00;
//    encodeData(txbuffer,ax,true);
//    float ay = -0.33;
//    encodeData(txbuffer,ay,true);
//    float az = -1.00;
//    encodeData(txbuffer,az,true);
//    float qw = .0011;
//    encodeData(txbuffer,qw,false);
//    float qx = -.0001;
//    encodeData(txbuffer,qx,false);
//    float qy = -.3300;
//    encodeData(txbuffer,qy,false);
//    float qz = -.3388;
//    encodeData(txbuffer,qz,false);
//    float ax2 = 1.49;
//    encodeData(txbuffer,ax2,true);
//    float ay2 = -9.8;
//    encodeData(txbuffer,ay2,true);
//    float az2 = -1.99;
//    encodeData(txbuffer,az2,true);
//    float qw2 = -.3154;
//    encodeData(txbuffer,qw2,false);
//    float qx2 = .3853;
//    encodeData(txbuffer,qx2,false);
//    for(int i = 0; i < txsize; i++) {
//        uint8_t txbyte = txbuffer[i];
//        if(txbyte > 128) Serial.println('-');
//        txbyte = (txbyte > 128) ? (256 - txbyte) : txbyte; //undo two's complement encoding to see the original digits
//        Serial.println(txbyte);
//    }
//    delay(2000);
//    txsize = 0;

}
bool isPositive(uint8_t secondByte) {
    return (secondByte < 128);
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
