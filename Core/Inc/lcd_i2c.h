/*
 * lcd_i2c.h
 *
 *  Created on: Mar 30, 2023
 *      Author: user
 */

#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "main.h"

#define LCD_COMMAND 			0
#define LCD_DATA				1

#define LCD_EN 					0x04  // Enable bit on
#define LCD_RW 					0x02  // Read/Write bit on
#define LCD_RS 					0x01  // Register select bit on

// commands
#define LCD_CLEARDISPLAY 		0x01
#define LCD_RETURNHOME 			0x02

// commands for config lcd
#define LCD_ENTRYMODESET 		0x04
#define LCD_DISPLAYCONTROL 		0x08
#define LCD_CURSORSHIFT 		0x10
#define LCD_FUNCTIONSET 		0x20
#define LCD_SETCGRAMADDR 		0x40
#define LCD_SETDDRAMADDR	 	0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 			0x00
#define LCD_ENTRYLEFT 			0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 			0x04
#define LCD_DISPLAYOFF 			0x00
#define LCD_CURSORON 			0x02
#define LCD_CURSOROFF 			0x00
#define LCD_BLINKON 			0x01
#define LCD_BLINKOFF 			0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 		0x08
#define LCD_CURSORMOVE 			0x00
#define LCD_MOVERIGHT 			0x04
#define LCD_MOVELEFT 			0x00

// flags for function set
#define LCD_8BITMODE 			0x10
#define LCD_4BITMODE 			0x00
#define LCD_2LINE 				0x08
#define LCD_1LINE 				0x00
#define LCD_5x10DOTS 			0x04
#define LCD_5x8DOTS 			0x00

#define LCD_BACKLIGHT			0x08  // Back light bit on
#define LCD_NOBACKLIGHT			0x00  // Back light off

//==============NOTE FOR CUSTOM NUM==============//
#define BIG_BLANK				100


typedef struct{
	I2C_HandleTypeDef* I2C;
	uint8_t ADDRESS;
	uint8_t COL;
	uint8_t ROW;
	uint8_t ENTRYMODE;
	uint8_t FUNCTIONSET;
	uint8_t DISPLAYCTRL;
	uint8_t CURSORSHIFT;
	uint8_t BACKLIGHT;
}CLCD_I2C_Name;



void CLCD_Init(I2C_HandleTypeDef* I2C, uint8_t address, uint8_t row, uint8_t col);

void CLCD_PrintCharBuffer(uint8_t row, uint8_t col, char c);
void CLCD_PrintStringBuffer(uint8_t row, uint8_t col, char* str);
void CLCD_PrintNumBuffer(uint8_t row, uint8_t col, int32_t num);
void CLCD_PrintFloatBuffer(uint8_t row, uint8_t col, float f);

void CLCD_ClearBuffer(void);
void CLCD_DisplayScreen(void);

//=============CUSTOM CHAR=============//
void CLCD_InitBigDigit(void);
void CLCD_CreateChar(uint8_t addr, uint8_t* custom_arr);
void CLCD_PrintBigDigitBuffer(uint8_t col, int8_t digit);
void CLCD_PrintBigNumBuffer(uint8_t col, int8_t number);

//==========CUSTOM ICON ADDRESS========//
extern uint8_t alarm_icon[];
extern uint8_t temp_icon[];
extern uint8_t humi_icon[];



#endif /* INC_LCD_I2C_H_ */
