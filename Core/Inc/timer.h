/*
 * timer.h
 *
 *  Created on: Apr 13, 2023
 *      Author: user
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "main.h"

void TM_Init(TIM_HandleTypeDef* htim);
void TM_SetTime(uint16_t duration);
uint8_t TM_IsFlag(void);

#endif /* INC_TIMER_H_ */
