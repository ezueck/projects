//This is our driver for our main GPIO Ports
//This is divided to get IO for the two different players 
//Each player has: 5 inputs (aside from ADC) and 3 

//We start our IO system
//
#include "tm4c123gh6pm.h"
#include "Systick.h"

int Toggle1 = 0; //Toggle checker for Player 1
int Toggle2 = 0; //Toggle checker for Player 2

void IO_Init(void){
//Init for Player 1 
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC; 
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTC_AMSEL_R &= ~0xFF;      //no analog
  GPIO_PORTC_PCTL_R &= ~0x000000FF; //normal function
  GPIO_PORTC_DIR_R |= 0x1F;  			  //Make PB 0-4 Outss
	GPIO_PORTC_AFSEL_R &= ~0xE0; 			//Make PC 5-7 Ins
  GPIO_PORTC_AFSEL_R &= ~0xFF; 			//disable alt func on PB-5
  GPIO_PORTC_DEN_R |= 0xFF;    			//enable digital for PORTD
	
//Init for Player 2
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; 
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTD_AMSEL_R &= ~0xFF;      //no analog
  GPIO_PORTD_PCTL_R &= ~0x000000FF; //normal function
  GPIO_PORTD_DIR_R |= 0x1F;  			  //Make PB 0-4 Outss
	GPIO_PORTD_AFSEL_R &= ~0xE0; 			//Make PC 5-7 Ins
  GPIO_PORTD_AFSEL_R &= ~0xFF; 			//disable alt func on PB-5
  GPIO_PORTD_DEN_R |= 0xFF;    			//enable digital for PORTD
}


//Get the input the player is sending
//If he is also clicking the bomb button, add 10 
//Convention: Up: 0, Down:1, Left: 2, Right:3 
//Order of importance in case more than one button is clicked
//		Up > Down > Left > Right > 

unsigned int Input_Check1(void){unsigned int input;
	input = GPIO_PORTC_DATA_R;
	input = input&0x0000001F;
	return input;
}

unsigned int Input_Check2(void){unsigned int input;
	input = GPIO_PORTD_DATA_R;
	input = input&0x0000001F;
	return input;
}

//Send the state of live for player 1 
//If it is 1, it will toggle
void LivesOne(unsigned int lives){int Counter1;
	if(lives == 3){
		GPIO_PORTC_DATA_R |= 0xE0;
	}
	if(lives == 2){
		GPIO_PORTC_DATA_R |= 0x60;
	}
	if(lives == 1){
		if (Toggle1==0){
			Counter1 = TheTimer;
			Toggle1 = 1;
			GPIO_PORTC_DATA_R |= 0x20;
		}
		else if(Toggle1==1){
			if( TheTimer >= Counter1+30){
				GPIO_PORTC_DATA_R &= ~0x20;
				Toggle1=2;
				Counter1=TheTimer;
			}
		else if(Toggle1==2){
			if( TheTimer >= Counter1+30){
				GPIO_PORTC_DATA_R |= 0x20;
				Toggle1=1;
				Counter1=TheTimer;
			}
		}
	}
}
}

//Send the state of lie for player 2
//If it is 1, it will toggle
void LivesTwo(unsigned int lives){int Counter1;
	if(lives == 3){
		GPIO_PORTD_DATA_R |= 0xE0;
	}
	if(lives == 2){
		GPIO_PORTD_DATA_R |= 0x60;
	}
	if(lives == 1){
		if (Toggle2==0){
			Counter1 = TheTimer;
			Toggle2 = 1;
			GPIO_PORTD_DATA_R |= 0x20;
		}
		else if(Toggle2==1){
			if( TheTimer >= Counter1+30){
				GPIO_PORTD_DATA_R &= ~0x20;
				Toggle2=2;
				Counter1=TheTimer;
			}
		else if(Toggle2==2){
			if( TheTimer >= Counter1+30){
				GPIO_PORTD_DATA_R |= 0x20;
				Toggle2=1;
				Counter1=TheTimer;
			}
		}
	}
}
}

