#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<linux/i2c-dev.h>
#include<linux/i2c.h>
#include<i2c/smbus.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ioctl.h>
#include<fcntl.h>

int i2cFile;

int i2cOpen(){
	i2cFile = open("/dev/i2c-2", O_RDWR);
	if(i2cFile < 0) {
		perror("i2cOpen");
		exit(1);
	} else {
		return 1;
	}
}

void i2cClose() { close(i2cFile); };

void i2cSetAddress(int address) {
	if(ioctl(i2cFile, I2C_SLAVE, address) < 0) {
		perror("i2cSetAddress");
		exit(1);
	}
}

int i2cget(unsigned int file, unsigned int address, unsigned int daddress, unsigned int *data)
{
	int res;
	*data = 0;
	if (daddress > 0xFF) {
		printf("Error: Data address invalid!\n");
		return -1;
	}
	res = i2c_smbus_read_byte_data(file, daddress);
	close(file);
	if (res < 0) {
	fprintf(stderr, "Error: I2C Read failed\n");
		return -4;
	}
	*data = res;
	return 0;
}

int main(){
   char readBuffer[256];
   int *res1;
   printf("Starting the test application\n");
   i2cOpen();
   i2cSetAddress(0x77);
   readBuffer[1] = i2c_smbus_read_byte_data(i2cFile, 0xD0);
   printf("The read buffer is: 0x%02x\n", readBuffer[1]);
   i2cClose();
   return 0;
}

