/*
 * lcd_screen.h
 *
 *  Created on: Apr 13, 2023
 *      Author: user
 */

#ifndef INC_LCD_SCREEN_H_
#define INC_LCD_SCREEN_H_

//==================LCD SCREEN==================//
#define SCREEN_INIT_0                   "      CLOCK     "
#define SCREEN_INIT_1                   "                "

#define SCREEN_TIME_0					"      .         "	//"XXXXXX:XXXXXX ||"
#define SCREEN_TIME_1					"      .         "  //"XXXXXX:XXXXXX xx"

#define SCREEN_CONFIRM_SET_ALARM_0		"   SET ALARM ?  "
#define SCREEN_CONFIRM_SET_ALARM_1		"[N]   [OK]   [N]"

#define SCREEN_SET_ALARM_0				"    SET ALARM   "
#define SCREEN_SET_ALARM_1				"H:      M:      "

#define SCREEN_DATE_DHT_0				"       /  /     "	//" Sat 12/10/2023 "
#define SCREEN_DATE_DHT_1				"      *C      % "	//" |30.3*C |40.3% "

#define SCREEN_ALARM_GO_OFF_0			"RENGGGGGGGGGGGGG"
#define	SCREEN_ALARM_GO_OFF_1			"GET UPPPPPPPPPPP"

#define SCREEN_BLANK					"                "

//==================INDEX LCD==================//
#define INDEX_LCD_HOUR				0
#define INDEX_COLON					6
#define INDEX_LCD_MIN				7
#define INDEX_LCD_ICON_ALARM		14
#define INDEX_LCD_SEC				14

#define INDEX_LCD_WEEK_DAY			1
#define INDEX_LCD_DAY				5
#define INDEX_LCD_MONTH				8
#define INDEX_LCD_YEAR				11
#define INDEX_LCD_TEMP_SYMBOL		1
#define INDEX_LCD_TEMP				2
#define INDEX_CEL_SYMBOL			6
#define INDEX_LCD_HUMI_SYMBOL		9
#define INDEX_LCD_HUMI				10

#define INDEX_ALARM_HOUR			3
#define INDEX_ALARM_MIN				11






#endif /* INC_LCD_SCREEN_H_ */
