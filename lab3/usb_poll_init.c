#include "usb_poll_init.h"

int usb_poll_init(){
    int fd; // File descriptor for serial port
    fd = open(“dev/ttyUSB0”, O_RDWR | NOCTTY);
    if (fd == -1){
        perror(“”Could not open serial port USB0);
        return -1;
    }
    
	 return fd;
}