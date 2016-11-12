#include "usb_poll_init.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

int usb_poll_init(){
    int fd; // File descriptor for serial port

    fd = open("/dev/ttyUSB0", O_RDONLY);
    if (fd == -1){
        perror("Could not open serial port USB0");
        return -1;
    }
    
    // fcntl(fd, F_SETFL, 0); // Not sure if this is necessary
    return fd;
}
