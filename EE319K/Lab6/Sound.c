// Sound.c
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: Eduardo Zueck
// Date Created: 8/25/2014 
// Last Modified: 3/24/2015 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "dac.h"
#include "tm4c123gh6pm.h"
#include "piano.h"
unsigned char Index;
const unsigned char SineWave[32] = {0,0,0,0,1,1,2,3,5,6,7,9,11,12,13,14,14,15,15,15,14,14,13,12,10,9,7,6,5,3,2,1};
unsigned long count = 420;
unsigned int togglebit = 0x10;
int SCount =0;
// ****** Toggle for our Heartbear *******
void Toggle(void){
	unsigned long status = NVIC_ST_RELOAD_R;
	if(status>50000){
		GPIO_PORTC_DATA_R = GPIO_PORTC_DATA_R^0x10; //Toggle LED
	}
	else{
		if(count==0){
			GPIO_PORTC_DATA_R = GPIO_PORTC_DATA_R^0x10; 
			count =420;
		}
		count= count-1;
	}
}

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: interrupt period
//           Units to be determined by 12.5ns
// Output: none
void Sound_Init(uint32_t period){
	DAC_Init();
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = period-1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;
	NVIC_ST_CTRL_R = 0x0007;
}


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be 12.5 ns
//         input of zero disable sound output
// Output: none
void Sound_Play(unsigned long period){
	NVIC_ST_RELOAD_R = period-1; 
}

void SysTick_Handler(void){
	SCount=SCount+1;
	GPIO_PORTC_DATA_R = GPIO_PORTC_DATA_R^0x20; //Service up
	//We output our next sample of the sine wave 
	Toggle();
	Index = (Index+1)&0x1F;  //Next index
	uint32_t out = SineWave[Index]; //Get the next sine 
	out = out <<2; //Mask or value 
  DAC_Out(out); //Output
	GPIO_PORTC_DATA_R = GPIO_PORTC_DATA_R^0x20; //Service done
}
