/*
 * global.h
 *
 *  Created on: Apr 13, 2023
 *      Author: user
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "stm32f1xx_hal.h"

typedef struct Data_Sys_Package{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t week_day;
	uint8_t alarm_hour;
	uint8_t alarm_min;
	uint8_t alarm_flag;

	float temp;
	float humid;
}Data_Sys_Package;

extern Data_Sys_Package _data_sys;

extern uint8_t _counter_time_elapsed;
extern uint16_t _time_screen, _time_read_DHT, _time_out;


#endif /* INC_GLOBAL_H_ */
