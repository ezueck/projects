//This is our Systick handler file 
//Made by Eduardo Zueck Garces
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
	
#include "tm4c123gh6pm.h"
#include "ADC.h"
#include "UART.h"

signed int message[5];
int ADCStatus = 0;
int ADCMail = 0;
int TxCounter = 0;

//Function to convert our data and place it in an array
void Convert(uint32_t input){
	signed long output;
	output = 1575-(0.40344*input);
	
	//We have set bounds for 0-1.5 cm
	if(output > 1500){ 											
		message[0]= 1;
		message[1]= -2;
		message[2]= 5;
		message[3]= 0;
		message[4]= 0;
	}
	else if (output < 0){
		message[0]= 0;
		message[1]= -2;
		message[2]= 0;
		message[3]= 0;
		message[4]= 0;
	}
	
	//Get our message. No iteration, excuse my laziness
	else{
		message[0]= output/1000;
		output= output-(message[0]*1000);
		message[1]= -2;
		message[2]= output/100;
		output= output-(message[2]*100);
		message[3]= output/10;
		output= output-(message[3]*10);
		message[4]= output;
	}
}
	
//Handler sampling the distance and sending it
void SysTick_Handler(void){unsigned long sampling;  int i = 0;
	GPIO_PORTF_DATA_R ^= 0x04;    				  //Heartbeat
	sampling = ADC_In();			//Get our new sample
	GPIO_PORTF_DATA_R ^= 0x04;
	
	if(sampling > ADCMail+5 || sampling < ADCMail-5){	//Implemented to eliminate stuttering 
		Convert(sampling);
		UART1_OutChar(0x02);
		while (i<5){
			UART1_OutChar(message[i] + 0x30);
			i++;
		}
		UART1_OutChar(0x0D);
		UART1_OutChar(0x03);
		i=0;
	}
	ADCMail = sampling;				//MAAAAIL
	TxCounter ++;							//Increment our conter of samples 
	GPIO_PORTF_DATA_R ^= 0x04; 							//Heartbeat
}


//Initialize our Systick timer with 40HZ
void Systick_Init(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 2000000-1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;
	NVIC_ST_CTRL_R = 0x0007;
}

