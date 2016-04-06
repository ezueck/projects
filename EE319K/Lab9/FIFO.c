//This is our implementation of a software FIFO
//Created by Eduardo Zueck Garces

int OutPt;
int PutPt;
int PutA;
int OutA;
int Slots;
int FIFOArray[6];

//This short function initializes our FIFO 
//Not much to say here
void Fifo_Init(void){
	OutPt = 0;
	PutPt = 0;
	Slots = 0;
	PutA = 0;
	OutA = 0;
}

//This function in C saves data in a FIFO tube of our making 
//Fails when trying to store more than 6 elements
void Fifo_Put(int data){
	if (Slots == 8){
		return;				// A full array, aka no luck
	}
		FIFOArray[PutPt] = data;  //Save the data
		Slots++;									//Increase our countr
		if(PutPt == 7){						//Increase our pointer
			PutPt = 0;
			PutA++;
		}
		else{
			PutPt++;
		}
		return;
	}

//This is our function that gets data from our FIFO 
//Fails if the FIFO is empty 
int Fifo_Get(void){ int GetData;
	if(OutPt == PutPt){					//Check if FIFO is empty
		if(OutA == PutA){
			return -1;
		}
	}
	GetData = FIFOArray[OutPt]; //Get our data
	Slots--;										//Decrease elements counter
	if(OutPt == 7){							//Increase our pointer
		OutPt = 0;
		OutA++;
	}
	else{
		OutPt++;
	}
	return(GetData);
}

