/*
 * DHT.c
 *
 *  Created on: Apr 4, 2023
 *      Author: user
 */


#include "DHT.h"


DHT_Name dht11;

void Delay_us(uint16_t duration){
	__HAL_TIM_SET_COUNTER(dht11.timer, 0);
	while(__HAL_TIM_GET_COUNTER(dht11.timer) < duration){}
}

void SetPinIn(void){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /*Configure GPIO pin*/
	  GPIO_InitStruct.Pin = dht11.DHT_pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(dht11.DHT_port, &GPIO_InitStruct);
}

void SetPinOut(void){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /*Configure GPIO pin*/
	  GPIO_InitStruct.Pin = dht11.DHT_pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(dht11.DHT_port, &GPIO_InitStruct);
}

static void WritePin(uint8_t value){
	HAL_GPIO_WritePin(dht11.DHT_port, dht11.DHT_pin, value);
}

static uint8_t ReadPin(void){
	return HAL_GPIO_ReadPin(dht11.DHT_port, dht11.DHT_pin);
}

//===========================HIGH LEVEL FUNCTION===========================//
void DHT_Init(GPIO_TypeDef* DHT_port, uint16_t DHT_pin, TIM_HandleTypeDef* timer){
	dht11.DHT_port = DHT_port;
	dht11.DHT_pin = DHT_pin;
	dht11.timer = timer;

	dht11.humi = dht11.temp = 0;

	SetPinOut();
	WritePin(1);
}


void DHT_Read(void){


	uint16_t time_out = 0;
	uint8_t data[5] = {0, 0, 0, 0, 0};
	uint8_t humi_integral, humi_decimal,
			temp_integral, temp_decimal, check_sum;

	uint16_t temp, humi;
/*
	data[0] integral Humi
	data[1] decimal Humi
	data[2] integral Temp
	data[3] decimal Temp
	data[4] SUM
*/

	SetPinOut();
	WritePin(0);
	HAL_Delay(20);


	SetPinIn();

	Delay_us(40);

	if(!ReadPin()){
		Delay_us(80);
	}



	for(time_out = 0; (time_out < 100) && ReadPin(); time_out++);

//	start DHT successful => read
	for(uint8_t i = 0; i < 5; i++){
		for(uint8_t j = 0; j < 8; j++){

			for(time_out = 0; (time_out < 100) && !ReadPin(); time_out++);

			Delay_us(40);

			if(!ReadPin()) data[i] &= (~(1<<(7-j)));
			else data[i] |= (1 << (7-j));


			for(time_out = 0; (time_out < 100) && ReadPin(); time_out++);
		}
	}

	if(data[4] != (data[0] + data[1] + data[2] + data[3]))
		return;

	humi = (float)(data[0] * 1.0 + data[1] / 10.0);
	temp = (float)(data[2] * 1.0 + data[3] / 10.0);

	dht11.humi = humi;
	dht11.temp = temp;

	SetPinOut();
	WritePin(1);
}

float DHT_GetTemp(void){
	return dht11.temp;
}

float DHT_GetHumi(void){
	return dht11.humi;
}

