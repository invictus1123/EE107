/* File: process_imu.c */
/* Written by Vinh Nguyen & LC Tao
 * EE107, Sachin Katti, Fall 2016
 * Functionality: Decodes packets and writes
 * IMU data to the struct for further use.
 */
#include <stdint.h>
#include <stdbool.h>
#include "process_imu.h"
#include <stdio.h>

float decodeData(char msByte, char lsByte, bool acc_or_quat) {
    float ret = 0.0;
    bool neg = false;
    if(msByte <0) { //if this bit was encoded negative, flip it to its absolute value and set the flag true
        msByte = -msByte;
        neg = true;
    }
    if(lsByte <0) {
        lsByte = -lsByte;
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
void process_imu(char *buff, Imu *imu_data) {
    int i;
    for(i = 0; i < BYTES_PER_ENTRY*ENTRIES_PER_PACKET+TSTAMP_SIZE; i++) {
        imu_data->raw_data[i] = buff[i];
    }
    float ax = decodeData(buff[0],buff[1],true);
    imu_data->accel_x = ax;
    float ay = decodeData(buff[2],buff[3],true);
    imu_data->accel_y = ay;
    float az = decodeData(buff[4],buff[5],true);
    imu_data->accel_z = az;
    float quatw = decodeData(buff[6],buff[7],false);
    imu_data->q_1 = quatw;
    float quatx = decodeData(buff[8],buff[9],false);
    imu_data->q_i = quatx;
    float quaty = decodeData(buff[10],buff[11],false);
    imu_data->q_j = quaty;
    float quatz = decodeData(buff[12],buff[3],false);
    imu_data->q_k = quatz;
    uint32_t timestamp = ((uint32_t)buff[14]<<24) + ((uint32_t)buff[15]<<16) + ((uint32_t)buff[16]<<8) + (buff[17]);
    imu_data->t_stamp = timestamp;
    printf("accelerometer (x,y,z) = (%f,%f,%f)\n", \
        imu_data->accel_x, imu_data->accel_y, imu_data->accel_z);
    printf("quaternion (q1,q2,q3,q4) = (%f,%f,%f,%f)\n", \
        imu_data->q_1, imu_data->q_i, imu_data->q_j, imu_data->q_k);
    printf("timestamp = %d\n", imu_data->t_stamp);    
}
