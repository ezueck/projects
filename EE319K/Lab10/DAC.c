//This is our DAC Driver 
//It uses Timer0 to create an output at 1000hz 
//We can output: The normal sound, explosion, normal sound + beeeping

#include "tm4c123gh6pm.h"

void DAC_Init(void){
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x21; 
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_AMSEL_R &= ~0x1F;      //No analog
  GPIO_PORTF_PCTL_R &= ~0x0000001F; //Normal function
  GPIO_PORTF_DIR_R |= 0x1F;    			//Make PF 0-4 outss
  GPIO_PORTF_AFSEL_R &= ~0x1F;		  //disable alt func on PB-5
  GPIO_PORTF_DEN_R |= 0x1F;   		  //enable digital for PORTB
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned char data){
	GPIO_PORTF_DATA_R=data&0x1F;
}
