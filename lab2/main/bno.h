#ifndef BNO_H
#define BNO_H
/* File: bno.ino
 * Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * This file provides a basic implementation for transmitting
 * and receiving data from the BNO055.
 * Functionality:
 * -Read IMU data from the BNO055
 */
#include "Arduino.h"

static const uint8_t BNO055_PAGE_ID_ADDR                                     = 0X07;

static const uint8_t BNO055_CHIP_ID = 0xA0;

/* PAGE0 REGISTER DEFINITION START*/
static const uint8_t  BNO055_CHIP_ID_ADDR                                     = 0x00;
static const uint8_t  BNO055_ACCEL_REV_ID_ADDR                                = 0x01;
static const uint8_t  BNO055_MAG_REV_ID_ADDR                                  = 0x02;
static const uint8_t  BNO055_GYRO_REV_ID_ADDR                                 = 0x03;
static const uint8_t  BNO055_SW_REV_ID_LSB_ADDR                               = 0x04;
static const uint8_t  BNO055_SW_REV_ID_MSB_ADDR                               = 0x05;
static const uint8_t  BNO055_BL_REV_ID_ADDR                                   = 0X06;

/* Accel data register */
static const uint8_t  BNO055_ACCEL_DATA_X_LSB_ADDR                            = 0X08;
static const uint8_t  BNO055_ACCEL_DATA_X_MSB_ADDR                            = 0X09;
static const uint8_t  BNO055_ACCEL_DATA_Y_LSB_ADDR                            = 0X0A;
static const uint8_t  BNO055_ACCEL_DATA_Y_MSB_ADDR                            = 0X0B;
static const uint8_t  BNO055_ACCEL_DATA_Z_LSB_ADDR                            = 0X0C;
static const uint8_t  BNO055_ACCEL_DATA_Z_MSB_ADDR                            = 0X0D;

/* Mag data register */
static const uint8_t  BNO055_MAG_DATA_X_LSB_ADDR                              = 0X0E;
static const uint8_t  BNO055_MAG_DATA_X_MSB_ADDR                              = 0X0F;
static const uint8_t  BNO055_MAG_DATA_Y_LSB_ADDR                              = 0X10;
static const uint8_t  BNO055_MAG_DATA_Y_MSB_ADDR                              = 0X11;
static const uint8_t  BNO055_MAG_DATA_Z_LSB_ADDR                              = 0X12;
static const uint8_t  BNO055_MAG_DATA_Z_MSB_ADDR                              = 0X13;

/* Gyro data registers */
static const uint8_t  BNO055_GYRO_DATA_X_LSB_ADDR                             = 0X14;
static const uint8_t  BNO055_GYRO_DATA_X_MSB_ADDR                             = 0X15;
static const uint8_t  BNO055_GYRO_DATA_Y_LSB_ADDR                             = 0X16;
static const uint8_t  BNO055_GYRO_DATA_Y_MSB_ADDR                             = 0X17;
static const uint8_t  BNO055_GYRO_DATA_Z_LSB_ADDR                             = 0X18;
static const uint8_t  BNO055_GYRO_DATA_Z_MSB_ADDR                             = 0X19;

/* Quaternion data registers */
static const uint8_t  BNO055_QUATERNION_DATA_W_LSB_ADDR                       = 0X20;
static const uint8_t  BNO055_QUATERNION_DATA_W_MSB_ADDR                       = 0X21;
static const uint8_t  BNO055_QUATERNION_DATA_X_LSB_ADDR                       = 0X22;
static const uint8_t  BNO055_QUATERNION_DATA_X_MSB_ADDR                       = 0X23;
static const uint8_t  BNO055_QUATERNION_DATA_Y_LSB_ADDR                       = 0X24;
static const uint8_t  BNO055_QUATERNION_DATA_Y_MSB_ADDR                       = 0X25;
static const uint8_t  BNO055_QUATERNION_DATA_Z_LSB_ADDR                       = 0X26;
static const uint8_t  BNO055_QUATERNION_DATA_Z_MSB_ADDR                       = 0X27;

/* Mode registers */
static const uint8_t  BNO055_OPR_MODE_ADDR                                    = 0X3D;
static const uint8_t  OPERATION_MODE_CONFIG                                   = 0X00;
static const uint8_t  BNO055_PWR_MODE_ADDR                                    = 0X3E;
static const uint8_t  OPERATION_MODE_NDOF                                     = 0X0C;
static const uint8_t  BNO055_SYS_TRIGGER_ADDR                                 = 0X3F;
static const uint8_t  BNO055_TEMP_SOURCE_ADDR                                 = 0X40;

/* Power settings */
static const uint8_t  POWER_MODE_NORMAL                                       = 0X00;
static const uint8_t  POWER_MODE_LOWPOWER                                     = 0X01;
static const uint8_t  POWER_MODE_SUSPEND                                      = 0X02;

bool bno_init();
void bno_gyro(float* buf);
void bno_accelerometer(float* buf);
void bno_magnetometer(float* buf);
void bno_orientation(float* buf);
#endif // BNO_H
