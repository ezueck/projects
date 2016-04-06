// Lab6.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// MOOC lab 13 or EE319K lab6 starter
// Program written by: Eduardo Zueck Garces
// Date Created: 1/24/2015 
// Last Modified: 3/24/2015
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********


#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"
#include "dac.h"
unsigned int Note;

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void HeartBeat(void){
	unsigned long volatile delayE;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;
	delayE = SYSCTL_RCGC2_R;   
	GPIO_PORTC_AMSEL_R &= ~0x30;      // no analog
  GPIO_PORTC_PCTL_R &= ~0x00000FFF; // regular function
  GPIO_PORTC_DIR_R |= 0x30;      // make PC4-5 out
  GPIO_PORTC_AFSEL_R &= ~0x30;   // disable alt on PC4-5
  GPIO_PORTC_DEN_R |= 0x30;      // enable digitalon PE4-5
}

int main(void){  
	//We intialize our system
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  Piano_Init();
  Sound_Init(0);
	HeartBeat();
  EnableInterrupts();
	
	//Loop to check for Piano Keys 
  while(1){ 
		unsigned long note;
		unsigned long OldNote;
		note = Piano_In();  //Get current key
		if (note!=OldNote){ //We check if different key to change SysTick Reload
			Sound_Play(note);
		OldNote = note;
		}
  }         
}

