/*
 * input.c
 *
 *  Created on: Apr 13, 2023
 *      Author: user
 */


#include "input.h"


GPIO_TypeDef* PORT_BUTTON[] = {BT0_GPIO_Port, BT1_GPIO_Port, BT2_GPIO_Port};
uint16_t PIN_BUTTON[] = {BT0_Pin, BT1_Pin, BT2_Pin};

uint8_t Flag_for_button_press[NO_OF_BUTTONS];
uint16_t Counter_button_press[NO_OF_BUTTONS];


void IN_Init(void){

}



//this function is called every 50ms
void IN_ReadButton(void){
	for(uint8_t i = 0; i < NO_OF_BUTTONS; i++){
		if(HAL_GPIO_ReadPin(PORT_BUTTON[i], PIN_BUTTON[i]) == BUTTON_IS_PRESSED){
			if(Counter_button_press[i] < MAX_COUNTER){
				Counter_button_press[i]++;
			}
		}
		else {
			Flag_for_button_press[i] = (Counter_button_press[i] > 0) && (Counter_button_press[i] < 10);
			Counter_button_press[i] = 0;
		}
	}
}

uint16_t IN_Stick(uint8_t index){
	return Counter_button_press[index];
}


uint8_t IN_IsPressed(uint8_t index){
	if(index >= NO_OF_BUTTONS) return 0;

	return Flag_for_button_press[index];
}

uint8_t IN_IsPressed_ms(uint8_t index, uint16_t duration){
	if(index >= NO_OF_BUTTONS) return 0;

	//button is read every 50ms -> counter += 1 every 50ms if button is held
	return Counter_button_press[index] >= (duration / 50);
}

