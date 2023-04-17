/*
 * output.h
 *
 *  Created on: Apr 14, 2023
 *      Author: user
 */

#ifndef INC_OUTPUT_H_
#define INC_OUTPUT_H_

#include "stm32f1xx_hal.h"
#include "main.h"

void OUT_Init(GPIO_TypeDef* buzzer_port, uint16_t buzzer_pin);

void OUT_SetBuzzer(uint8_t value);


#endif /* INC_OUTPUT_H_ */
