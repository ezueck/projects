// Sound.c
// Runs on any computer
// Sound assets based off the original Space Invaders 
// Import these constants into the Bomberman.c
// Eduardo Zueck

#include <stdint.h>
#include "Sound.h"
#include "DAC.h"
#include "Timer0.h"


unsigned int Sound = 0;
unsigned int LowLife = 0;

//We initializa the timer to 11025 
//Initialize the DAC
void Sound_Init(void){
	Timer0_Init(11025);
	DAC_Init();
}

//We send the sound to the Timer setting via a global variable Sound 
//We consider the special case were we have low life 
void Play_Sound(unsigned int sound){
	if (Sound == 0){
		Sound = sound;
	}
}
