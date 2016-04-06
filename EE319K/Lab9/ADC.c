// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/6/2015 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
  volatile unsigned long delay;
  GPIO_PORTE_DIR_R &= ~0x04;      // Make PE2 input 
  GPIO_PORTE_AFSEL_R |= 0x04;     // Alt Functions for PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // No DIO for PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // Analog for PE2
  SYSCTL_RCGCADC_R |= 0x01;  		  // Activate ADC0
  delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;
  ADC0_PC_R = 0x01;								// Configure for 125K
  ADC0_SSPRI_R = 0x0123;          // Sequencer 3 is priority
  ADC0_ACTSS_R &= ~0x0008;        // Disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // Seq3 is software triggered
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)	//Clear SS3
									+1;							// Set channel 1
  ADC0_SSCTL3_R = 0x0006;         // Clear TS0 D0, Set IE0 END0
  ADC0_ACTSS_R |= 0x0008;         // Enable sampling
}


//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
  unsigned long sample;
  ADC0_PSSI_R = 0x0008;            // 1) Start sampling
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  sample = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return sample;
}


