/*
 * fsm.c
 *
 *  Created on: Apr 13, 2023
 *      Author: user
 */

#include "fsm.h"

uint8_t mode_sys;

void ReadTimeDs1307(void){
	_data_sys.hour = DS_Read(ADDRESS_HOUR);
	_data_sys.min = DS_Read(ADDRESS_MINUTE);
	_data_sys.sec = DS_Read(ADDRESS_SECOND);
	_data_sys.week_day = DS_Read(ADDRESS_WEEK_DAY);
	_data_sys.day = DS_Read(ADDRESS_DATE);
	_data_sys.month = DS_Read(ADDRESS_MONTH);
	_data_sys.year = DS_Read(ADDRESS_YEAR);

	_data_sys.alarm_hour = DS_Read(ADDRESS_HOUR_ALARM);
	_data_sys.alarm_min = DS_Read(ADDRESS_MIN_ALARM);
	_data_sys.alarm_flag = DS_Read(ADDRESS_FLAG_ALARM);
}

char* ConvertWeekdayToStr(uint8_t x){
	switch(x){
	case 1:
		return "MON";
		break;
	case 2:
		return "TUE";
		break;
	case 3:
		return "WED";
		break;
	case 4:
		return "THU";
		break;
	case 5:
		return "FRI";
		break;
	case 6:
		return "SAT";
		break;
	case 7:
		return "SUN";
		break;
	}
	return "";
}


//===========================HIGH LEVEL FUNCTION===========================//
void FSM_Init(void){
	mode_sys = INIT;
	_time_screen = 50;
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, SET);
}

void FSM_SystemControl(void){
	switch(mode_sys){
	case INIT:
		CLCD_PrintStringBuffer(0, 0, SCREEN_INIT_0);
		CLCD_PrintStringBuffer(1, 0, SCREEN_INIT_1);

		DS_SetupTimForFirstProgram();

		if(_time_screen < 5){
			_time_screen = TIME_SCREEN_TIME;
			ReadTimeDs1307();

			CLCD_CreateChar(7, alarm_icon);
			CLCD_InitBigDigit();

			CLCD_PrintStringBuffer(0, 0, SCREEN_TIME_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_TIME_1);

			if(_data_sys.alarm_flag) CLCD_PrintCharBuffer(0, INDEX_LCD_ICON_ALARM, 7);

			mode_sys = DISPLAY_TIME;
		}
		break;
	case DISPLAY_TIME:
//		Blink COLON every 1000ms
		if(_counter_time_elapsed % 40 < 20){
			CLCD_PrintCharBuffer(0, INDEX_COLON, '.');
			CLCD_PrintCharBuffer(1, INDEX_COLON, '.');
		}
		else {
			CLCD_PrintCharBuffer(0, INDEX_COLON, ' ');
			CLCD_PrintCharBuffer(1, INDEX_COLON, ' ');
		}

//		Read time every 500ms
		if(_counter_time_elapsed % 10 == 0){
			ReadTimeDs1307();
		}

		CLCD_PrintBigNumBuffer(INDEX_LCD_HOUR, _data_sys.hour);
		CLCD_PrintBigNumBuffer(INDEX_LCD_MIN, _data_sys.min);

		CLCD_PrintNumBuffer(1, INDEX_LCD_SEC, _data_sys.sec);

//		Stick 1500ms
		if(IN_IsPressed_ms(BT_MODE, 1500)){
			_time_out = TIME_OUT_FOR_SET;
			mode_sys = SET_HOUR;
		}
		else if(IN_IsPressed_ms(BT_DEC, 1500) && IN_IsPressed_ms(BT_INC, 1500)){
			_time_out = TIME_OUT_FOR_SET;
			CLCD_PrintStringBuffer(0, 0, SCREEN_CONFIRM_SET_ALARM_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_CONFIRM_SET_ALARM_1);
			mode_sys = SET_ALARM;
		}
		else if(_data_sys.alarm_flag && (_data_sys.hour == _data_sys.alarm_hour)
				&& (_data_sys.min == _data_sys.alarm_min) && (_data_sys.sec == 0)){
			_time_out = TIME_FOR_ALARM_GO_OF;
			CLCD_PrintStringBuffer(0, 0, SCREEN_ALARM_GO_OFF_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_ALARM_GO_OFF_1);
			mode_sys = ALARM_GO_OFF;
		}
		else if(_time_screen < 5 || IN_IsPressed(BT_MODE)){
			_time_screen = TIME_SCREEN_DATE;

			CLCD_CreateChar(0, temp_icon);
			CLCD_CreateChar(1, humi_icon);

			CLCD_PrintStringBuffer(0, 0, SCREEN_DATE_DHT_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_DATE_DHT_1);
			CLCD_PrintCharBuffer(1, INDEX_CEL_SYMBOL, 0xDF);
			CLCD_PrintCharBuffer(1, INDEX_LCD_TEMP_SYMBOL, 0);
			CLCD_PrintCharBuffer(1, INDEX_LCD_HUMI_SYMBOL, 1);
			mode_sys = DISPLAY_DATE_DHT;
		}
		break;
	case DISPLAY_DATE_DHT:

		CLCD_PrintStringBuffer(0, INDEX_LCD_WEEK_DAY, ConvertWeekdayToStr(_data_sys.week_day));
		CLCD_PrintNumBuffer(0, INDEX_LCD_DAY, _data_sys.day);
		CLCD_PrintNumBuffer(0, INDEX_LCD_MONTH, _data_sys.month);
		CLCD_PrintNumBuffer(0, INDEX_LCD_YEAR, 2000 + _data_sys.year);

		CLCD_PrintFloatBuffer(1, INDEX_LCD_TEMP, _data_sys.temp);
		CLCD_PrintFloatBuffer(1, INDEX_LCD_HUMI, _data_sys.humid);

		if(_time_read_DHT < 5){
			_time_read_DHT = TIME_READ_DHT;
			DHT_Read();
			_data_sys.temp = DHT_GetTemp();
			_data_sys.humid = DHT_GetHumi();
		}

//		Stick 1500ms
		if(IN_IsPressed_ms(BT_MODE, 1500)){
			_time_out = TIME_OUT_FOR_SET;
			mode_sys = SET_WEEKDAY;
		}
		else if(IN_IsPressed_ms(BT_DEC, 1500) && IN_IsPressed_ms(BT_INC, 1500)){
			_time_out = TIME_OUT_FOR_SET;
			CLCD_PrintStringBuffer(0, 0, SCREEN_CONFIRM_SET_ALARM_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_CONFIRM_SET_ALARM_1);
			mode_sys = SET_ALARM;
		}
		else if(_data_sys.alarm_flag && (_data_sys.hour == _data_sys.alarm_hour)
				&& (_data_sys.min == _data_sys.alarm_min) && (_data_sys.sec == 0)){
			_time_out = TIME_FOR_ALARM_GO_OF;
			CLCD_PrintStringBuffer(0, 0, SCREEN_ALARM_GO_OFF_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_ALARM_GO_OFF_1);
			mode_sys = ALARM_GO_OFF;
		}
		else if(_time_screen < 5 || IN_IsPressed(BT_MODE)){
			_time_screen = TIME_SCREEN_TIME;
			ReadTimeDs1307();

			CLCD_CreateChar(7, alarm_icon);
			CLCD_InitBigDigit();

			CLCD_PrintStringBuffer(0, 0, SCREEN_TIME_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_TIME_1);

			if(_data_sys.alarm_flag) CLCD_PrintCharBuffer(0, INDEX_LCD_ICON_ALARM, 7);

			mode_sys = DISPLAY_TIME;
		}
		break;
	case SET_HOUR:
//		Blink BLink BLink :D
		if(_counter_time_elapsed % 20 < 10){
			CLCD_PrintBigNumBuffer(INDEX_LCD_HOUR, _data_sys.hour);
		}
		else {
			CLCD_PrintBigNumBuffer(INDEX_LCD_HOUR, BIG_BLANK);
		}

//		Increase/Decrease 1 every 400ms
		if(IN_Stick(BT_INC) % 8 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.hour = _data_sys.hour < 23 ? _data_sys.hour + 1 : 0;
			CLCD_PrintBigNumBuffer(INDEX_LCD_HOUR, _data_sys.hour);
		}
		else if(IN_Stick(BT_DEC) % 8 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.hour = _data_sys.hour > 0 ? _data_sys.hour - 1 : 23;
//			CLCD_PrintBigNumBuffer(INDEX_LCD_HOUR, _data_sys.hour);
		}


		if(IN_IsPressed(BT_MODE)){
			_time_out = TIME_OUT_FOR_SET;
			DS_Write(ADDRESS_HOUR, _data_sys.hour);
			CLCD_PrintBigNumBuffer(INDEX_LCD_HOUR, _data_sys.hour);
			mode_sys = SET_MIN;
		}
		else if(_time_out < 5){
			_time_screen = TIME_SCREEN_TIME;

			CLCD_PrintStringBuffer(0, 0, SCREEN_TIME_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_TIME_1);

			mode_sys = DISPLAY_TIME;
		}

		break;
	case SET_MIN:
//		Blink BLink BLink :D
		if(_counter_time_elapsed % 20 < 10){
			CLCD_PrintBigNumBuffer(INDEX_LCD_MIN, _data_sys.min);
		}
		else {
			CLCD_PrintBigNumBuffer(INDEX_LCD_MIN, BIG_BLANK);
		}

//		Increase/Decrease 1 every 500ms
		if(IN_Stick(BT_INC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.min = _data_sys.min < 59 ? _data_sys.min + 1 : 0;
			CLCD_PrintBigNumBuffer(INDEX_LCD_MIN, _data_sys.min);
		}
		else if(IN_Stick(BT_DEC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.min = _data_sys.min > 0 ? _data_sys.min - 1 : 59;
			CLCD_PrintBigNumBuffer(INDEX_LCD_MIN, _data_sys.min);
		}

		if(_time_out < 5 || IN_IsPressed(BT_MODE)){
			_time_screen = TIME_SCREEN_TIME;

			DS_Write(ADDRESS_MINUTE, _data_sys.min);

			CLCD_CreateChar(7, alarm_icon);
			CLCD_InitBigDigit();

			CLCD_PrintStringBuffer(0, 0, SCREEN_TIME_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_TIME_1);

			if(_data_sys.alarm_flag) CLCD_PrintCharBuffer(0, INDEX_LCD_ICON_ALARM, 7);

			mode_sys = DISPLAY_TIME;
		}
		break;
	case SET_WEEKDAY:
//		Blink BLink BLink :D
		if(_counter_time_elapsed % 20 < 10){
			CLCD_PrintStringBuffer(0, INDEX_LCD_WEEK_DAY, ConvertWeekdayToStr(_data_sys.week_day));
		}
		else {
			CLCD_PrintStringBuffer(0, INDEX_LCD_WEEK_DAY, "   ");
		}

//		Increase/Decrease 1 every 500ms
		if(IN_Stick(BT_INC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.week_day = _data_sys.week_day < 7 ? _data_sys.week_day + 1 : 1;
			CLCD_PrintStringBuffer(0, INDEX_LCD_WEEK_DAY, ConvertWeekdayToStr(_data_sys.week_day));
		}
		else if(IN_Stick(BT_DEC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.week_day = _data_sys.week_day > 1 ? _data_sys.week_day - 1 : 7;
			CLCD_PrintStringBuffer(0, INDEX_LCD_WEEK_DAY, ConvertWeekdayToStr(_data_sys.week_day));
		}

		if(IN_IsPressed(BT_MODE)){
			_time_out = TIME_OUT_FOR_SET;
			DS_Write(ADDRESS_WEEK_DAY, _data_sys.week_day);
			CLCD_PrintStringBuffer(0, INDEX_LCD_WEEK_DAY, ConvertWeekdayToStr(_data_sys.week_day));
			mode_sys = SET_DAY;
		}
		else if(_time_out < 5){
			_time_screen = TIME_SCREEN_DATE;

			CLCD_PrintStringBuffer(0, 0, SCREEN_DATE_DHT_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_DATE_DHT_1);
			CLCD_PrintCharBuffer(1, INDEX_CEL_SYMBOL, 0xDF);
			mode_sys = DISPLAY_DATE_DHT;
		}

		break;
	case SET_DAY:
//		Blink BLink BLink :D
		if(_counter_time_elapsed % 20 < 10){
			CLCD_PrintNumBuffer(0, INDEX_LCD_DAY, _data_sys.day);
		}
		else {
			CLCD_PrintStringBuffer(0, INDEX_LCD_DAY, "  ");
		}

//		Increase/Decrease 1 every 500ms
		if(IN_Stick(BT_INC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.day = _data_sys.day < 31 ? _data_sys.day + 1 : 1;
			CLCD_PrintNumBuffer(0, INDEX_LCD_DAY, _data_sys.day);
		}
		else if(IN_Stick(BT_DEC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.day = _data_sys.day > 1 ? _data_sys.day - 1 : 31;
			CLCD_PrintNumBuffer(0, INDEX_LCD_DAY, _data_sys.day);
		}

		if(IN_IsPressed(BT_MODE)){
			_time_out = TIME_OUT_FOR_SET;
			DS_Write(ADDRESS_DATE, _data_sys.day);
			CLCD_PrintNumBuffer(0, INDEX_LCD_DAY, _data_sys.day);
			mode_sys = SET_MONTH;
		}
		else if(_time_out < 5){
			_time_screen = TIME_SCREEN_DATE;

			CLCD_PrintStringBuffer(0, 0, SCREEN_DATE_DHT_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_DATE_DHT_1);
			CLCD_PrintCharBuffer(1, INDEX_CEL_SYMBOL, 0xDF);
			mode_sys = DISPLAY_DATE_DHT;
		}
		break;
	case SET_MONTH:
//		Blink BLink BLink :D
		if(_counter_time_elapsed % 20 < 10){
			CLCD_PrintNumBuffer(0, INDEX_LCD_MONTH, _data_sys.month);
		}
		else {
			CLCD_PrintStringBuffer(0, INDEX_LCD_MONTH, "  ");
		}

//		Increase/Decrease 1 every 500ms
		if(IN_Stick(BT_INC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.month = _data_sys.month < 12 ? _data_sys.month + 1 : 1;
			CLCD_PrintNumBuffer(0, INDEX_LCD_MONTH, _data_sys.month);

		}
		else if(IN_Stick(BT_DEC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.month = _data_sys.month > 1 ? _data_sys.month - 1 : 12;
			CLCD_PrintNumBuffer(0, INDEX_LCD_MONTH, _data_sys.month);
		}

		if(IN_IsPressed(BT_MODE)){
			_time_out = TIME_OUT_FOR_SET;
			DS_Write(ADDRESS_MONTH, _data_sys.month);
			CLCD_PrintNumBuffer(0, INDEX_LCD_MONTH, _data_sys.month);
			mode_sys = SET_YEAR;
		}
		else if(_time_out < 5){
			_time_screen = TIME_SCREEN_DATE;

			CLCD_PrintStringBuffer(0, 0, SCREEN_DATE_DHT_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_DATE_DHT_1);
			CLCD_PrintCharBuffer(1, INDEX_CEL_SYMBOL, 0xDF);
			mode_sys = DISPLAY_DATE_DHT;
		}
		break;
	case SET_YEAR:
//		Blink BLink BLink :D
		if(_counter_time_elapsed % 20 < 10){
			CLCD_PrintNumBuffer(0, INDEX_LCD_YEAR, 2000 + _data_sys.year);
		}
		else {
			CLCD_PrintStringBuffer(0, INDEX_LCD_YEAR, "    ");
		}

//		Increase/Decrease 1 every 500ms
		if(IN_Stick(BT_INC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.year = _data_sys.year < 99 ? _data_sys.year + 1 : 0;
			CLCD_PrintNumBuffer(0, INDEX_LCD_YEAR, 2000 + _data_sys.year);
		}
		else if(IN_Stick(BT_DEC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.year = _data_sys.year > 0 ? _data_sys.year - 1 : 99;
			CLCD_PrintNumBuffer(0, INDEX_LCD_YEAR, 2000 + _data_sys.year);
		}

		if(IN_IsPressed(BT_MODE) || (_time_out < 5)){
			_time_screen = TIME_SCREEN_DATE;
			DS_Write(ADDRESS_YEAR, _data_sys.year);

			CLCD_CreateChar(0, temp_icon);
			CLCD_CreateChar(1, humi_icon);

			CLCD_PrintStringBuffer(0, 0, SCREEN_DATE_DHT_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_DATE_DHT_1);
			CLCD_PrintCharBuffer(1, INDEX_CEL_SYMBOL, 0xDF);
			CLCD_PrintCharBuffer(1, INDEX_LCD_TEMP_SYMBOL, 0);
			CLCD_PrintCharBuffer(1, INDEX_LCD_HUMI_SYMBOL, 1);
			mode_sys = DISPLAY_DATE_DHT;
		}
		break;
	case SET_ALARM:

		if(IN_IsPressed(BT_MODE)){
			_data_sys.alarm_flag = 1;
			DS_Write(ADDRESS_FLAG_ALARM, 1);

			CLCD_PrintStringBuffer(0, 0, SCREEN_SET_ALARM_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_SET_ALARM_1);

			CLCD_PrintNumBuffer(1, INDEX_ALARM_HOUR, _data_sys.alarm_hour);
			CLCD_PrintNumBuffer(1, INDEX_ALARM_MIN, _data_sys.alarm_min);

			mode_sys = SET_ALARM_HOUR;
		}
		else if(IN_IsPressed(BT_DEC) || IN_IsPressed(BT_INC) || (_time_out < 5)){
			_time_screen = 50;

			_data_sys.alarm_flag = 0;
			DS_Write(ADDRESS_FLAG_ALARM, 0);

			mode_sys = INIT;
		}
		break;
	case SET_ALARM_HOUR:
//		Blink BLink BLink :D
		if(_counter_time_elapsed % 20 < 10){
			CLCD_PrintNumBuffer(1, INDEX_ALARM_HOUR, _data_sys.alarm_hour);
		}
		else {
			CLCD_PrintStringBuffer(1, INDEX_ALARM_HOUR, "  ");
		}

//		Increase/Decrease 1 every 500ms
		if(IN_Stick(BT_INC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.alarm_hour = _data_sys.alarm_hour < 23 ? _data_sys.alarm_hour + 1 : 0;
			CLCD_PrintNumBuffer(1, INDEX_ALARM_HOUR, _data_sys.alarm_hour);
		}
		else if(IN_Stick(BT_DEC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.alarm_hour = _data_sys.alarm_hour > 0 ? _data_sys.alarm_hour - 1 : 23;
			CLCD_PrintNumBuffer(1, INDEX_ALARM_HOUR, _data_sys.alarm_hour);
		}

		if(IN_IsPressed(BT_MODE)){
			_time_out = TIME_OUT_FOR_SET;
			DS_Write(ADDRESS_HOUR_ALARM, _data_sys.alarm_hour);
			CLCD_PrintNumBuffer(1, INDEX_ALARM_HOUR, _data_sys.alarm_hour);
			mode_sys = SET_ALARM_MIN;
		}
		else if(_time_out < 5){
			_time_screen = 50;
			DS_Write(ADDRESS_HOUR_ALARM, _data_sys.alarm_hour);
			mode_sys = INIT;
		}
		break;
	case SET_ALARM_MIN:
//		Blink BLink BLink :D
		if(_counter_time_elapsed % 20 < 10){
			CLCD_PrintNumBuffer(1, INDEX_ALARM_MIN, _data_sys.alarm_min);
		}
		else {
			CLCD_PrintStringBuffer(1, INDEX_ALARM_MIN, "  ");
		}

//		Increase/Decrease 1 every 500ms
		if(IN_Stick(BT_INC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.alarm_min = _data_sys.alarm_min < 59 ? _data_sys.alarm_min + 1 : 0;
			CLCD_PrintNumBuffer(1, INDEX_ALARM_MIN, _data_sys.alarm_min);
		}
		else if(IN_Stick(BT_DEC) % 10 == 1){
			_time_out = TIME_OUT_FOR_SET;
			_data_sys.alarm_min = _data_sys.alarm_min > 0 ? _data_sys.alarm_min - 1 : 59;
			CLCD_PrintNumBuffer(1, INDEX_ALARM_MIN, _data_sys.alarm_min);
		}

		if(IN_IsPressed(BT_MODE)){
			_time_screen = 50;
			DS_Write(ADDRESS_MIN_ALARM, _data_sys.alarm_min);

			mode_sys = INIT;
		}
		else if(_time_out < 5){
			_time_screen = 50;
			DS_Write(ADDRESS_MIN_ALARM, _data_sys.alarm_min);
			mode_sys = INIT;
		}
		break;
	case ALARM_GO_OFF:
		if(_counter_time_elapsed % 20 < 10){
			CLCD_PrintStringBuffer(0, 0, SCREEN_ALARM_GO_OFF_0);
			CLCD_PrintStringBuffer(1, 0, SCREEN_ALARM_GO_OFF_1);
		}
		else {
			CLCD_PrintStringBuffer(0, 0, SCREEN_BLANK);
			CLCD_PrintStringBuffer(1, 0, SCREEN_BLANK);
		}

		if(_counter_time_elapsed % 20 < 10){
			OUT_SetBuzzer(0);
		}
		else {
			OUT_SetBuzzer(1);
		}

		if(_time_out < 5 || IN_IsPressed(BT_MODE) || IN_IsPressed(BT_DEC) || IN_IsPressed(BT_INC)){
			_time_screen = 50;
			mode_sys = INIT;
		}
		break;
	}
}
