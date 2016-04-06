// Lab8.c
// Runs on LM4F120 or TM4C123
// Student names: Eduardo Zueck Garces
// Last Modified: 4/14/2015 

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"


//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
	
int32_t Data;        // 12-bit ADC
int32_t Position;    // 32-bit fixed-point 0.001 cm
uint32_t ADCStatus;
uint32_t ADCMail;

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
	ADCMail=0;
	ADCStatus=0;
}

//Initialize our Heartbeat registers 
void HeartBeat(void){
	unsigned long volatile delayE;
	GPIO_PORTC_AMSEL_R &= ~0x30;      // no analog
  GPIO_PORTC_DIR_R |= 0x30;      // make PC4-5 out
  GPIO_PORTC_AFSEL_R &= ~0x30;   // disable alt on PC4-5
  GPIO_PORTC_DEN_R |= 0x30;      // enable digitalon PE4-5
}

//Initialize our Systick timer with 40HZ
void Systick_Init(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 2000000-1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;
	NVIC_ST_CTRL_R = 0x0007;
}

//Function to convert our data
uint32_t Convert(uint32_t input){
	unsigned long output;
	output = (1575-(0.40344*input) * 4);
  return output;
}
//Our final main program
int main(void){
	
	//Initializations 
  TExaS_Init();         			//Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); //Initialize the screen
  PortF_Init();								//Initialize PortF
  ADC_Init();       				  //Turn on ADC, set channel to 1
	HeartBeat();								//Initialize PortC for heartbeat
	Systick_Init();							//Initialize our SysTick timer
  EnableInterrupts();					//We enable interrupts and run with it
  
	//Main loop
	while(1){
		while(ADCStatus==0){ 			//Wait for new data
	  }
		DisableInterrupts();

    Data = ADCMail;  					//Get Data from mail
   
		PF3 = 0x08;      				  //Profile Convert
    Position = Convert(Data); 
    PF3 = 0;          				//End of Convert Profile
    
		PF1 = 0x02;      				  //Profile LCD
		
		//Out of bounds section
		if(Position>1500 || Position<0){ 
		ST7735_SetCursor(0,0);	
    ST7735_OutString("Out of bounds!"); 
    ST7735_SetCursor(6,0);
		}
		
		//Good section for the ruler
		else{
    ST7735_SetCursor(0,0);	
    LCD_OutDec(Position); ST7735_OutString("    "); 
    ST7735_SetCursor(6,0);
    LCD_OutFix(Position); ST7735_OutString(" cm"); 
		}
    PF1 = 0;          				//End of LCD Profile
		ADCStatus=0;
		EnableInterrupts();
  }
}

void SysTick_Handler(void){unsigned long sampling;	
	PF2 ^= 0x04;    				  //Heartbeat
	GPIO_PORTC_DATA_R ^= 0x30;      //Toggle HB
	sampling = ADC_In();						//Get our new sample
	if(sampling > ADCMail+5 || sampling < ADCMail-5){				//Check if new one for MB
		ADCStatus = 1;								//Set mailbox
	}
	ADCMail = sampling;							//MAAAAIL
}









int main4(void){
  TExaS_Init();       // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 1
  PortF_Init();
  while(1){           // use scope to measure execution time for ADC_In and LCD_OutDec           
		GPIO_PORTF_DATA_R |= 0x04;       // Profile ADC
		Data = ADC_In();  // sample 12-bit channel 1
		GPIO_PORTF_DATA_R &= ~0x04;      // end of ADC Profile
	}
}
int main2(void){
  TExaS_Init();       // Bus clock is 80 MHz 
	ST7735_InitR(INITR_REDTAB);
  PortF_Init();
	ADC_Init();  
	HeartBeat();	// turn on ADC, set channel to 1
  while(1){           // use scope to measure execution time for ADC_In and LCD_OutDec           
    GPIO_PORTC_DATA_R ^= 0x30;      //Toggle HB
    Data = ADC_In();  // sample 12-bit channel 1
    GPIO_PORTC_DATA_R ^= 0x30;      //Toggle HB
    ST7735_SetCursor(0,0);
    GPIO_PORTC_DATA_R ^= 0x30;      //Toggle HB
    LCD_OutDec(Data); 
    ST7735_OutString("    ");  // these spaces are used to coverup characters from last output
    GPIO_PORTC_DATA_R ^= 0x30;      //Toggle HB
  }
}


int main3(void){ 
  TExaS_Init();         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  ADC_Init();         // turn on ADC, set channel to 1
  while(1){  
    Data = ADC_In();  // sample 12-bit channel 1
    PF3 = 0x08;       // Profile Convert
    Position = Convert(Data); 
    PF3 = 0;          // end of Convert Profile
    PF1 = 0x02;       // Profile LCD
    ST7735_SetCursor(0,0);
    LCD_OutDec(Data); ST7735_OutString("    "); 
    ST7735_SetCursor(6,0);
    LCD_OutFix(Position);
    PF1 = 0;          // end of LCD Profile
  }
} 

