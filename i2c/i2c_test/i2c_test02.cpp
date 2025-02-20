#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <i2c/smbus.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

using namespace std;
const char REGISTER_ID = 0xD0;

int main()
{
int fileHandler;
string i2cDeviceDriver="/dev/i2c-2";
struct i2c_rdwr_ioctl_data readByteData;
struct i2c_msg messages[2];
readByteData.nmsgs = 2;
readByteData.msgs = messages;

if((fileHandler=open(i2cDeviceDriver.c_str(),O_RDWR))<0){
        perror("Failed To Open i2c-2 Bus");
        exit(1);
}
if(ioctl(fileHandler,I2C_SLAVE,0x77)<0){
        perror("Failed to acquire i2c bus access and talk to slave");
        exit(1);
}				    }

// Write portion (send the register we wish to read)
char request = REGISTER_ID;
i2c_msg& message = messages[0];
message.addr  = SLAVE_ID;
message.flags = 0;            // 0 = Write
message.len   = 1;
message.buf   = &request;

// Read portion (read in the value of the register)
char response;
message = messages[1];
message.addr  = SLAVE_ID;
message.flags = I2C_M_RD;
message.len   = 1;            // Number of bytes to read
message.buf   = &response;    // Where to place the result

// Submit the combined read+write message
ioctl(fileHandler, I2C_RDWR, &readByteData);

// Output the result
printf("Contents of register 0x%02x is 0x%02x\n", REGISTER_ID, response);

}
