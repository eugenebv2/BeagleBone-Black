#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <i2c/smbus.h>
#include <sys/ioctl.h>
#include <fcntl.h>

// Small macro to display value in hexadecimal with 2 places
#define DEVID       0x00
#define BUFFER_SIZE 40

int i2cFile;

void i2cOpen(){
	i2cFile = open("/dev/i2c-1", O_RDWR);
	if(i2cFile < 0) {
		perror("i2cOpen");
		exit(1);
	}
}
void i2cClose() { close(i2cFile); };
void i2cSetAddress(int address) {
	if(ioctl(i2cFile, I2C_SLAVE, address) < 0) {
		perror("i2cSetAddress");
		exit(1);
	}
}

int main(){
   printf("Starting the test application\n");
   if((i2cFile=open("/dev/i2c-2", O_RDWR)) < 0){
      perror("failed to open the bus\n");
      return 1;
   }
   if(ioctl(i2cFile, I2C_SLAVE, 0x77) < 0){
      perror("Failed to connect to the sensor\n");
      return 1;
   }
   char writeBuffer[1] = {0xD0};
//   write(i2cFile, writeBuffer, 1);
   if(write(i2cFile, writeBuffer, 1)!=1){
      perror("Failed to reset the read address\n");
      return 1;
   }
   char readBuffer[BUFFER_SIZE];
//   read(i2cFile, readBuffer, 1);
   if(read(i2cFile, readBuffer, BUFFER_SIZE)!=BUFFER_SIZE){
      perror("Failed to read in the buffer\n");
      return 1;
   }
   printf("The Device ID is: 0x%02x\n", readBuffer[0]);
   close(i2cFile);
   return 0;
}
