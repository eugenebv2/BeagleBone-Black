#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

using namespace std;

int main(int argc, char **argv){
    int X_orientation=0;
    char buffer1[256];
    string i2cDeviceDriver="/dev/i2c-2";
    int fileHandler;

    if((fileHandler=open(i2cDeviceDriver.c_str(),O_RDWR))<0){
        perror("Failed To Open i2c-2 Bus");
        exit(1);
    }
    if(ioctl(fileHandler,I2C_SLAVE,0x77)<0){
        perror("Failed to acquire i2c bus access and talk to slave");
        exit(1);
    }

    //buffer1[0] = i2c_smbus_read_byte_data(fileHandler, 0xD0);

    char buffer[1]={0xD0};
    if(write(fileHandler,buffer,1)!=1){                 
        perror("Failed to write byte to accelerometer");
        exit(1);
    }
    if(read(fileHandler,buffer1,1)!=1){                 
        perror("Failed to read byte from accelerometer");
        exit(1);
    }
    printf("Contents of WHO AM I is 0x%02X\n",buffer1[0]);
}
