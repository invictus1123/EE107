#include "imu_data_txrx.h"

int imu_data_txrx(int usb_id, int *store_val, int *cnt, uint8_t *buf){
    int rx_count;
    int timestamp;

    rx_count = read(usb_id, (void *)store_val, 1); // Read 1 byte (block until byte is read, or timeout occurs)
    timestamp = (int) time(NULL);    // Take timestamp immediately after reading (even if unused)

    if (rx_count != 1){
        // Read error
	printf("Read error\n");
	*cnt = 0;
	return 0;
    }
    printf("Count: %d\n", *cnt); 
    printf("Store_val: %d\n", *store_val);
    printf("Timestamp: %d\n", timestamp);
    /* Store timestamp in transmit buffer if last byte */
    if (*cnt == 13){
        buf[13] = (uint8_t) *store_val;
        buf[14] = (timestamp >> 24) & 0xFF;
        buf[15] = (timestamp >> 16) & 0xFF;
        buf[16] = (timestamp >> 8) & 0xFF;
        buf[17] = timestamp & 0xFF;

        *cnt = 0;
        return 1;
    } else {
        buf[*cnt] = (uint8_t) *store_val;
        *cnt = *cnt + 1;
        return 0;
    }

    
}
