/*
 * ds130x.h
 *
 *  Created on: Apr 3, 2023
 *      Author: user
 */

#ifndef INC_DS130X_H_
#define INC_DS130X_H_

#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "main.h"

#define ADDRESS_DS130X       	0x68		//ds1307

#define ADDRESS_SECOND          0x00
#define ADDRESS_MINUTE          0x01
#define ADDRESS_HOUR            0x02
#define ADDRESS_WEEK_DAY        0x03
#define ADDRESS_DATE            0x04
#define ADDRESS_MONTH           0x05
#define ADDRESS_YEAR            0x06

#define ADDRESS_FIRST_PROGRAM   0x25
#define ADDRESS_HOUR_ALARM      0x26
#define ADDRESS_MIN_ALARM       0x27
#define ADDRESS_FLAG_ALARM      0x28

void DS_Init(I2C_HandleTypeDef* hi2c);
void DS_Write(uint8_t address, uint8_t data);
uint8_t DS_Read(uint8_t address);
void DS_SetupTimForFirstProgram(void);


#endif /* INC_DS130X_H_ */
