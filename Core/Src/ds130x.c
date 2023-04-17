/*
 * ds130x.c
 *
 *  Created on: Apr 3, 2023
 *      Author: user
 */


#include "ds130x.h"


I2C_HandleTypeDef *i2c_ds;

uint8_t BCD2DEC(uint8_t bcd){
	uint8_t low, high;
	low = bcd & 0x0F;
	high = (bcd >> 4) * 10;

	return (low + high);
}

uint8_t DEC2BCD(uint8_t dec){
	uint8_t low, high;
	low = dec % 10;
	high = (dec / 10) << 4;

	return (low + high);
}

void DS_Init(I2C_HandleTypeDef* hi2c){
	i2c_ds = hi2c;
}


void DS_Write(uint8_t address, uint8_t data){
	uint8_t byte[2] = { address, DEC2BCD(data) };
	HAL_I2C_Master_Transmit(i2c_ds, ADDRESS_DS130X << 1, byte, 2, 1000);
}

uint8_t DS_Read(uint8_t address){
	uint8_t value = 0;
	HAL_I2C_Master_Transmit(i2c_ds, ADDRESS_DS130X << 1, &address, 1, 1000);
	HAL_I2C_Master_Receive(i2c_ds, ADDRESS_DS130X << 1, &value, 1, 1000);

	return BCD2DEC(value);
}

void DS_SetupTimForFirstProgram(void){
	if(DS_Read(ADDRESS_FIRST_PROGRAM) == 1) return;

	DS_Write(ADDRESS_HOUR, 1);
	DS_Write(ADDRESS_MINUTE, 1);
	DS_Write(ADDRESS_SECOND, 1);

	DS_Write(ADDRESS_WEEK_DAY, 1);
	DS_Write(ADDRESS_DATE, 1);
	DS_Write(ADDRESS_MONTH, 1);
	DS_Write(ADDRESS_YEAR, 1);

	DS_Write(ADDRESS_HOUR_ALARM, 1);
	DS_Write(ADDRESS_MIN_ALARM, 1);
	DS_Write(ADDRESS_FLAG_ALARM, 0);

	DS_Write(ADDRESS_FIRST_PROGRAM, 1);

}
