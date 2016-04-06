//This is our driver for our main GPIO Ports
//This is divided to get IO for the two different players 
//Each player has: 5 inputs (aside from ADC) and 3 

//We start our IO system
//
#include "tm4c123gh6pm.h"
#include "Systick.h"
#include "Bomberman.h"
#include "Screen.h"


void IO_Init(void){
	
//Init for Player 1 
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; 
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_AMSEL_R &= ~0x3E;      //no analog
  GPIO_PORTE_PCTL_R &= ~0x0000003E; //normal function
  GPIO_PORTE_DIR_R &= ~0x3E;  			  //Make PE1-5 ins 
  GPIO_PORTE_AFSEL_R &= ~0x3E; 			//disable alt func on PB-5
  GPIO_PORTE_DEN_R |= 0x3E;    			//enable digital for PORTD
	
//Init for Player 2
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; 
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTB_AMSEL_R &= ~0x1F;      //no analog
  GPIO_PORTB_PCTL_R &= ~0x0000001F; //normal function
  GPIO_PORTB_DIR_R &= ~0x1F;  			  //Make PB 0-4 ins
  GPIO_PORTB_AFSEL_R &= ~0x1F; 			//disable alt func on PB-5
  GPIO_PORTB_DEN_R |= 0x1F;    			//enable digital for PORTD
}


//Get the input the player is sending
//If he is also clicking the bomb button, add 10 
//Convention: Up: 0, Down:1, Left: 2, Right:3 
//Order of importance in case more than one button is clicked
//		Up > Down > Left > Right > 

unsigned int Input_Check1(void){unsigned int input;
	input = GPIO_PORTE_DATA_R;
	input = input&0x0000003E;
	input = input>>1;
	return input;
}

unsigned int Input_Check2(void){unsigned int input;
	input = GPIO_PORTB_DATA_R;
	input = input&0x0000001F;
	return input;
}

//Send the state of live for player 1 
//If it is 1, it will toggle
void LivesOne(unsigned int lives){
	if(lives == 2){
		PrintDarkness(31, 12);
	}
	if(lives == 1){
		PrintDarkness(20, 12);
		}
	if(lives ==0){
		Conclusion(2);
	}
}

//Send the state of lie for player 2
//If it is 1, it will toggle
void LivesTwo(unsigned int lives){
	if(lives == 2){
		PrintDarkness(112, 12);
	}
	if(lives == 1){
		PrintDarkness(101, 12);
	}
	if(lives ==0){
		Conclusion(1);
	}
}

