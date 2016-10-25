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
#include "uart_hardware.h"
#define BNO055_SAMPLERATE_DELAY_MS 100
Adafruit_BNO055 bno = Adafruit_BNO055(55);

  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2
void imu_gyro();
void imu_accelerometer();
void imu_magnetometer();
void imu_orientation();

void setup() {
   Wire.begin();
//   Serial.begin(9600);
    uart_hardware_init();
    if(!bno.begin()) {
        uart_hardware_tx_str("Failed to initialize\n");
    }
    bno.setExtCrystalUse(true);
}

//void loop() {
//    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
//    imu::Quaternion quat = bno.getQuat();
//    uart_hardware_tx_str
//}
