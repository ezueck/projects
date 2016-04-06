//This includes our selector of action 
//It is a massive checker of conditions to check whether you can do the action
//Sorry for the mess

#include "Screen.h"

signed int Position1x = 0;
signed int Position1y = 7;
signed int Position2x = 7;
signed int Position2y = 0;

int Direction1 = 1;
int Direction2 = 3;
int ServiceMove1 = 0;
int ServiceMove2 = 0;

//Empy = 0, Bush = 10, Rock = 11, Bomb = 12, P1 = 13, P2 = 14 Explosion = 15
char Map[8][8] = {
	{0, 10, 0, 10, 0, 10, 0, 14},
	{10, 11, 10, 11, 10, 11, 0, 11},
	{0, 10, 0, 10, 0, 10, 0, 0},
	{10, 11, 10, 11, 10, 11, 10, 11},
	{0, 10, 0, 10, 0, 10, 0, 10},
	{0, 11, 10, 11, 10, 11, 10, 11},
	{0, 10, 0, 10, 0, 10, 0, 10},
	{13, 11, 10, 11, 10, 11, 10, 11}
};

void SetBomb(int, int, int, int);
void Move(int, int, int, int);
void ChangeDirection(int, int, int, int);

//This function selects the appropiate function for the input
//It sends the action for both Player 1 or Player 2
//It gets stuck there until the action services

void ActionSelector(int Action1, int Action2){
		if(Action1==10){ 										  			//Set bomb for Player 1
			SetBomb(Direction1, Position1x, Position1y, 1);
		}
		
		else if((Action1!=0) && (Action1 !=10)){ 
			if(Action1 == Direction1){ 								//Move for Player 1
				Move(Direction1, Position1x, Position1y, 1); 
			}
			else if(Direction1!=Action1){
					DisplayDirection1(Action1, Position1x, Position1y);
					Direction1 = Action1; 									//Change our direction for Player 1
			}
		}
		
		if(Action2==10){											 		 //Set bomb for Player 2
			SetBomb(Direction2, Position2x, Position2y, 2);
		}
		
		else if((Action2!=0) && (Action2!=10)){ 
			if(Action2 == Direction2){								//Move for Player 1
				Move(Direction2, Position2x, Position2y, 2);
			}
			else{
				if(Direction2!=Action2){
					DisplayDirection2(Action2, Position2x, Position2y); 						//Change our direction for Player 2
					Direction2 = Action2;
				}
			}
		}
	}


//This function checks if you can put a bomb in that place 
//1) If it's in the bounds 2) If the spot is empty 
void SetBomb(int direction, int x, int y, int player){
	
		//Scenario for placing up
		if			(direction == 1){
			if(((y-1) >= 0) && (Map[x][y-1] ==0)){
				PlaceBomb(x, y-1, player);
			}
		}
		
		//Scenario for placing down
		else if (direction == 2){
			if(((y+1) <= 7) && (Map[x][y+1] ==0)){ // Check if you can set there
				PlaceBomb(x, y+1, player);
			}		
		}
		
		//Scenario for placing left 
		else if (direction == 3){
			if(((x-1) >= 0) && (Map[x-1][y] ==0)){ // Check if you can set there
				PlaceBomb(x-1, y, player);
			}
		}
		else if (direction == 4){
			if(((x+1) <= 7) && (Map[x+1][y] ==0)){ // Check if you can set there
				PlaceBomb(x+1, y, player);
			}
		}
	}


//This function checks whether you can move in that direction 
void Move(int direction, int x, int y, int player){
	
		//Scenario for moving up
		if			(direction == 1){
			if(((y-1) >= 0) && (Map[x][y-1] <10)){
				if(player == 1){
					ServiceMove1 = direction; //Request servicing
					Map[x][y-1] = 13; //Change our matrix values 
					Map[x][y] = 0;
				}
				else if(player ==2){
					ServiceMove2 = direction; //Request servicing
					Map[x][y-1] = 14; //Change our matrix values 
					Map[x][y] = 0;
				}
			}
		}
		
		//Scenario for moving down
		else if (direction == 2){
			if(((y+1) <= 7) && (Map[x][y+1] <10)){ // Check if you can set there
				if(player == 1){
					ServiceMove1 = direction; //Request servicing
					Map[x][y+1] = 13; //Change our matrix values 
					Map[x][y] = 0;
				}
				else if(player ==2){
					ServiceMove2 = direction; //Request servicing
					Map[x][y+1] = 14; //Change our matrix values 
					Map[x][y] = 0;
				}		
			}
		}
		
		//Scenario for moving left 
		else if (direction == 3){
			if(((x-1) >= 0) && (Map[x-1][y] <10)){ // Check if you can set there
				if(player == 1){
					ServiceMove1 = direction; //Request servicing
					Map[x-1][y] = 13; //Change our matrix values 
					Map[x][y] = 0;
				}
				else if(player ==2){
					ServiceMove2 = direction; //Request servicing
					Map[x-1][y] = 14; //Change our matrix values 
					Map[x][y] = 0;
				}
			}
		}
		
		//Scenario for moving right
		else if (direction == 4){
			if(((x+1) <= 7) && (Map[x+1][y] <10)){ // Check if you can set there
				if(player == 1){
					ServiceMove1 = direction; //Request servicing
					Map[x+1][y] = 13; //Change our matrix values 
					Map[x][y] = 0;
				}
				else if(player ==2){
					ServiceMove2 = direction; //Request servicing
					Map[x+1][y] = 14; //Change our matrix values 
					Map[x][y] = 0;
				}
			}
		}
	}

