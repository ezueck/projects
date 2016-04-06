//Get the direction the player is sending
//If he is also clicking the bomb button, add 10 
//Convention: Up: 1, Down:2, Left: 3, Right:4 
//Order of importance in case more than one button is clicked
//		Up > Down > Left > Right > 

unsigned int Input_Convert(unsigned int input){
	unsigned int output = 0;
	if (((input&0x10)) == 0x10){ //Signals a bomb is placed
		output = 10;
	}
	if ((input&0x80) == 0x80){ //Signals up
		output =1;
	}
	else if((input&0x40) == 0x40){ //Signals down
		output = 2;
	}
	else if((input&0x20) == 0x20){ //Signals left
		output = 3;
	}
	else if ((input&0x10) == 0x10){ //Signals right
		output = 4;
	}
	return output;
}

unsigned int ADC_Convert(unsigned int input, unsigned int max){unsigned int output = 0;
	max = max/5;
	output = input/max;
	return output;
}
	

