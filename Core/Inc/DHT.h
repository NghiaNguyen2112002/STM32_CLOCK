/*
 * DHT.h
 *
 *  Created on: Apr 4, 2023
 *      Author: user
 */

#ifndef INC_DHT_H_
#define INC_DHT_H_

#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "main.h"
#include "lcd_i2c.h"

typedef struct{
	GPIO_TypeDef* 			DHT_port;
	uint16_t 				DHT_pin;
	TIM_HandleTypeDef* 		timer;
	float 					temp;
	float 					humi;
}DHT_Name;

void DHT_Init(GPIO_TypeDef* DHT_port, uint16_t DHT_pin, TIM_HandleTypeDef* timer);
void DHT_Read(void);
float DHT_GetTemp(void);
float DHT_GetHumi(void);


#endif /* INC_DHT_H_ */
