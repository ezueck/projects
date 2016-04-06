// ***** 0. Documentation Section *****
// TrafficLight.c for Lab 5
// Runs on LM4F120/TM4C123
// Implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// Feb 24, 2015

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "SysTick.c"
#include "tm4c123gh6pm.h"
#include <stdint.h>
// ***** 2. Global Declarations Section *****
//We define our structure 
struct State {
	uint32_t Out;
	uint32_t Time;
	uint32_t Next[8];
};
typedef const struct State STyp;

#define GoS 		0
#define WaitS		1
#define GoW		2
#define WaitW		3
#define Walk		4
#define HurryUpA	5
#define HurryUpB	6
#define HurryUpC	7
#define HurryUpD	8
#define HurryUpE	9
#define WaitSI		10
#define GoWI		11
#define WaitWI		12

//We build our structure using the FSM design 

STyp FSM[13] = {
	{0x8C, 200, {GoS,WaitS,GoS,WaitS,WaitSI,WaitSI,WaitSI,WaitSI}},
	{0x94, 100, {GoW,GoW,GoW,GoW,GoW,GoW,GoW,GoW}},
	{0xA1, 200, {GoW,GoW,WaitW,WaitW,WaitWI,WaitWI,WaitW,WaitW}},
	{0xA2, 100, {GoS,GoS,GoS,GoS,GoS,GoS,GoS,GoS}},
	{0x64, 100, {HurryUpA,HurryUpA,HurryUpA,HurryUpA,HurryUpA,HurryUpA,HurryUpA,HurryUpA}},
	{0xE4,  50, {HurryUpB,HurryUpB,HurryUpB,HurryUpB,HurryUpB,HurryUpB,HurryUpB,HurryUpB}},
	{0x64,  50, {HurryUpC,HurryUpC,HurryUpC,HurryUpC,HurryUpC,HurryUpC,HurryUpC,HurryUpC}},
	{0xE4,  50, {HurryUpD,HurryUpD,HurryUpD,HurryUpD,HurryUpD,HurryUpD,HurryUpD,HurryUpD}},
	{0x64,  50, {HurryUpE,HurryUpE,HurryUpE,HurryUpE,HurryUpE,HurryUpE,HurryUpE,HurryUpE}},
	{0xE4,  50, {GoS,GoW,GoS,GoS,GoS,GoW,GoS,GoS}},
	{0x94, 100, {Walk,GoWI,Walk,GoWI,Walk,GoWI,Walk,GoWI}},
	{0xA1, 200, {WaitWI,WaitWI,WaitWI,WaitWI,WaitWI,WaitWI,WaitWI,WaitWI}},
	{0xA2, 100, {Walk,Walk,Walk,Walk,Walk,Walk,Walk,Walk}}
};

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void GPIOInit(void);
// ***** 3. Subroutines Section *****

int main(void){ 
	uint32_t CS; 								// The current state
	uint32_t Input; 						//Input 
	uint32_t Output;
	uint32_t OutputF;
	uint32_t OutFP;

  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210); // activate grader and set system clock to 80 MHz
  GPIOInit();
	CS = GoS;
  EnableInterrupts();

  while(1) {
		Output = FSM[CS].Out; //We will update the output of PortE
		Output = Output&0x3F;
		GPIO_PORTE_DATA_R = Output; //Update our traffic lights
		Output = FSM[CS].Out;  //We will update the output of PortF

		OutputF = Output&0x40; //We mask the output to get our PF1 and PF3
		OutputF = OutputF >> 3;
		OutFP = Output&0x80;
		OutFP = OutFP >> 6;
		GPIO_PORTF_DATA_R = OutFP + OutputF; //Update PF	
		SysTick_Wait10ms(FSM[CS].Time); //Delay

		Input = GPIO_PORTA_DATA_R; //Get our inputs
		Input = Input >> 2; //Right shift our data as the inputs are A2-4
		Input = Input&0x07; //We just want the first three values
		CS = FSM[CS].Next[Input];
	}
}

void GPIOInit(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x31;      // 1) enable clock to F E B
  delay = SYSCTL_RCGC2_R;      // 2) no need to unlock
	SysTick_Init();  //Initialize SysTick
	//Initialization for PortF
	GPIO_PORTF_DIR_R |= 0x0A;   // make PF1 and PF3 out 
  GPIO_PORTF_AFSEL_R &= ~0xFF;// disable alt funct on PF
  GPIO_PORTF_DEN_R |= 0xFF;   // enable digital I/O on PF
	GPIO_PORTF_AMSEL_R = 0;     // disable analog functionality on PF 
	//Initialization for PortE
	GPIO_PORTE_DIR_R |= 0x3F;   // make PE0-PE5 outs
  GPIO_PORTE_AFSEL_R &= ~0xFF;// disable alt funct on PE
  GPIO_PORTE_DEN_R |= 0xFF;   // enable digital I/O on PE
	GPIO_PORTE_AMSEL_R = 0;     // disable analog functionality on PE
	//Initialization for PortA
	GPIO_PORTA_DIR_R = 0x00;   // make PA inputs
  GPIO_PORTA_AFSEL_R &= ~0xFF;// disable alt funct on PE
  GPIO_PORTA_DEN_R |= 0xFF;   // enable digital I/O on PE
	GPIO_PORTA_AMSEL_R = 0;     // disable analog functionality on PE
}
