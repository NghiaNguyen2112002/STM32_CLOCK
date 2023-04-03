/*
 * lcd_i2c.c
 *
 *  Created on: Mar 30, 2023
 *      Author: user
 */

#include "lcd_i2c.h"



uint8_t *Lcd_buffer = NULL;

CLCD_I2C_Name LCD;

uint8_t currentCol, currentRow;

static void Delay(uint16_t time){
	HAL_Delay(time);
}


static void WriteI2C(uint8_t data, uint8_t mode){
	uint8_t dataH, dataL;
	uint8_t data_I2C[4];

	dataH = data & 0xF0;
	dataL = (data << 4) & 0xF0;

	if(LCD.BACKLIGHT){
		dataH |= LCD_BACKLIGHT;
		dataL |= LCD_BACKLIGHT;
	}

	if(mode == LCD_DATA){
		dataH |= LCD_RS;
		dataL |= LCD_RS;
	}

	data_I2C[0] = dataH | LCD_EN;
	data_I2C[1] = dataH;
	data_I2C[2] = dataL | LCD_EN;
	data_I2C[3] = dataL;

	HAL_I2C_Master_Transmit(LCD.I2C, LCD.ADDRESS << 1, (uint8_t*)data_I2C, 4, 1000);
}



//===========================HIGH LEVEL FUNCTION===========================//
void CLCD_Init(I2C_HandleTypeDef* I2C, uint8_t address, uint8_t row, uint8_t col){
	uint8_t i;
	LCD.I2C = I2C;
	LCD.ADDRESS = address;
	LCD.COL = col;
	LCD.ROW = row;

	LCD.FUNCTIONSET = LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
	LCD.ENTRYMODE = LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	LCD.DISPLAYCTRL = LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LCD.CURSORSHIFT = LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT;
	LCD.BACKLIGHT = LCD_BACKLIGHT;

	Delay(50);

	if(Lcd_buffer != NULL) free(Lcd_buffer);
	Lcd_buffer = (uint8_t*)malloc(row * col * 8);

	for(i = 0; i < LCD.ROW * LCD.COL; i++) Lcd_buffer[i] = ' ';

	WriteI2C(0x33, LCD_COMMAND);
//	Delay(5);
	WriteI2C(0x33, LCD_COMMAND);
	Delay(5);
	WriteI2C(0x32, LCD_COMMAND);
	Delay(5);
	WriteI2C(0x20, LCD_COMMAND);
	Delay(5);

//	config lcd
	WriteI2C(LCD.ENTRYMODE, LCD_COMMAND);
	Delay(1);
	WriteI2C(LCD.DISPLAYCTRL, LCD_COMMAND);
	Delay(1);
	WriteI2C(LCD.CURSORSHIFT, LCD_COMMAND);
	Delay(1);
	WriteI2C(LCD.FUNCTIONSET, LCD_COMMAND);
	Delay(1);

	WriteI2C(LCD_CLEARDISPLAY, LCD_COMMAND);
	Delay(1);

	WriteI2C(LCD_RETURNHOME, LCD_COMMAND);
}

void SetCursor(uint8_t row, uint8_t col){
	uint8_t address_dram = 0x00;

	row = row % LCD.ROW;
	col = col % LCD.COL;

	switch(row){
	case 0:
		address_dram = 0x00 + col;
		break;
	case 1:
		address_dram = 0x40 + col;
		break;
	case 2:
		address_dram = 0x14 + col;
		break;
	case 3:
		address_dram = 0x54 + col;
		break;
	}

	WriteI2C(LCD_SETDDRAMADDR | address_dram, LCD_COMMAND);
}

void UpdateCharBuffer(uint8_t c){
	Lcd_buffer[currentRow * LCD.COL + currentCol] = c;
	currentCol++;
}

void CLCD_PrintCharBuffer(uint8_t row, uint8_t col, uint8_t c){
	currentRow = row % LCD.ROW;
	currentCol = col % LCD.COL;
	Lcd_buffer[currentRow * LCD.COL + currentCol] = c;
}

void CLCD_PrintStringBuffer(uint8_t row, uint8_t col, uint8_t* str){
	currentRow = row % LCD.ROW;
	currentCol = col % LCD.COL;
	while(*str){
		UpdateCharBuffer(*str++);
	}
}

void CLCD_PrintNumBuffer(uint8_t row, uint8_t col, uint16_t str){

}

void CLCD_ClearBuffer(void){
	uint8_t i;

	for(i = 0; i < LCD.ROW * LCD.COL; i++) Lcd_buffer[i] = ' ';
}

void CLCD_DisplayScreen(void){
	uint8_t i, j;

	for(i = 0; i < LCD.ROW; i++){
		SetCursor(i, 0);
		for(j = 0; j < LCD.COL; j++){
			WriteI2C(Lcd_buffer[i * LCD.COL + j], LCD_DATA);
		}
	}
}






























