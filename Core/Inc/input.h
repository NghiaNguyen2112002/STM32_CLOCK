/*
 * input.h
 *
 *  Created on: Apr 13, 2023
 *      Author: user
 */

#ifndef INC_INPUT_H_
#define INC_INPUT_H_

#include "stm32f1xx_hal.h"
#include "main.h"


#define NO_OF_BUTTONS						3
#define MAX_COUNTER        					200   //10s

#define BUTTON_IS_PRESSED                   0
#define BUTTON_IS_RELEASED                  1

//==================INDEX BUTTON==================//
#define BT_INC							0
#define BT_MODE							1
#define BT_DEC							2

void IN_Init(void);
void IN_ReadButton(void);
uint16_t IN_Stick(uint8_t index);
uint8_t IN_IsPressed(uint8_t index);
uint8_t IN_IsPressed_ms(uint8_t index, uint16_t duration);



#endif /* INC_INPUT_H_ */
