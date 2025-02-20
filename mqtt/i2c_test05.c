#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>
#include <inttypes.h>
#include "Seeed_BMP280.h"
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include "I2C.h"
#include <getopt.h>
#include <sys/types.h>
#include "mosquitto.h"
#include "mqtt_userdefs.h"

#define CLIENT_ID "BBB"

void main() {
	int debug_flag = 0;
	int disp_oled = 0;
	int file;
	int adapter_nr = 2; /* probably dynamically determined */
	char filename[20];
	struct mosquitto *mosq;

	ambient_t ambient;

	char mqtt_channel_name[256];
	start_arg_t start_arg = {
		.broker_hostname = "10.68.40.210",
		.broker_port = 1883,
		.location = "BBB/bmp280"
	};

	mosquitto_lib_init();
	mosq = mosquitto_new( CLIENT_ID, true, NULL);

	if(!mosq)
	{
		printf("Error: failed to create mosquitto client\n");
	}

	mosquitto_username_pw_set( mosq, "mqtt", "1234");

	if(mosquitto_connect(mosq, start_arg.broker_hostname, start_arg.broker_port, 60) != MOSQ_ERR_SUCCESS)
	{
		printf("Error: connect to MQTT broker failed\n");
	}

	snprintf(ambient.location, sizeof(ambient.location), "%s", start_arg.location);	
	// data read from BMP
	//
	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) {
      	/* ERROR HANDLING; you can check errno to see what went wrong */
		perror("Error 1");
      		exit(1);
	}

	int addr = BMP280_ADDRESS; /* The I2C address */

	if (ioctl(file, I2C_SLAVE, addr) < 0) {
	/* ERROR HANDLING; you can check errno to see what went wrong */
		perror("Error 2");
		exit(1);
	}

	__u8 reg = BMP280_REG_CHIPID; /* Device register to access */
	__s32 res;
	char buf[10];

     	/* Using SMBus commands */
	buf[0] = i2c_smbus_read_byte_data(file, reg);
	res = i2c_smbus_read_byte_data(file, reg);
	if (res < 0) {
		perror("Error 3");
    	/* ERROR HANDLING: i2c transaction failed */
	} else {
    	/* res contains the read word */
		printf("DEVICE ID IS: 0x%02x\n", buf[0]);
		if(debug_flag == 1)
		printf("DEVICE ID IS: %d\n", res);
	}

	dig_T1 = bmp280Read16LE(file, BMP280_REG_DIG_T1);
	dig_T2 = bmp280ReadS16LE(file, BMP280_REG_DIG_T2);
	dig_T3 = bmp280ReadS16LE(file, BMP280_REG_DIG_T3);
	dig_P1 = bmp280Read16LE(file, BMP280_REG_DIG_P1);
	dig_P2 = bmp280ReadS16LE(file, BMP280_REG_DIG_P2);
	dig_P3 = bmp280ReadS16LE(file, BMP280_REG_DIG_P3);
	dig_P4 = bmp280ReadS16LE(file, BMP280_REG_DIG_P4);
	dig_P5 = bmp280ReadS16LE(file, BMP280_REG_DIG_P5);
	dig_P6 = bmp280ReadS16LE(file, BMP280_REG_DIG_P6);
	dig_P7 = bmp280ReadS16LE(file, BMP280_REG_DIG_P7);
	dig_P8 = bmp280ReadS16LE(file, BMP280_REG_DIG_P8);
	dig_P9 = bmp280ReadS16LE(file, BMP280_REG_DIG_P9);
	if( debug_flag == 1) {
	printf("dig_T1 reg: = %d\n", dig_T1);
	printf("dig_T2 reg: = %d\n", dig_T2);
	printf("dig_T3 reg: = %d\n", dig_T3);
	printf("dig_P1 reg: = %d\n", dig_P1);
	printf("dig_P2 reg: = %d\n", dig_P2);
	printf("dig_p3 reg: = %d\n", dig_P3);
	printf("dig_P4 reg: = %d\n", dig_P4);
	printf("dig_P5 reg: = %d\n", dig_P5);
	printf("dig_P6 reg: = %d\n", dig_P6);
	printf("dig_P7 reg: = %d\n", dig_P7);
	printf("dig_P8 reg: = %d\n", dig_P8);
	printf("dig_P9 reg: = %d\n", dig_P9);
	}

	i2c_smbus_write_byte_data(file, BMP280_REG_CONTROL, 0x3F);
	
	sprintf(mqtt_channel_name, "/home/%s/ambient_data", start_arg.location);

	while(1) {
	float temp_tmp = getTemperature(file);
	uint32_t temp_pre = getPressure(file);
	float temp_ati = calcAltitude(temp_pre, temp_tmp);
	printf("Temperature:  %.02f C\n", temp_tmp);
	printf("Pressure: %d Pa\n", temp_pre);
	printf("Altitude: %.02f M\n\n", temp_ati);
	ambient.temperature = temp_tmp;	
	ambient.pressure =  temp_pre;	
	ambient.altitude = temp_ati;	

	mosquitto_publish(mosq, NULL, mqtt_channel_name, sizeof(ambient_t), &ambient, MQTT_QOS_0, false);

	sleep(10);
	}
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}

float getTemperature(int file) {
	char buf[10];
  int32_t var1, var2, adc_T;
// Read Temperature value  ( 0xF7 msb/ 0xF8 lsb/ 0xF9 xlsb)
  i2c_smbus_read_i2c_block_data(file, BMP280_REG_TEMPDATA, 3, buf);
  adc_T = buf[0];
  adc_T <<= 8;
  adc_T |= buf[1];
  adc_T <<=8;
  adc_T |= buf[2];
  adc_T >>= 4;
  var1 = (((adc_T >> 3) - ((int32_t)(dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
  var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

   t_fine = var1 + var2;
   float T = (t_fine * 5 + 128 ) >> 8;
   return T / 100 ;
}

uint16_t bmp280Read16(int fd, uint8_t reg) {
	uint8_t buf[2];
	uint8_t msb, lsb;
	i2c_smbus_read_i2c_block_data(fd, reg, 2, buf);
	msb = buf[0];
	lsb = buf[1];
//	printf("reg: 0x%02x , msb: 0x%x\n", reg, msb);
//	printf("reg: 0x%02x , lsb: 0x%x\n", reg, lsb);
	return (uint16_t) msb << 8 | lsb;
}

uint16_t bmp280Read16LE(int fd, uint8_t reg) {
	  uint16_t data = bmp280Read16(fd, reg);
	    return (data >> 8) | (data << 8);
}

int16_t bmp280ReadS16(int fd, uint8_t reg) {
	  return (int16_t)bmp280Read16(fd, reg);
}

int16_t bmp280ReadS16LE(int fd, uint8_t reg) {
	  return (int16_t)bmp280Read16LE(fd, reg);
}

void another_i2ctest(){
/*
*    * Using I2C Write, equivalent of
*       * i2c_smbus_write_word_data(file, reg, 0x6543)
*
*
	buf[0] = reg;
	buf[1] = 0x43;
	buf[2] = 0x65;
        if (write(file, buf, 3) != 3) {
	// ERROR HANDLING: i2c transaction failed 
	}

	// Using I2C Read, equivalent of i2c_smbus_read_byte(file) 
	if (read(file, buf, 1) != 1) {
	// ERROR HANDLING: i2c transaction failed 
	} else {
        // buf[0] contains the read byte 
	}
*/
}

//Returns temperature in DegC, resolution is 0.01 DegC. Output value
//of ¨5123〃equals 51.23 DegC. 
//t_fine carries fine temperature as global value BMP280_S32_t t_fine;

int32_t bmp280_compensate_T_int32(int file)
{
	char buf[10];
	int32_t var1, var2, T;
	uint32_t adc_T;
// Read Temperature value  ( 0xF7 msb/ 0xF8 lsb/ 0xF9 xlsb)
	i2c_smbus_read_i2c_block_data(file, BMP280_REG_TEMPDATA, 3, buf);
	adc_T = buf[0];
	adc_T <<= 8;
	adc_T |= buf[1];
	adc_T <<=8;
	adc_T |= buf[2];
	adc_T >>= 4;
	var1 = (((adc_T >> 3) - ((int32_t)(dig_T1 << 1)))
		* ((int32_t)dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((int32_t)dig_T1))
		* ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12)
		* ((int32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

uint32_t getPressure(int file)
{
	char buf[10];
        int64_t var1, var2, p, adc_P;

// Read Pressure value  ( 0xFA msb/ 0xFB lsb/ 0xFC xlsb)
	i2c_smbus_read_i2c_block_data(file, BMP280_REG_PRESSUREDATA, 3, buf);
	adc_P = buf[0];
	adc_P <<= 8;
	adc_P |= buf[1];
	adc_P <<=8;
	adc_P |= buf[2];

	adc_P >>= 4;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
	var2 = var2 + (((int64_t)dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) +
		((var1 * (int64_t)dig_P2) << 12);
	var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
	if(var1 == 0) {
		return 0;
	}
	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t)dig_P8) * p)  >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
	return (uint32_t)p / 256;
}

//Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format
//(24 integer bits and 8 fractional bits).
//Output value of ¨24674867〃represents 24674867/256 = 96386.2 Pa
// = 963.862 hPa
//
uint32_t bmp280_compensate_P_int64(int32_t adc_P)
{
	int64_t var1, var2, p;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1*(int64_t)dig_P5) << 17);
	var2 = var2 + (((int64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) +
		((var1 * (int64_t)dig_P2) << 12);
	var1 = (((((int64_t)1) << 47)+var1))*((int64_t)dig_P1) >> 33;
	if (var1 == 0) {
		return 0;
// avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p<<31) - var2) * 3125) / var1;
	var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
	return (int32_t)p;
}
float calcAltitude( uint32_t p1, float t)
{
	float C;
	float p0 = 102009;
	C = (p0 / p1);
	C = pow(C, (1 / 5.25588)) - 1.0;
	C = (C * (t + 273.15)) / 0.0065;
	return C;
}
