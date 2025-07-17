#define EEPROM_ADDRESS 			0x50  // 0x51, Chip enable address = (001)--->(E2 E1 E0)
#define EEPROM_PAGE_SIZE		64   // Page Size = 128 bytes
#define EEPROM_PAGES_COUNT		512	  // Number of pages
#define EEPROM_SIZE			32768 // EEPROM Size 256 (1024*(256/8)) Bytes
#define EEPROM_ADDRESS_SIZE		2 // Address length
#define EEPROM_PAGE_NUM			EEPROM_SIZE/EEPROM_PAGE_SIZE // Page No.
