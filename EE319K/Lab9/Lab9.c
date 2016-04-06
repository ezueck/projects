// Lab9.c
// Runs on LM4F120 or TM4C123
// Student names: Eduardo Zueck Garces
// Last Modified: 4/19/2015

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats

#include <stdint.h>
#include "Systick.h"
#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "FIFO.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
	
int32_t Data;        // 12-bit ADC
int32_t Position;    // 32-bit fixed-point 0.001 cm


// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x035; //  We activate clock PE
  delay = SYSCTL_RCGC2_R;        //   
  GPIO_PORTF_AMSEL_R &= ~0x0F;      // no analog
	GPIO_PORTF_PCTL_R &= ~0x00000FFF; // regular function
	GPIO_PORTF_DIR_R |= 0x0E;      // Make PF1-3 input 
  GPIO_PORTF_AFSEL_R &= ~0x0F;   // disable alt on PE2-0
  GPIO_PORTF_DEN_R |= 0x0E;      // Enable Digital IO 
}

//Our final main program
int main(void){ int print; 
	
	//Initializations 
  TExaS_Init();         			//Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); //Initialize the screen
  PortF_Init();								//Initialize PortF
  ADC_Init();       				  //Turn on ADC, set channel to 1
	UART1_Init();
	Systick_Init();							//Initialize our SysTick timer
  EnableInterrupts();					//We enable interrupts and run with it
	while(1){
		print = Fifo_Get();
		if(print==0x02){ 					//We got a new value!
			ST7735_SetCursor(0,0);	
			while(print!= 0x03 && print!=-1){				//Print while we have a message
				print &= 0xFF;
				if( ((print>= 0x30) && (print<= 0x39)) || (print == 0x2E)){	//Only print the numbers, not the end etc. 
					ST7735_OutChar(print);
				}
				print = Fifo_Get();	
			}
			ST7735_OutString(" cm");
		}	
	}
}



