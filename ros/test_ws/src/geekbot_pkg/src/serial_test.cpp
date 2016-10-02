#include <stdio.h>

#include "easy_serial.h"


int main(int argc, char **argv)
{
            int arduino = serial_port_init("/dev/ttyUSB0", 57600);
                if(arduino < 1) return 1;
                    sleep(2);
                        unsigned char handshake = 0x77;
                        unsigned char temp = 0;
			int n;
                        while(temp != handshake)
			{	
				n = read(arduino, &temp, 1);
                            	if(n)
                                    printf("read %d bytes, temp =  %0x\n", n, temp);
			}
                            printf("Handshake received!\n");
                            write(arduino, &handshake, sizeof(handshake));
                                sleep(2);
                                    write(arduino, &handshake, sizeof(handshake));
                                    write(arduino, &handshake, sizeof(handshake));
                                    write(arduino, &handshake, sizeof(handshake));
                                        sleep(2);
                                            close(arduino);
                                                return 0;
}


