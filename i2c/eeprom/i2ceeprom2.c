#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <string.h>
#include "M24256.h"

void delay(int number_of_seconds)
{
	// Converting time into milli_seconds
	int milli_seconds = 1000 * number_of_seconds;
	   
	// Storing start time
	clock_t start_time = clock();
	    
	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds)       ;
}
/* Address of the EEPROM on the BeagleBone Black board */
#define I2C_ADDRESS 0x50

/****************************************************************
 * Function Name: EEPROM_writePage
 * Description: Writes to the EEPROM
 * Returns: NONE.
 * Params	@adr: Address to write 
 *		@data: write buffer
 *     		@len: No. of bytes to write 
*************************************************************/
static void EEPROM_writePage(int f, unsigned int page, void *data, unsigned int size){
	unsigned adr = 0;
	adr = page * EEPROM_PAGE_SIZE * 8 ;
	unsigned int cmd[EEPROM_ADDRESS_SIZE + EEPROM_PAGE_SIZE]; 
	cmd[0] = adr >> 8;
	cmd[1] = (unsigned int)adr;
					
	if(size > EEPROM_PAGE_SIZE){
		printf("[EEPROM_writePage], ERROR: Invalid/Too big data size, LEN: %hu\r\n", size);
		return; // Error, data size is too big!
	}
	memcpy(cmd + 2 , data, size); // prepare payload i.e. memory address + data
	write(f, cmd, size + 2); // perform write transaction
	for ( int i = 0 ; i < size + 2; i++) {
		printf("0x%2x ", cmd[i]);
	}
	printf("\n");
}

/****************************************************************
 * Function Name: EEPROM_write
 * Description: Writes to the EEPROM
 * Returns: NONE.
 * Params	@adr: Address to write 
 *		@data: write buffer
 *     		@len: No. of bytes to write 
**********************************************************/
void EEPROM_write(int f, unsigned int adr, void *data, unsigned int size) {	
	unsigned int cmd[EEPROM_ADDRESS_SIZE] = {adr >> 8, (unsigned int )adr};
	printf("write address: 0x%x, 0x%x \n",cmd[0],cmd[1]);
	write(f, cmd , 2);
	write(f, data, size);
}
/****************************************************************
 * Function Name: EEPROM_read
 * Description: Reads from the EEPROM
 * Returns: NONE.
 * Params	@adr: Address to read 
 		@data: output buffer
 *  		@len: No. of bytes to read 
****************************************************************/
void EEPROM_read(int f, unsigned int adr, void *data, unsigned int size) {
	unsigned int cmd[EEPROM_ADDRESS_SIZE] = {adr >> 8, (unsigned int )adr};
	printf("read address: 0x%x, 0x%x \n",cmd[0],cmd[1]);
	write(f, cmd, 2); // write address
	read(f, data, size); // read operation
}
void EEPROM_readPage(int f, unsigned int page, void *data, unsigned int size) {
	unsigned adr = page * EEPROM_PAGE_SIZE * 8 ;
	unsigned int cmd[EEPROM_ADDRESS_SIZE] = {adr >> 8, (unsigned int )adr};
	printf("read address: 0x%x, 0x%x \n",cmd[0],cmd[1]);
	write(f, cmd, 2); // write address
	read(f, data, size); // read operation
}

/****************************************************************
 * Function Name: EEPROM_erase
 * Description: Erase the entire EEPROM
 * Returns: NONE.
 * Params: NONE.
****************************************************************/
void EEPROM_erase(int f){
   	unsigned int zeroArray[EEPROM_PAGE_SIZE] = {0};
	unsigned int cmd[EEPROM_ADDRESS_SIZE + EEPROM_PAGE_SIZE]; 
	unsigned adr = 0;
      	for(unsigned int page = 0; page < EEPROM_PAGE_NUM; page++) {
	    adr = page * EEPROM_PAGE_SIZE * 8 ;
	    cmd[0] = adr >> 8;
	    cmd[1] = (unsigned int) adr;
	    printf("erace address: 0x%x, 0x%x \n",cmd[0],cmd[1]);
	    memcpy ( cmd + 2, zeroArray, EEPROM_PAGE_SIZE );
       	    write(f, cmd, EEPROM_PAGE_SIZE + 2);
        }
}
void EEPROM_erasePage(int f, unsigned int page){
	unsigned adr = 0;
	adr = page * EEPROM_PAGE_SIZE * 8 ;
   	unsigned int zeroArray[EEPROM_PAGE_SIZE] = {0};
	unsigned int cmd[EEPROM_ADDRESS_SIZE + EEPROM_PAGE_SIZE]; 
	cmd[0] = adr >> 8;
	cmd[1] = (unsigned int) adr;
	printf("erace address: 0x%x, 0x%x \n",cmd[0],cmd[1]);
	memcpy ( cmd + 2, zeroArray, EEPROM_PAGE_SIZE );
       	write(f,cmd ,EEPROM_PAGE_SIZE + 2);
}

int main(void)
{
       	int f,i=0;
	int n=0;
	char buf[EEPROM_PAGE_SIZE];

   	/* Open the adapter and set the address of the I2C device */
	f = open("/dev/i2c-2", O_RDWR);
	if (f < 0) {
		perror("/dev/i2c-2:");
		return 1;
	}

	if (ioctl(f, I2C_SLAVE, I2C_ADDRESS) == -1) {
		perror("ioctl I2C_SLAVE");
		return 1;
	} else {
		printf("\n i2c device open\n");
	}
	delay(2);// delay of one second

	EEPROM_erasePage(f, 32);
	EEPROM_readPage(f, 32, buf, 9);
	printf("\nReading ..\n");
	printf("0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x\n\n",buf[0], buf[1], buf[2], buf[3],buf[4],buf[5],buf[6],buf[7],buf[8]);
	memset(buf, 0, sizeof(buf));
	delay(2);   // delay of one second

	buf[0] = 0x50; // 0x30 will be stored in location 0x51
	buf[1] = 0x51; // 0x31 will be stored in location 0x52
	buf[2] = 0x52; // 0x32 will be stored in location 0x53
	buf[3] = 0x53; // 0x33 will be stored in location 0x54
	buf[4] = 0x54; // 0x34 will be stored in location 0x55
	buf[5] = 0x55; // 0x35 will be stored in location 0x56
	buf[6] = 0x56; // 0x36 will be stored in location 0x57
	buf[7] = 0x57; // 0x37 will be stored in location 0x58
	buf[8] = 0x58;

	EEPROM_writePage(f, 32, buf, 9);	
	memset(buf, 0, sizeof(buf));
	delay(2);   // delay of one second

	EEPROM_readPage(f, 32, buf, 9);
	printf("\nReading ..\n");
	printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n\n",buf[0], buf[1], buf[2], buf[3],buf[4],buf[5],buf[6],buf[7],buf[8]);
	close(f);
	return 0;
}

