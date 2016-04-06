//This is our DAC Driver 
//It uses Timer0 to create an output at 10025hz 
//We can output: The normal sound, explosion, normal sound + beeeping

#include "tm4c123gh6pm.h"

void DAC_Init(void){
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x03; 
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTB_AMSEL_R &= ~0x3F;      //no analog
  GPIO_PORTB_PCTL_R &= ~0x0000003F; //normal function
  GPIO_PORTB_DIR_R |= 0x3F;    //Make PB 0-5 outss
  GPIO_PORTB_AFSEL_R &= ~0x3F; //disable alt func on PB-5
  GPIO_PORTB_DEN_R |= 0x3F;    //enable digital for PORTB
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned char data){
	GPIO_PORTB_DATA_R=data;
}
