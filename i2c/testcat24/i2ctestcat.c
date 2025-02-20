#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h> // htons
#include "I2C.h"

I2C_DeviceT i2c_dev;

void writeEEPROM(int fd, char* buf, int len);
char readEEPROM(int fd, int add);

void main()
{
	int res, i;
	char buf[256];
	ushort add;

	init_i2c_dev(I2C_DEV2_PATH, 0x50);

	res = Open_device( I2C_DEV2_PATH, &i2c_dev.fd_i2c);
	printf("open i2c path: %s\n", I2C_DEV2_PATH);

	res = Set_slave_addr( i2c_dev.fd_i2c, 0x50);
	printf("open slave: 0x%02x\n", 0x50);

	add = htons(64);

	i2c_multiple_writes( i2c_dev.fd_i2c, 2, &add);

	for( i = 0; i < 255 ; i++){
	   i2c_read( i2c_dev.fd_i2c, &buf[i]);
	}

	int j = 0;
	for( i = 0; i< 255; i++){
	   j++;
	   printf("%02x ", buf[i]);
	   if( j > 16 )
	   {
		   printf("\n");
		   j = 0;
	   }
	}
	printf("\n");

}

void writeEEPROM(int fd, char* buf, int len)
{
	i2c_multiple_writes( fd, len, buf);
}

char readEEPROM(int fd, int add)
{
	char buf[4];
	char data = 0xFF;
	buf[0] = add >> 8;
	buf[1] = add & 0xff;
	i2c_multiple_writes( fd, 2, buf);
	data = i2c_read(fd, &data);
	return data;
}
