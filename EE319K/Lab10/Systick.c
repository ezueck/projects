//This is our Systick Handler File 
//We init to 30hz to get data to refresh screen
//We get input from: Buttons, ADC
//Made by Eduardo Zueck 

#include "InputOutput.h"
#include "Systick.h"
#include "ADC.h"
#include "tm4c123gh6pm.h"
#include "InputConversion.h"

//Systick_Init initializes our ST to 30hz 
//Very simple stuff 
unsigned int InputOne = 0;
unsigned int InputTwo = 0;
unsigned int TheTimer = 0;
unsigned int DaBomb = 0;

void Systick_Init(void){
		NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 2666666-1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;
	NVIC_ST_CTRL_R = 0x0007;
}
	
	
//SysTick_Handler 
//Get data to update the state of the videogame 
//Send it bck through global variables 
void SysTick_Handler(void){
	InputOne = Input_Check1();
	InputTwo = Input_Check2();
	TheTimer++;
	DaBomb = ADC_Convert(ADC0_In());
}

