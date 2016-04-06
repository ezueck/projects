// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: Eduardo Zueck Garces
// Date Created: 8/25/2014 
// Last Modified: 3/24/2015
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; 
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTA_AMSEL_R &= ~0x7C;      //no analog
  GPIO_PORTA_PCTL_R &= ~0x00000FFF; //normal function
  GPIO_PORTA_DIR_R |= 0x7C;    //Make PA2-7 outss
  GPIO_PORTA_AFSEL_R &= ~0x7C; //disable alt func on PA2-5 
  GPIO_PORTA_DEN_R |= 0x7C;    //enable digital for PORTA
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTA_DATA_R=data;
}
