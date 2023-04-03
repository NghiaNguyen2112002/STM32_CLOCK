/*
 * ds130x.c
 *
 *  Created on: Apr 3, 2023
 *      Author: user
 */


#include "ds130x.h"


I2C_HandleTypeDef *i2c_ds;


void DS_Init(I2C_HandleTypeDef* hi2c){
	i2c_ds = hi2c;
}


void DS_Write(unsigned char address, unsigned char data){
	uint8_t data[2] = { address, data };
	HAL_I2C_Master_Transmit(i2c_ds, ADDRESS_DS130X << 1, 2, 1000);
}

uint8_t DS_Read(uint8_t address){
	uint8_t value;
	HAL_I2C_Master_Transmit(i2c_ds, ADDRESS_DS130X << 1, &address, 1, 1000);
	HAL_I2C_Master_Receive(i2c_ds, ADDRESS_DS130X << 1, &value, 1, 1000);

	return value;
}
