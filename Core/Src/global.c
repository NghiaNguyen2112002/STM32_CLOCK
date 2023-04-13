/*
 * global.c
 *
 *  Created on: Apr 13, 2023
 *      Author: user
 */


#include "global.h"

Data_Sys_Package _data_sys = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uint8_t _counter_time_elapsed = 0;
uint16_t _time_screen = 0, _time_read_DHT = 0, _time_out = 0;
