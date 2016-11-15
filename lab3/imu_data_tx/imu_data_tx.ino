#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include "uart_hardware.h"
#include "Wire.h"
static const int MAX_BUF_SIZE = 256;
int txsize = 0;
uint8_t txbuffer[MAX_BUF_SIZE] = {0};
static const uint8_t START_BYTE = 128; //(-128)
static const uint8_t STOP_BYTE = 127;  //(127)

void encodeData(uint8_t* txbuffer,float data, bool acc_or_quat);
void imu_data_tx();
struct Imu  {
    float accel_x = 0.0;
    float accel_y = 0.0;
    float accel_z = 0.0;
    float q_1 = 0.0;
    float q_i = 0.0;
    float q_j = 0.0;
    float q_k = 0.0;
    uint32_t t_stamp = 0;
};

Adafruit_BNO055 bno = Adafruit_BNO055(55);
/* File: process_imu.c */

float decodeData(uint8_t msByte, uint8_t lsByte, bool acc_or_quat) {
    float ret = 0.0;
    bool neg = false;
    if(msByte > 128) { //if this bit was encoded negative, flip it to its absolute value and set the flag true
        msByte = 256-msByte;
        neg = true;
    }
    if(lsByte > 128) {
        lsByte = 256-lsByte;
        neg = true;
    }
    if(acc_or_quat) {
        ret+=msByte;
        ret+= (float) (lsByte*.01);
        ret = (neg) ? -ret : ret; //add the negative sign if we found earlier that the number was negative.
        return ret;
    } else {
        ret+=.01*msByte;
        ret+=.0001*lsByte; 
        ret = (neg) ? -ret : ret;
        return ret;
    }
}
void process_imu(uint8_t *buf, Imu *imu_data) {
    float ax = decodeData(buf[0],buf[1],true);
    imu_data->accel_x = ax;
    float ay = decodeData(buf[2],buf[3],true);
    imu_data->accel_y = ay;
    float az = decodeData(buf[4],buf[5],true);
    imu_data->accel_z = az;
    float quatw = decodeData(buf[6],buf[7],false);
    imu_data->q_1 = quatw;
    float quatx = decodeData(buf[8],buf[9],false);
    imu_data->q_i = quatx;
    float quaty = decodeData(buf[10],buf[11],false);
    imu_data->q_j = quaty;
    float quatz = decodeData(buf[12],buf[3],false);
    imu_data->q_k = quatz;
    uint32_t timestamp = (uint32_t)buf[14]<<24 + (uint32_t)buf[15]<<16 + (uint32_t)buf[16]<<8 + buf[17];
    imu_data->t_stamp = timestamp;    
}
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
//    Imu *imu = new Imu();
//    process_imu(txbuffer,imu);
//    printf("accelerometer (x,y,z) = (%f,%f,%f)\n", \
//            imu->accel_x, imu->accel_y, imu->accel_z);
//
//
//    printf("quaternion (q1,q2,q3,q4) = (%f,%f,%f,%f)\n", \
//            imu->q_1, imu->q_i, imu->q_j, imu->q_k);
//
//
//    printf("timestamp = %d\n", imu->t_stamp);
//    Serial.println("Actual data:");
//    printf("accelerometer (x,y,z) = (%f,%f,%f)\n", \
//            acc.x(), acc.y(), acc.z());
//
//
//    printf("quaternion (q1,q2,q3,q4) = (%f,%f,%f,%f)\n", \
//            quat.w(), quat.x(), quat.y(), quat.z());

    for(int i = 0; i < txsize; i++) {
//          Serial.println(txbuffer[i]);
        uart_hardware_tx_byte(txbuffer[i]);
    }
    txsize = 0;
    delay(2000);
/* Test Code for Data Encoding */
//    float ax = 0.01;
//    encodeData(txbuffer,ax,true);
//    float ay = 0.22;
//    encodeData(txbuffer,ay,true);
//    float az = 3.33;
//    encodeData(txbuffer,az,true);
//    float qw = -.0001;
//    encodeData(txbuffer,qw,false);
//    float qx = .1122;
//    encodeData(txbuffer,qx,false);
//    float qy = .3344;
//    encodeData(txbuffer,qy,false);
//    float qz = -.0001;
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
//        uart_hardware_tx_byte(txbuffer[i]);
//        Serial.print(txbuffer[i]);
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
