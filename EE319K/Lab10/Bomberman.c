//This is our main file for Bomberman 

#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Systick.h"
#include "Sound.h"
#include "ADC.h"
#include "InputOutput.h"
#include "InputConversion.h"
#include "print.h"
#include "Action.h"
#include "Screen.h"
#include "startup.h"


//Essential variables
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Initialization(void);
void Introduction(void);
int OneInput;
int TwoInput;
int Service1;
int Service2;
int BombI = 0;
int PrevDaBomb = 0;


//Our main program
int main(void){ 
	Initialization(); 
  Introduction();
	PrintStartImage(); 													 //Print our first image and give it a roll
	PrintPower(1, 1);
	PrintPower(2, 1);
	while(1){
		if(PrevDaBomb != DaBomb){
			PrintADC(DaBomb);
			PrevDaBomb = DaBomb;			
		}
		OneInput = Input_Convert(InputOne); 			 //Get the converted version of our inputs
		TwoInput = Input_Convert(InputTwo);
		
		if(ServiceMove1 == 0){ //Select Action for Player 1
			ActionSelector(OneInput, 5);
		}
		if(ServiceMove2 == 0){ //Select Action for Player 2
			ActionSelector(5, TwoInput);
		}
		
		if(ServiceMove1 != 0){ 										 //Service moves for Player 1
			ScreenMove1();
		}
		if(ServiceMove2 != 0){ 										 //Service moves for Player 2
			ScreenMove2();
		}
		
		while(BombI<=9){													 //Handles our bombs ie, explosions and fixing their explosions
			if(BombStatus[BombI] != 0){							 //Evidently, there's only 10 bomb slots
				BombHandler(BombI);
			}
			BombI++;
		}
		
		if(DaBombStat[0] == 3 && TheTimer>= DaBombStat[5]+15){					//Fixes the mess created by DaBomb
			ExplosionFixer(DaBombStat[1], DaBombStat[2], DaBombStat[4]);
			DaBombStat[0] = 0;
			DaBombStat[1] = 0;
			DaBombStat[2] = 0;
			DaBombStat[3] = 0;
			DaBombStat[4] = 0;
			DaBombStat[5] = 0;
		}
		BombI = 0;
	}
}


//I learned how to use this 
//Basically, same thing as our old counters 
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

//Our initialization ritual
//Run this or nothing will work 
void Initialization(void){
	PLL_Init();   
	Sound_Init();
	ST7735_InitR(INITR_REDTAB);
	ADC0_Init();
	IO_Init();
	Systick_Init();
	LivesOne(3);
	LivesTwo(3);
	srand(NVIC_ST_CURRENT_R);
	EnableInterrupts();
}

//Displays our intro image 
//Waits for any kind of input
void Introduction(void){
	PrintIntro();
	while(InputOne == 0 && InputTwo == 0){
	}
}

//This is our conclusion
//It prints the image of whoever won
//Reverts back to the beginning
void Conclusion(int winner){
	if(winner == 1){
		PrintWinner1();
	}
	else if(winner == 2){
		PrintWinner2();
	}
	Delay100ms(1000);
}

