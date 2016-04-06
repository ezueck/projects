// Piano.c
// This software configures the off-board piano keys
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 8/25/2014 
// Last Modified: 3/24/2015
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "sound.h"
unsigned long Song[29]={5062, 4252, 5682, 6377, 5682, 5062, 4252, 5682, 5062, 4252, 5682, 3188, 4252, 4778, 5062, 5682, 5062, 4252, 5682, 6377, 5682, 5062, 4252, 5682, 5062, 4252, 2841, 3188, 2127};

	//We play a secret song!
	void SecretSong(void){
	unsigned int SIndex = 0;
	unsigned int Half = 40000000; 
	while(SIndex<27){
		double Div = Half/Song[SIndex]; //Calculate number of repeats for each frequency
		Sound_Play(Song[SIndex]);
		while(SCount<Div){
		}
		SIndex = SIndex + 1;
		SCount=0;
	}
	Sound_Play(0);
}

// **************Piano_Init*********************
// Initialize piano key inputs, called once 
// Input: none 
// Output: none
void Piano_Init(void){ 
	unsigned long volatile delayE;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	delayE = SYSCTL_RCGC2_R;   
	GPIO_PORTE_AMSEL_R &= ~0x0F;      // no analog
  GPIO_PORTE_PCTL_R &= ~0x00000FFF; // regular function
  GPIO_PORTE_DIR_R &= ~0x07;      // make PE2-0 in
  GPIO_PORTE_AFSEL_R &= ~0x0F;   // disable alt on PE2-0
  GPIO_PORTE_DEN_R |= 0x0F;      // enable digitalon PE2-0
}

// **************Piano_In*********************
// Input from piano key inputs 
// Input: none 
// Output: 0 to 7 depending on keys
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2
uint32_t Piano_In(void){
	unsigned long input;
	input = GPIO_PORTE_DATA_R;
	input = input&0x07;
	//Change input into actual notes
	if(input == 0x01){ 			//Note G4
		input = 6378;
	}
	else if(input == 0x02){ //Note A4
		input = 5681;
	}
	else if(input == 0x04){ //Note B4
		input = 5061;
	}
	else if(input ==0){ 		//No sound
		input = 0;}
	else if(input==0x07){
		SecretSong();
		input=6378;
	}
	else{										//Note F4
		input = 7163;
	}
  return input; 
}


