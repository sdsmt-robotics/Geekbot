#ifndef __EASY_SERIAL_H
#define __EASY_SERIAL_H
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>    // File control definitions
#include <errno.h>    // Error number definitions
#include <termios.h>  // POSIX terminal control definitions, THE BIG ONE
#include <sys/ioctl.h>
#include <getopt.h>

/* This header is designed to provide easy serial port setup. To that end
 *  I like to think it performs well and is a -relatively- concise solution.
 */

/* This function initializes and opens a serial port for reading/writing. It
 *  uses a comically archaic system for doing so, but the means fit the medium :P
 *  It takes in a device location (/dev/ttyUSBX of /dev/ttySX) and baudrate, and
 *  spits out an INT file descriptor number. From that point it can be read from
 *  and written to like any file.
 *
 *  WARNING: Changing the flags on the init section is particulary... risky.
 */
int serial_port_init(const char* serialport, int baud)
{
    struct termios toptions; // Standard struct for holding port data/settings
    int fd;

    fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY); // Read/write, no delays
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
    
    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // Simple switch for the baudrate selection
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
    case 19200:  brate=B19200;  break;
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1, no hw flow control, configured for read access as well as write
    toptions.c_cflag &= ~(PARENB | CSTOPB | CSIZE | CRTSCTS | CREAD | CLOCAL);
    toptions.c_cflag |= CS8;

    toptions.c_iflag |= (IXON | IXOFF | IXANY); // turn on s/w flow ctrl
    toptions.c_iflag &= ~(INLCR | ICRNL); // make sure the CR and NL aren't mapped to each other on input
    // THE ABOVE OPTION'S EXISTENCE IS NECESSARY. IF YOU REMOVE IT
    //  YOU WILL SPEND HOURS PULLING YOUR HAIR OUR TROUBLESHOOTING
    //  STRINGS THAT JUST WON'T PARSE BECAUSE '\r' IS REPLACED WITH '\n'
    //  by the cheap serial->USB adapter you have or the different
    //  default port settings of your kernel. This bit me.
    
    toptions.c_oflag &= ~OPOST;           // make raw output as well
    toptions.c_oflag &= ~(INLCR | ICRNL); // make sure the CR and NL aren't mapped to each other on output

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw input, not line-based canonical

    toptions.c_cc[VMIN]  = 0; // Not necessary, but peace of mind to set timeouts
    toptions.c_cc[VTIME] = 20;
    
    if( tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
        printf("serial_port_init: Couldn't set term attributes"); // Self explanatory, attempts to set the port attribs
        return -1;
    }

    return fd;
}

// This shunts any piled-up/remaining data in the serial
//  device's buffers (input as the file descriptor) into
//  oblivion, guaranteeing clean input.
void clear_port(int port)
{
    int n = 1;
    char nothing = 0;
    while(n > 0)
        n = read(port, &nothing, 1); // If a byte is read n is >0, so
	   	   	   	   	   	   	   	   	 //  it continues until flushed
    return;
}

#endif

