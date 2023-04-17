/*
 * fsm.h
 *
 *  Created on: Apr 13, 2023
 *      Author: user
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "ds130x.h"
#include "lcd_i2c.h"
#include "stm32f1xx_hal.h"
#include "DHT.h"
#include "lcd_screen.h"
#include "input.h"
#include "global.h"
#include "output.h"

//==================MODE FSM SYSTEM CONTROL==================//
#define INIT					0
#define DISPLAY_TIME			1
#define DISPLAY_DATE_DHT		2
#define DISPLAY_SET_ALARM		3
#define SET_HOUR				4
#define SET_MIN					5
#define SET_WEEKDAY				6
#define SET_DAY					7
#define SET_MONTH				8
#define SET_YEAR				9
#define SET_ALARM				10
#define SET_ALARM_HOUR			11
#define SET_ALARM_MIN			12
#define ALARM_GO_OFF			13


//==================MODE FSM SYSCONTROL==================//
//#define INIT					0
//#define SYS_


//==================TIME==================//
#define TIME_SCREEN_TIME		1000			//10 000ms
#define TIME_SCREEN_DATE		500				//5000ms
#define TIME_READ_DHT			1000			//10 000ms
#define TIME_OUT_FOR_SET		1000			//10 000ms
#define TIME_FOR_ALARM_GO_OF	2000			//20 000ms


void FSM_Init(void);
void FSM_LcdDisplay(void);
void FSM_SystemControl(void);

#endif /* INC_FSM_H_ */
