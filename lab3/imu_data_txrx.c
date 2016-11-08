#include “imu_data_txrx.h”

int imu_data_txrx(int usb_id, int *store_val, int *cnt, uint8_t *buf){
    int rx_count;
    int timestamp;

    rx_count = read(usb_id, store_val, 1); // Read 1 byte (block until byte is read, or timeout occurs)
    timestamp = (int) time(NULL);    // Take timestamp immediately after reading (even if unused)

    if (rx_count == -1){
        // Read error
    }
	 
    /* Store timestamp in transmit buffer if last byte */
    if (*cnt == 3){
        buf[3] = (uint8_t) *store_val;
        buf[4] = (timestamp >> 24) & 0xFF;
        buf[5] = (timestamp >> 16) & 0xFF;
        buf[6] = (timestamp >> 8) & 0xFF;
        buf[7] = timestamp & 0xFF;

        *cnt = 0;
        return 1;
    } else {
        *cnt++;
        buf[cnt] = (uint8_t) *store_val;
        return 0;
    }

    
}
