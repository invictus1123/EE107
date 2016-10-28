bool bno_init() {
    /* Make sure we have the right device */
    uint8_t id = I2C_read(BNO055_CHIP_ID_ADDR);
    if(id != BNO055_CHIP_ID)
    {
     delay(1000); // hold on for boot
     id = I2C_read(BNO055_CHIP_ID_ADDR);
     if(id != BNO055_CHIP_ID) {
       return false;  // still not? ok bail
     }
    }

    /* Switch to config mode (just in case since this is the default) */
    I2C_write(BNO055_OPR_MODE_ADDR,OPERATION_MODE_CONFIG);

    /* Reset */
    I2C_write(BNO055_SYS_TRIGGER_ADDR, 0x20);
    while (I2C_read(BNO055_CHIP_ID_ADDR) != BNO055_CHIP_ID)
    {
     delay(10);
    }
    delay(50);

    /* Set to normal power mode */
    I2C_write(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
    delay(10);

    I2C_write(BNO055_PAGE_ID_ADDR, 0);

    //use external crystal
    I2C_write(BNO055_SYS_TRIGGER_ADDR, 0x80);
    delay(10);
    /* Set the operating mode to full functionality (see section 3.3) */
    I2C_write(BNO055_OPR_MODE_ADDR, OPERATION_MODE_NDOF);
    delay(35);
    return true;
}

void bno_gyro(float* buf) {
     uint8_t buffer8[6];
     int16_t x = 0;
     int16_t y = 0;
     int16_t z = 0;
     I2C_read_len(BNO055_GYRO_DATA_X_LSB_ADDR,buffer8,6);
     x = ((int16_t)buffer[0]) | (((int16_t)buffer[1]) << 8);
     y = ((int16_t)buffer[2]) | (((int16_t)buffer[3]) << 8);
     z = ((int16_t)buffer[4]) | (((int16_t)buffer[5]) << 8);
     buf[0] = ((double)x)/900.0;
     buf[1] = ((double)y)/900.0;
     buf[2] = ((double)z)/900.0;
}
void bno_accelerometer(float* buf){
     uint8_t buffer8[6];
     int16_t x = 0;
     int16_t y = 0;
     int16_t z = 0;
     I2C_read_len(BNO055_ACCEL_DATA_X_LSB_ADDR,buffer8,6);
     x = ((int16_t)buffer[0]) | (((int16_t)buffer[1]) << 8);
     y = ((int16_t)buffer[2]) | (((int16_t)buffer[3]) << 8);
     z = ((int16_t)buffer[4]) | (((int16_t)buffer[5]) << 8);
     buf[0] = ((double)x)/100.0;
     buf[1] = ((double)y)/100.0;
     buf[2] = ((double)z)/100.0;
}
void bno_magnetometer(float* buf){
     uint8_t buffer8[6];
     int16_t x = 0;
     int16_t y = 0;
     int16_t z = 0;
     I2C_read_len(BNO055_MAG_DATA_X_LSB_ADDR,buffer8,6);
     x = ((int16_t)buffer[0]) | (((int16_t)buffer[1]) << 8);
     y = ((int16_t)buffer[2]) | (((int16_t)buffer[3]) << 8);
     z = ((int16_t)buffer[4]) | (((int16_t)buffer[5]) << 8);
     buf[0] = ((double)x)/16.0;
     buf[1] = ((double)y)/16.0;
     buf[2] = ((double)z)/16.0;
}
void bno_orientation(float* buf){
    uint8_t* buffer8[8] = {0};
    I2C_read_len(BNO055_QUATERNION_DATA_W_LSB_ADDR, buffer8, 8);
    uint16_t w = (((uint16_t)buf[1]) << 8) | ((uint16_t)buf[0]);
    uint16_t x = (((uint16_t)buf[3]) << 8) | ((uint16_t)buf[2]);
    uint16_t y = (((uint16_t)buf[5]) << 8) | ((uint16_t)buf[4]);
    uint16_t z = (((uint16_t)buf[7]) << 8) | ((uint16_t)buf[6]);
    double scale = (1.0 / (1<<14));
    buf[0] = w*scale;
    buf[1] = x*scale;
    buf[2] = y*scale;
    buf[3] = z*scale;
}
