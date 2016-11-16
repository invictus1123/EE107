#include "imu_data_txrx.h"

#define START_BYTE -128
#define STOP_BYTE 127

int imu_data_txrx(int usb_id, int *store_val, int *cnt, uint8_t *buf){
    int rx_count;
    int timestamp;
    int8_t read_buf;

    rx_count = read(usb_id, &read_buf, 1); // Read 1 byte (block until byte is read, or timeout occurs)
	
    // Read error
    if (rx_count != 1){
        if (rx_count == 0){
            printf("Timeout, no data read\n");
        } else {
            printf("Read error\n");
        }
        //*cnt = 0; // Not sure if cnt should be reset
        return 0;
    }
    // Stop byte read
    if (read_buf == STOP_BYTE){
        // Append timestamp to buf
        timestamp = (int) time(NULL);
        buf[*cnt] = (timestamp >> 24) & 0xFF;
        buf[*cnt + 1] = (timestamp >> 16) & 0xFF;
        buf[*cnt + 2] = (timestamp >> 8) & 0xFF;
        buf[*cnt + 3] = timestamp & 0xFF;
        // Reset store_val and cnt
        *store_val = 0;
        *cnt = 0;
        return 1; //Complete IMU data (including timestamp) has been read
    // Start byte read
    } else if (read_buf == START_BYTE){
        *store_val = 1;
    } else if (*store_val == 1){
        buf[*cnt] = (uint8_t) read_buf;
        *cnt = *cnt + 1;
    }

    return 0;
}
