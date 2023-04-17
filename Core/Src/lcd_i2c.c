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
//===========CUSTOM FOR BIG DIGIT========//
uint8_t LT[8] = {
		0b00111,
		0b01111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111
};

uint8_t UB[8] = {
		0b11111,
		0b11111,
		0b11111,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
};

uint8_t RT[8] = {
		0b11100,
		0b11110,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111
};

uint8_t LL[8] = {
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b01111,
		0b00111
};

uint8_t LB[8] = {
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b11111,
		0b11111,
		0b11111
};

uint8_t LR[8] = {
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11110,
		0b11100
};

uint8_t MB[8] = {
		0b11111,
		0b11111,
		0b11111,
		0b00000,
		0b00000,
		0b00000,
		0b11111,
		0b11111
};


//===========CUSTOM FOR ICON========//
uint8_t alarm_icon[8] = {
		0b00100,
		0b01110,
		0b01110,
		0b01110,
		0b11111,
		0b00000,
		0b00100,
		0b00000
};

uint8_t temp_icon[8] = {
		0b00000,
		0b00000,
		0b10101,
		0b01110,
		0b11011,
		0b01110,
		0b10101,
		0b00000
};

uint8_t humi_icon[8] = {
		0b00100,
		0b00100,
		0b01010,
		0b01010,
		0b10001,
		0b10001,
		0b10001,
		0b01110
};


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

void CLCD_PrintCharBuffer(uint8_t row, uint8_t col, char c){
	currentRow = row % LCD.ROW;
	currentCol = col % LCD.COL;
	Lcd_buffer[currentRow * LCD.COL + currentCol] = c;
}

void CLCD_PrintStringBuffer(uint8_t row, uint8_t col, char* str){
	currentRow = row % LCD.ROW;
	currentCol = col % LCD.COL;
	while(*str){
		UpdateCharBuffer(*str++);
	}
}

void CLCD_PrintNumBuffer(uint8_t row, uint8_t col, int32_t num){
    char flag_num = 0;
    unsigned char i;
    unsigned long power_of_10 = 1000000000;
    currentRow = row % 2;
    currentCol = col % 16;

    if(num < 0) {
        num *= -1;
        UpdateCharBuffer('-');
    }
    else if(num == 0){
    	UpdateCharBuffer('0');
    	UpdateCharBuffer('0');
//        UpdateCharBuffer(' ');
//        UpdateCharBuffer(' ');
        return;
    }

    else if(num < 10){
        UpdateCharBuffer('0');
    }

    for(i = 10; i > 0; i--){
        if( (num / power_of_10) != 0){
            flag_num = 1;
            UpdateCharBuffer(num / power_of_10 + '0');
        }
        else{
            if(flag_num != 0) UpdateCharBuffer('0');
        }
        num %= power_of_10;
        power_of_10 /= 10;
    }

//    UpdateCharBuffer(' ');
//    UpdateCharBuffer(' ');
}


void CLCD_PrintFloatBuffer(uint8_t row, uint8_t col, float f){
	uint8_t integer_part, decimal_part;
    currentRow = row % 2;
    currentCol = col % 16;

    integer_part = (uint8_t) f;
    decimal_part = (uint8_t) ((f - integer_part)*10);

    if(integer_part > 99){
    	UpdateCharBuffer(integer_part / 100 + '0');
    }


    UpdateCharBuffer((integer_part % 100) / 10 + '0');
    UpdateCharBuffer((integer_part % 100) % 10 + '0');

    UpdateCharBuffer('.');
    UpdateCharBuffer(decimal_part % 10 + '0');
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

//=============CUSTOM CHAR=============//
void CLCD_CreateChar(uint8_t addr, uint8_t* custom_arr){
	uint8_t i;

	WriteI2C(LCD_SETCGRAMADDR + 8*addr, LCD_COMMAND);

	for(i = 0; i < 8; i++){
		WriteI2C(custom_arr[i], LCD_DATA);
	}
}

void CLCD_InitBigDigit(void){
	CLCD_CreateChar(0, LT);
	CLCD_CreateChar(1, UB);
	CLCD_CreateChar(2, RT);
	CLCD_CreateChar(3, LL);
	CLCD_CreateChar(4, LB);
	CLCD_CreateChar(5, LR);
	CLCD_CreateChar(6, MB);
}

void CLCD_PrintBigDigitBuffer(uint8_t col, int8_t digit){
	currentRow = 0;
	currentCol = col % 16;

	switch(digit){
	case 0:
		UpdateCharBuffer(0);
		UpdateCharBuffer(1);
		UpdateCharBuffer(2);

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(3);
		UpdateCharBuffer(4);
		UpdateCharBuffer(5);
		break;
	case 1:
		UpdateCharBuffer(1);
		UpdateCharBuffer(2);
		UpdateCharBuffer(' ');

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(4);
		UpdateCharBuffer(0xFF);
		UpdateCharBuffer(4);
		break;
	case 2:
		UpdateCharBuffer(6);
		UpdateCharBuffer(6);
		UpdateCharBuffer(2);

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(3);
		UpdateCharBuffer(4);
		UpdateCharBuffer(4);
		break;
	case 3:
		UpdateCharBuffer(6);
		UpdateCharBuffer(6);
		UpdateCharBuffer(2);

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(4);
		UpdateCharBuffer(4);
		UpdateCharBuffer(5);
		break;
	case 4:
		UpdateCharBuffer(3);
		UpdateCharBuffer(4);
		UpdateCharBuffer(2);

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(' ');
		UpdateCharBuffer(' ');
		UpdateCharBuffer(0xFF);
		break;
	case 5:
		UpdateCharBuffer(3);
		UpdateCharBuffer(6);
		UpdateCharBuffer(6);

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(4);
		UpdateCharBuffer(4);
		UpdateCharBuffer(5);
		break;
	case 6:
		UpdateCharBuffer(0);
		UpdateCharBuffer(6);
		UpdateCharBuffer(6);

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(3);
		UpdateCharBuffer(4);
		UpdateCharBuffer(5);
		break;
	case 7:
		UpdateCharBuffer(1);
		UpdateCharBuffer(1);
		UpdateCharBuffer(2);

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(' ');
		UpdateCharBuffer(' ');
		UpdateCharBuffer(5);
		break;
	case 8:
		UpdateCharBuffer(0);
		UpdateCharBuffer(6);
		UpdateCharBuffer(2);

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(3);
		UpdateCharBuffer(4);
		UpdateCharBuffer(5);
		break;
	case 9:
		UpdateCharBuffer(0);
		UpdateCharBuffer(6);
		UpdateCharBuffer(2);

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(4);
		UpdateCharBuffer(4);
		UpdateCharBuffer(5);
		break;
	default:
		UpdateCharBuffer(' ');
		UpdateCharBuffer(' ');
		UpdateCharBuffer(' ');

		currentRow = 1;
		currentCol = col % 16;

		UpdateCharBuffer(' ');
		UpdateCharBuffer(' ');
		UpdateCharBuffer(' ');
	}
}

void CLCD_PrintBigNumBuffer(uint8_t col, int8_t number){
	if(number < 99) {
		CLCD_PrintBigDigitBuffer(col, number / 10);
		CLCD_PrintBigDigitBuffer(col + 3, number % 10);
	}
	else {
		CLCD_PrintBigDigitBuffer(col, ' ');
		CLCD_PrintBigDigitBuffer(col + 3, ' ');

	}

}

























