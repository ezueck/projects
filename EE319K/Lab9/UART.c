//This is our UART1 driver
//Connected with PC5-4 for input and output 
//We run with a baud rate of 100,000
//Made by: Eduardo Zueck Garces

#include "tm4c123gh6pm.h"
#include "FIFO.h"

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))

int RxCounter;

//UART1_Init is our initialization routine for the UART
//Sets the baud rate of 100,000
//Activates PC5-4
void UART1_Init(void){
	Fifo_Init();
	
	SYSCTL_RCGCUART_R |= 0x0002;  // Activate UART1
  SYSCTL_RCGC2_R |= 0x0004;  // Clock for C
  UART1_CTL_R &= ~0x0001;    // Disable UART to configure it 
  UART1_IBRD_R = 50; 				     // 80,000,000/(16*100,000)= 0
  UART1_FBRD_R = 0;     				 // No fraction
  UART1_LCRH_R = 0x0070; 		 // Normal config: 8 bit + FIFOS 
	
	
  GPIO_PORTC_AFSEL_R |= 0x30;    // Enable alt PC5-4
  GPIO_PORTC_DEN_R |= 0x30;      
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000; // Configure PC5-4 to UART1
  GPIO_PORTC_AMSEL_R &= ~0x30;   // Disable analog on PC5-4
	
	UART1_IM_R |= 0x10;
	UART1_IFLS_R |= 0x010;
  UART1_CTL_R |= 0x0301;     // Enable UART
	NVIC_PRI1_R &= 0xFFEFFFFF; // Enable interrupt for UART1 in NVIC
	NVIC_EN0_R |= 0x040;			 // Enable interrupt for UART1 in NVIC
	
	RxCounter = 0;
}

//UART1_OutChar is the routine to send a character through the UART
//Wait for flag, then send 
void UART1_OutChar(unsigned char data){
	while((UART1_FR_R&0x0020) != 0){}	//Wait unti FIFO is empty 
  UART1_DR_R = data;								//Sned our data
}

//UART1_InChar is the routine to receive a character through the UART
//Wait till there's something in the FIFO, then get it 
unsigned char UART_InChar(void){
  while((UART1_FR_R&0x0010) != 0){}      		// Wait until FIFO has something 
  return((unsigned char)(UART1_DR_R&0xFF)); // Only return our data, not the error codes
}

//This is our interrupt handler for UART1
//Gets data obtaind from the UART and puts it in the FIFO
void UART1_Handler(void){
	GPIO_PORTF_DATA_R ^= 0x02;
	while (((UART1_FR_R & 0x10) == 0)) {
		Fifo_Put(UART1_DR_R);
		RxCounter++;
	}
	UART1_ICR_R = 0x10;
	GPIO_PORTF_DATA_R ^= 0x02;
}
	

