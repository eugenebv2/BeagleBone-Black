#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <time.h>
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
*************************************************************
static void EEPROM_writePage(unsigned int adr, void *data, unsigned int size){
	unsigned int cmd[EEPROM_ADDRESS_SIZE + EEPROM_PAGE_SIZE]; 

	cmd[0] = adr >> 8;
	cmd[1] = (unsigned int)adr;
					
	if(size > EEPROM_PAGE_SIZE){
		printf("[EEPROM_writePage], ERROR: Invalid/Too big data size, LEN: %hu\r\n", size);
		return; // Error, data size is too big!
	}

	memcpy(cmd + 2, data, size); // prepare payload i.e. memory address + data
	I2C_EEPROM_write(cmd, size + 2); // perform write transaction
}*/

/****************************************************************
 * Function Name: EEPROM_write
 * Description: Writes to the EEPROM
 * Returns: NONE.
 * Params	@adr: Address to write 
 *		@data: write buffer
 *     		@len: No. of bytes to write 
**********************************************************
void EEPROM_write(unsigned int adr, void *data, unsigned int size) {	
	unsigned int wlen = 0;		// write length
	unsigned int dlen = size;	// data length remaining
	unsigned int addr_ofst = 0;	// address to write

	while(dlen > 0){
		wlen = ((dlen > EEPROM_PAGE_SIZE) ? EEPROM_PAGE_SIZE : dlen);
	// if addr is not at the start of a page then write bytes until we hit the boundary
		if(addr_ofst == 0 && (adr % EEPROM_PAGES_COUNT) != 0){
			wlen = EEPROM_PAGE_SIZE - (adr % EEPROM_PAGE_SIZE);
			if(size < wlen){// check we have enough data to hit end of page if not just write the entire length
				wlen = size;	
			}
		}

	// printf("Write: Addr:%hu Write: %hu Remain: %hu next: %hu\r\n", adr + addr_ofst,  wlen, dlen - wlen , addr_ofst + wlen);
	// write in blocks of %page_size%
		EEPROM_writePage(adr + addr_ofst, data + addr_ofst, wlen);
		dlen -= wlen;
		addr_ofst += wlen;
	}//while
}*/
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
	printf("0x%x, 0x%x",cmd[0],cmd[1]);
	write(f, cmd, 2); // write address
	read(f, data, size); // read operation
}

/****************************************************************
 * Function Name: EEPROM_erase
 * Description: Erase the entire EEPROM
 * Returns: NONE.
 * Params: NONE.
****************************************************************
void EEPROM_erase(void){
   	unsigned int zeroArray[EEPROM_PAGE_SIZE] = {0};
      	for(unsigned int page = 0; page < EEPROM_PAGE_NUM; page++) {
       	    EEPROM_write(page*EEPROM_PAGE_SIZE, zeroArray, EEPROM_PAGE_SIZE);
        }
}
*/

int main(void)
{
       	int f,i=0;
	int n=0;
	char buf[11];

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
		printf("\n i2c device open");
	}
	delay(2);// delay of one second

	unsigned int temp_adr = 32 * EEPROM_PAGE_SIZE * 8;
	unsigned int cmd[EEPROM_ADDRESS_SIZE] = {temp_adr >> 8, temp_adr};

 	buf[0] = cmd[0]; // starting address of eeprom
	buf[1] = cmd[1]; // starting address  
	buf[2] = 0x48; // 0x30 will be stored in location 0x51
	buf[3] = 0x45; // 0x31 will be stored in location 0x52
	buf[4] = 0x4C; // 0x32 will be stored in location 0x53
	buf[5] = 0x4C; // 0x33 will be stored in location 0x54
	buf[6] = 0x4F; // 0x34 will be stored in location 0x55
	buf[7] = 0x40; // 0x35 will be stored in location 0x56
	buf[8] = 0x45; // 0x36 will be stored in location 0x57
	buf[9] = 0x55; // 0x37 will be stored in location 0x58
	buf[10] = 0x47;

	// writing data to memory location 0x50 onwards.
 	n = write(f, buf, 11);
	if (n == -1) {
		perror("\n 63 write");
		return 1;
	} else {
		printf("\n write 10 buf");
	}
	delay(2);   // delay of one second

	//========================
	// read
	//===============
	/* Set the 8-bit address to read (0x50) */
	//buf[0] = 0x50;	/* address byte 1 */
	//buf[1] = 0x00;

	//n = write(f, buf,2);
	//if (n == -1) {
//		perror("\n 75 write");
//		return 1;
//	}
//	delay(1);   // delay of one second
	// clear buffer
	for (i=0;i<11;i++)
	{
		buf[i]= 0;
	}
	/* Now read 9 bytes from memory location 0x50 address */
//	n = read(f, buf, 9);
//	if (n == -1) {
//		perror("read");
//		return 1;
//	}
	EEPROM_read(f, 16384, buf, 9);

	printf("\nReading ..\n");
	printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",buf[0], buf[1], buf[2], buf[3],buf[4],buf[5],buf[6],buf[7],buf[8]);
	close(f);
	return 0;
}
