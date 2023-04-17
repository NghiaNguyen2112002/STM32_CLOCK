/*
 * output.c
 *
 *  Created on: Apr 14, 2023
 *      Author: user
 */

#include "output.h"

GPIO_TypeDef* Buzzer_port;
uint16_t Buzzer_pin;

void OUT_Init(GPIO_TypeDef* buzzer_port, uint16_t buzzer_pin){
	Buzzer_port = buzzer_port;
	Buzzer_pin = buzzer_pin;

	HAL_GPIO_WritePin(Buzzer_port, Buzzer_pin, SET);
}

void OUT_SetBuzzer(uint8_t value){
	if(value == 1) HAL_GPIO_WritePin(Buzzer_port, Buzzer_pin, RESET);
	else HAL_GPIO_WritePin(Buzzer_port, Buzzer_pin, SET);
}
