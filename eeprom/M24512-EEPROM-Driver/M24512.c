/*
 * @file   M24512.c
 * @brief  M24512 EEPROM Driver
 * @author Vinay Divakar 
 * Known Bugs: None.
 *
 * Future Features: 
	- Consider making writes non-blocking with recursive callbacks as needed
	 if practical without complicating applications using this.
	 
 * NOTES:
	- No end of EEPROM sanity checks has been added for writes/reads
  Reference: https://os.mbed.com/users/dsMartyn/code/M24512/docs/tip/
 */

/*
 *====================
 * Includes
 *====================
 */
#include "M24512.h"
#include <string.h>
/****************************************************************
 * Function Name: EEPROM_writePage
 * Description: Writes to the EEPROM
 * Returns: NONE.
 * Params	@adr: Address to write 
			@data: write buffer
			@len: No. of bytes to write 
 ****************************************************************/
static void EEPROM_writePage(uint16_t adr, void *data, uint16_t size){
	uint8_t cmd[EEPROM_ADDRESS_SIZE + EEPROM_PAGE_SIZE]; 
	
	cmd[0] = adr >> 8;
	cmd[1] = (uint8_t)adr;
		
	if(size > EEPROM_PAGE_SIZE){
		printf("[EEPROM_writePage], ERROR: Invalid/Too big data size, LEN: %hu\r\n", size);
		return; // Error, data size is too big!
	}
	
	memcpy(cmd + 2, data, size); // prepare payload i.e. memory address + data
	I2C_EEPROM_write(cmd, size + 2); // perform write transaction
}

/****************************************************************
 * Function Name: EEPROM_write
 * Description: Writes to the EEPROM
 * Returns: NONE.
 * Params	@adr: Address to write 
			@data: write buffer
			@len: No. of bytes to write 
 ****************************************************************/
void EEPROM_write(uint16_t adr, void *data, uint16_t size) {	
	uint16_t wlen = 0;		// write length
	uint16_t dlen = size;	// data length remaining
	uint16_t addr_ofst = 0;	// address to write
	
	while(dlen > 0){
		wlen = ((dlen > EEPROM_PAGE_SIZE) ? EEPROM_PAGE_SIZE : dlen);
		// if addr is not at the start of a page then write bytes until we hit the boundary
		if(addr_ofst == 0 && (adr % EEPROM_PAGES_COUNT) != 0){
			wlen = EEPROM_PAGE_SIZE - (adr % EEPROM_PAGE_SIZE);
			if(size < wlen){// check we have enough data to hit end of page if not just write the entire length
				wlen = size;	
			}
		}
		
		//printf("Write: Addr:%hu Write: %hu Remain: %hu next: %hu\r\n", adr + addr_ofst,  wlen, dlen - wlen , addr_ofst + wlen);
		
		// write in blocks of %page_size%
		EEPROM_writePage(adr + addr_ofst, data + addr_ofst, wlen);
		dlen -= wlen;
		addr_ofst += wlen;
	}//while
}

/****************************************************************
 * Function Name: EEPROM_read
 * Description: Reads from the EEPROM
 * Returns: NONE.
 * Params	@adr: Address to read 
			@data: output buffer
			@len: No. of bytes to read 
 ****************************************************************/
void EEPROM_read(uint16_t adr, void *data, uint16_t size) {
	uint8_t cmd[EEPROM_ADDRESS_SIZE] = {adr >> 8, (uint8_t )adr};

	I2C_EEPROM_write(cmd, 2); // write address
	I2C_EEPROM_read(data, size); // read operation
}

/****************************************************************
 * Function Name: EEPROM_erase
 * Description: Erase the entire EEPROM
 * Returns: NONE.
 * Params: NONE.
 ****************************************************************/
void EEPROM_erase(void){
	   uint8_t zeroArray[EEPROM_PAGE_SIZE] = {0};
    for(uint16_t page = 0; page < EEPROM_PAGE_NUM; page++) {
	    EEPROM_write(page*EEPROM_PAGE_SIZE, zeroArray, EEPROM_PAGE_SIZE);
    }
}
