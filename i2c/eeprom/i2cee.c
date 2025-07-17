/*
 *     Simple program to write / read the eeprom 24C256.
 *     On Grove Base Cape for BBB.
 */

#include <stdio.h>
#include <sys/ioctl.h> // ioctl
#include <fcntl.h>     // open
#include <unistd.h>    // read/write usleep
#include <time.h>
#include <netinet/in.h> // htons
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

// #pragma pack(1)
#define PAGESIZE 64
#define NPAGES  512
#define NBYTES (NPAGES*PAGESIZE)
#define ADDRESS 0x50  //  CAT24C256's address on I2C bus 

typedef struct {
	ushort AW;
	char  buf[PAGESIZE+2];
}WRITE;

static WRITE AT = {0};

int main() {
	int fd;
	char bufIN[180] = {0};
	time_t clock=time(NULL);
        snprintf(AT.buf, PAGESIZE+1, "%s: def", ctime(&clock)); 

	//  the buffer to write, cut to 64 bytes
	printf("init buf info\n");
	printf("AT: %s\n", AT.buf);
	printf("\nbufIN: %s\n", bufIN);

	if ((fd = open("/dev/i2c-2", O_RDWR)) < 0) {  
		printf("Couldn't open device! %d\n", fd); 
		return 1; 
	}
	printf("i2c-2 connected\n");

	if (ioctl(fd, I2C_SLAVE, ADDRESS) < 0) { 
		printf("Couldn't find device on address!\n"); 
		return 1; 
	}
	printf("device 0x%02x connected\n", ADDRESS);

	//  I will write to start from byte 0 of page 0 ( 64nd byte of eeprom )
	AT.AW = htons(32704);    
	if (write(fd, &AT, PAGESIZE+2) != (PAGESIZE+2)) { 
		perror("Write error !");    
		return 1; 
	}
	printf("write to device\n");
	while (1) { 
		char ap[4];  
		if (read(fd,&ap,1) != 1) usleep(500); else break; 
	} 
	//   wait on write's end 
	//   read from address
	if (write(fd, &AT, 2) != 2) {  
		perror("Error in sending the reading address");
	    	return 1;  
	}
	if (read(fd,bufIN,PAGESIZE) != PAGESIZE) { 
		perror("reading error\n"); return 1;
	}
	printf("read from 0x%02x device, address 0x%04x:\n", ADDRESS, AT.AW);
	printf("\n%s\n", bufIN);

	//
	AT.AW = htons(16384);    
//	if (write(fd, &AT, PAGESIZE+2) != (PAGESIZE+2)) { 
//		perror("Write error !");    
//		return 1; 
//	}
//	printf("write to device\n");
//	while (1) { 
//		char ap[4];  
//		if (read(fd,&ap,1) != 1) usleep(500); else break; 
//	}
	if (write(fd, &AT, 2) != 2) {  
		perror("Error in sending the reading address");
	    	return 1;  
	}
	if (read(fd,bufIN,PAGESIZE) != PAGESIZE) { 
		perror("reading error\n"); return 1;
	}
	printf("read from 0x%02x device, address 0x%04x:\n", ADDRESS, AT.AW);
	printf("\n%s\n", bufIN);

	close(fd);
	return 0;
}
