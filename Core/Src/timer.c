/*
 * timer.c
 *
 *  Created on: Apr 13, 2023
 *      Author: user
 */


#include "timer.h"

TIM_HandleTypeDef *stimer;

static uint8_t	TIME_CYCLE 		= 50;

static uint16_t MUL_timer		= 1;
static uint8_t flag_timer		= 0;
static uint16_t counter_timer 	= 0;

void RunSoftwareTimer(void){
	if(counter_timer > 0){
		counter_timer--;
		if(counter_timer == 0){
			counter_timer = MUL_timer;
			flag_timer = 1;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == stimer->Instance) {
		RunSoftwareTimer();
	}
}


//==================HIGH LEVEL FUNCTION==================//
void TM_Init(TIM_HandleTypeDef* htim){
	stimer = htim;
	counter_timer = MUL_timer;
	flag_timer = 0;

	HAL_TIM_Base_Start_IT(stimer);
}

void TM_SetTime(uint16_t duration){
	MUL_timer = duration / TIME_CYCLE;
	counter_timer = MUL_timer;
	flag_timer = 0;
}

uint8_t TM_IsFlag(void){
	if(flag_timer) {
		flag_timer = 0;
		return 1;
	}
	return 0;
}
