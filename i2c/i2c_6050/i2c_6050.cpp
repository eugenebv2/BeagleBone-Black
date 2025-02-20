//BBB-MPU6050.cpp
//by OP from teachmemicro.com
//

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <string>
#include <memory>
#include <array>

using namespace std;

int16_t accel_x;
int16_t accel_y;
int16_t accel_z;
int16_t gyro_x;
int16_t gyro_y;
int16_t gyro_z;

const char* deviceADDR = " 0x68";
const char* PWR_MGMT_1 = " 0x6B";
const char* ACCEL_X_OUT_H = " 0x3B ";
const char* ACCEL_X_OUT_L = " 0x3C ";
const char* ACCEL_Y_OUT_H = " 0x3D ";
const char* ACCEL_Y_OUT_L = " 0x3E ";
const char* ACCEL_Z_OUT_H = " 0x3F ";
const char* ACCEL_Z_OUT_L = " 0x40 ";
const char* GYRO_X_OUT_H = " 0x43 ";
const char* GYRO_X_OUT_L = " 0x44 ";
const char* GYRO_Y_OUT_H = " 0x45 ";
const char* GYRO_Y_OUT_L = " 0x46 ";
const char* GYRO_Z_OUT_H = " 0x47 ";
const char* GYRO_Z_OUT_L = " 0x48 ";

const char* cmdGet = "i2cget -y 2";
const char* cmdSet = "i2cset -y 2";

//*exec function that runs bash commands in c++
string exec(char* cmd) {
	string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	strcat(cmd," 2>&1");
	stream = popen(cmd, "r");

	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
			pclose(stream);
		}
	return data;
}

//*function that performs geti2c
string get(const char* reg1, const char* reg2){
	char str[100];
	string str2;

	strcpy(str, cmdGet);
	strcat(str, reg1);
	strcat(str, reg2);

	str2 = exec(str);
	return str2;
}

//*function that performs seti2c
void set(const char* reg1, const char* reg2, int value){
	char str[100];
	string str2;

	strcpy(str, cmdSet);
	strcat(str, reg1);
	strcat(str, reg2);
	strcat(str, to_string(value).c_str());

	str2 = exec(str);
}

int main(){
	set(deviceADDR, PWR_MGMT_1, 0);	//turn on the MPU6050
	while(true){
		accel_x = stoi(get(deviceADDR, ACCEL_X_OUT_H), nullptr, 16) << 8 + stoi(get(deviceADDR, ACCEL_X_OUT_L), nullptr, 16);
		accel_y = stoi(get(deviceADDR, ACCEL_Y_OUT_H), nullptr, 16) << 8 + stoi(get(deviceADDR, ACCEL_Y_OUT_L), nullptr, 16);
		accel_z = stoi(get(deviceADDR, ACCEL_Z_OUT_H), nullptr, 16) << 8 + stoi(get(deviceADDR, ACCEL_Z_OUT_L), nullptr, 16);
		accel_x = accel_x / 16384;
		accel_y = accel_y / 16384;
		accel_z = accel_z / 16384;

		gyro_x = stoi(get(deviceADDR, GYRO_X_OUT_H), nullptr, 16) << 8 + stoi(get(deviceADDR, GYRO_X_OUT_L), nullptr, 16);
		gyro_y = stoi(get(deviceADDR, GYRO_Y_OUT_H), nullptr, 16) << 8 + stoi(get(deviceADDR, GYRO_Y_OUT_L), nullptr, 16);
		gyro_z = stoi(get(deviceADDR, GYRO_Z_OUT_H), nullptr, 16) << 8 + stoi(get(deviceADDR, GYRO_Z_OUT_L), nullptr, 16);
		gyro_x = gyro_x / 131;
		gyro_y = gyro_y / 131;
		gyro_z = gyro_z / 131;

		cout << "X-acc: " << accel_x << " Y-acc: " << accel_y << " Z-acc: " << accel_z << endl;
		cout << "X-gyro: " << gyro_x << " Y-gyro: " << gyro_y << " Z-gyro: " << gyro_z << endl;

	}
	return 0;
}
