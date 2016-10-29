/* File: main.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file provides a basic implementation for transmitting
 * and receiving data on the I2C bus.
 * Functionality: 
 * -Read the orientation, accelerometer, gyroscope, and magnetometer
 */
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Wire.h>
#include "i2c.h"
#include "bno.h"
#include "uart_hardware.h"
#define BNO055_SAMPLERATE_DELAY_MS 100
float acc_buf[6];
float mag_buf[6];
float orient_buf[8];
float gyro_buf[6];
//Adafruit_BNO055 bno = Adafruit_BNO055(55);

  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2
void imu_gyro(float* buf);
void imu_accelerometer(float* buf);
void imu_magnetometer(float* buf);
void imu_orientation(float* buf);

void setup() {
//   Wire.begin();
   Serial.begin(9600);
    uart_hardware_init();
//    if(!bno.begin()) {
//        uart_hardware_tx_str("Failed to initialize\n");
//    }
//    bno.setExtCrystalUse(true);
    if(!bno_init()) {
        Serial.println("initialization failed!");
    } else {
        Serial.println("BNO initialized successfully");
    }
}

//void loop() {
//    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
//    imu::Quaternion quat = bno.getQuat();
//    uart_hardware_tx_str
//}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        void loop()
{
  
  imu_accelerometer(acc_buf);
  uart_hardware_tx_byte('\t');
  imu_gyro(gyro_buf);
  uart_hardware_tx_byte('\t');
  imu_magnetometer(mag_buf);
  uart_hardware_tx_byte('\t');
  imu_orientation(orient_buf);
  uart_hardware_tx_byte('\n');
  delay(BNO055_SAMPLERATE_DELAY_MS);
  
}

void imu_accelerometer(float *buf) {
//    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
//    uart_hardware_tx_str("Accelerometer: ");
//    uart_hardware_tx_str("x: ");
//    uart_hardware_tx_str((String)accel.x());
//    uart_hardware_tx_str(", y: ");  
//    uart_hardware_tx_str((String)accel.y());
//    uart_hardware_tx_str(", z: "); 
//    uart_hardware_tx_str((String)accel.z());
//    uart_hardware_tx_byte('.');
    bno_accelerometer(buf);
    uart_hardware_tx_str("Accelerometer: ");
    uart_hardware_tx_str("x: ");
    uart_hardware_tx_str((String)buf[0]);
    uart_hardware_tx_str(", y: ");  
    uart_hardware_tx_str((String)buf[1]);
    uart_hardware_tx_str(", z: "); 
    uart_hardware_tx_str((String)buf[2]);
    uart_hardware_tx_byte('.');
}

void imu_gyro(float *buf) {
//    imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
//    uart_hardware_tx_str("Gyroscope: ");
//    uart_hardware_tx_str("x: ");
//    uart_hardware_tx_str((String)gyro.x());
//    uart_hardware_tx_str(", y: ");  
//    uart_hardware_tx_str((String)gyro.y());
//    uart_hardware_tx_str(", z: "); 
//    uart_hardware_tx_str((String)gyro.z());
//    uart_hardware_tx_byte('.');
    bno_gyro(buf);
    uart_hardware_tx_str("Gyroscope: ");
    uart_hardware_tx_str("x: ");
    uart_hardware_tx_str((String)buf[0]);
    uart_hardware_tx_str(", y: ");  
    uart_hardware_tx_str((String)buf[1]);
    uart_hardware_tx_str(", z: "); 
    uart_hardware_tx_str((String)buf[2]);
    uart_hardware_tx_byte('.');
}

void imu_magnetometer(float* buf) {
//    imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
//    uart_hardware_tx_str("Magnetometer: ");
//    uart_hardware_tx_str("x: ");
//    uart_hardware_tx_str((String)mag.x());
//    uart_hardware_tx_str(", y: ");  
//    uart_hardware_tx_str((String)mag.y());
//    uart_hardware_tx_str(", z: "); 
//    uart_hardware_tx_str((String)mag.z());
//    uart_hardware_tx_byte('.');
    bno_magnetometer(buf);
    uart_hardware_tx_str("Magnetometer: ");
    uart_hardware_tx_str("x: ");
    uart_hardware_tx_str((String)buf[0]);
    uart_hardware_tx_str(", y: ");  
    uart_hardware_tx_str((String)buf[1]);
    uart_hardware_tx_str(", z: "); 
    uart_hardware_tx_str((String)buf[2]);
    uart_hardware_tx_byte('.');
}

void imu_orientation(float* buf) {
//    imu::Quaternion quat = bno.getQuat();
//    uart_hardware_tx_str("Orientation: ");
//    uart_hardware_tx_str(" q0: ");
//    uart_hardware_tx_str((String)quat.w());
//    uart_hardware_tx_str(", q1: ");
//    uart_hardware_tx_str((String)quat.x());
//    uart_hardware_tx_str(", q2: ");
//    uart_hardware_tx_str((String)quat.y());
//    uart_hardware_tx_str(", q3: ");
//    uart_hardware_tx_str((String)quat.z());
//    uart_hardware_tx_byte('.');
    bno_orientation(buf);
    uart_hardware_tx_str("Orientation: ");
    uart_hardware_tx_str(" q0: ");
    uart_hardware_tx_str((String)buf[0]);
    uart_hardware_tx_str(", q1: ");
    uart_hardware_tx_str((String)buf[1]);
    uart_hardware_tx_str(", q2: ");
    uart_hardware_tx_str((String)buf[2]);
    uart_hardware_tx_str(", q3: ");
    uart_hardware_tx_str((String)buf[3]);
    uart_hardware_tx_byte('.');
}

