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
	else if ((input&0x01) == 0x01){ //Signals up
		output =1;
	}
	else if((input&0x02) == 0x02){ //Signals down
		output = 2;
	}
	else if((input&0x04) == 0x04){ //Signals left
		output = 3;
	}
	else if ((input&0x08) == 0x08){ //Signals right
		output = 4;
	}
	return output;
}

unsigned int ADC_Convert(unsigned int input, unsigned int max){
  signed int output;
	output = (1575-(0.40344*input))*4;
	output = output/1000;
	if (output<1){
		return 1;
	}
	else{
		return output;
	}
}
	

