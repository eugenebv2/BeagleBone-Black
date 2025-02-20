/*
 * M24512.h
 *
 * Author: Vinay Divakar
 * Purpose: M24512 EEPROM Driver
 */
#ifndef M24512_H_
#define M24512_H_

// Dependancies
#include <atmel_start.h>
#include <i2c_simple_master.h>

#define EEPROM_ADDRESS 			0x51  // 0x51, Chip enable address = (001)--->(E2 E1 E0)
#define EEPROM_PAGE_SIZE		128   // Page Size = 128 bytes
#define EEPROM_PAGES_COUNT		512	  // Number of pages
#define EEPROM_SIZE				65536 // EEPROM Size 512Kbit (1024*(512/8)) Bytes
#define EEPROM_ADDRESS_SIZE		2 // Address length
#define EEPROM_PAGE_NUM			EEPROM_SIZE/EEPROM_PAGE_SIZE // Page No.

// Links to the I2C driver API's in the HAL
#define I2C_EEPROM_write(data, len)				I2C_writeNBytes(EEPROM_ADDRESS, data, len)
#define I2C_EEPROM_read(data, len)				I2C_readNBytes(EEPROM_ADDRESS, data, len)

// Function Prototypes
void EEPROM_read(uint16_t adr, void *data, uint16_t size);
void EEPROM_write(uint16_t adr, void *data, uint16_t size);
void EEPROM_erase(void);

#endif /* M24512_H_ */


