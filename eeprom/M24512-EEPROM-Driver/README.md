# M24512-EEPROM-Driver

The main purpose of this driver is to make EEPROM memory reading and writing in applications agnostic to the page boundaries.
This means that reading or writing large buffers from any starting address is easier for the program as it's not required to know the page size.
It is possible to read and write the full size of the EEPROM in one call as it handles the page counting internally.

The driver has been written for the AVR controllers. However, it can be easily ported to work with other controllers.
