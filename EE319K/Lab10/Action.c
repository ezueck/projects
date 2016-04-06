//This includes our selector of action 
//It is a massive checker of conditions to check whether you can do the action
//Sorry for the mess

#include "Screen.h"
#include "Systick.h"
#include "Sound.h"
#include "InputOutput.h"
#include "random.h"
#include "tm4c123gh6pm.h"
#include "InputConversion.h"
#include <stdlib.h>

signed int Position1x = 0;
signed int Position1y = 7;
signed int Position2x = 7;
signed int Position2y = 0;
int SetLock1 = 0;
int SetLock2=0;

int Direction1 = 1;
int Direction2 = 3;
int ServiceMove1 = 0;
int ServiceMove2 = 0;
int MoveTimer1 = 0;
int MoveTimer2 = 0;
int BombStatus[10] = {0,0,0,0,0,0,0,0,0,0};
int BombTimer[10] = {0,0,0,0,0,0,0,0,0,0};
int BombX[10] = {0,0,0,0,0,0,0,0,0,0};
int BombY[10] = {0,0,0,0,0,0,0,0,0,0};
int BombNumber = 1;
int BombPlayer[10] = {0,0,0,0,0,0,0,0,0,0};
int PowerUp[3] = {0, 1, 1};
int Lives1 = 3;
int Lives2 = 3;
int DaBombStat[6] = {0, 0, 0, 0, 0, 0};

//Empy = 0, Bush = 10, Rock = 11, Bomb = 12, P1 = 13, P2 = 14 Explosion = 15
int Map[8][8] = {
	{0, 10, 0, 10, 0, 0, 0, 13},
	{10, 11, 10, 11, 10, 11, 10, 11},
	{0, 10, 0, 10, 0, 10, 0, 10},
	{10, 11, 10, 11, 10, 11, 10, 11},
	{0, 10, 0, 10, 0, 10, 0, 10},
	{10, 11, 10, 11, 10, 11, 10, 11},
	{0, 0, 0, 10, 0, 10, 0, 10},
	{14, 11, 0, 11, 10, 11, 10, 0}
};

void SetBomb(int, int, int, int);
void Move(int, int, int, int);
void ChangeDirection(int, int, int, int);
void Explosion(int, int, int);
void ExplosionFixer(int, int, int);
void NewGround(int, int);
void AddPowerCheck(int, int, int);

//This function selects the appropiate function for the input
//It sends the action for both Player 1 or Player 2
//It gets stuck there until the action services

void ActionSelector(int Action1, int Action2){
		if(Action1!=Direction1 && (Action1 !=5)){
			SetLock1 = 0;
		}
		if(Action2!=Direction2 && (Action2 !=5)){
			SetLock2 = 0;
		}
		if(Action1==10){ 										  			//Set bomb for Player 1
			if (DaBombStat[0] == 2  && DaBombStat[3] == 1 && TheTimer>= DaBombStat[5]+30){
				DaBombStat[4] = DaBomb;
				Explosion(DaBombStat[1], DaBombStat[2], DaBombStat[4]);
				DaBombStat[5] = TheTimer;
				DaBombStat[0] = 3;
			}
			else if(DaBombStat[0] != 2){
				SetBomb(Direction1, Position1x, Position1y, 1);
			}
		}
		
		else if((Action1!=0) && (Action1 !=10) && (Action1 !=5)){ 
			if((Action1 == Direction1) && (SetLock1==0)){ 								//Move for Player 1
				Move(Direction1, Position1x, Position1y, 1); 
			}
			else if(Direction1!=Action1 ){
					DisplayDirection1(Action1, Position1x, Position1y);
					Direction1 = Action1; 									//Change our direction for Player 1
				 	SetLock1= 1;
			}
		}
		
		if(Action2==10){											 		 //Set bomb for Player 2
				if (DaBombStat[0] == 2 && DaBombStat[3] == 2 && TheTimer>= DaBombStat[5]+30){
				DaBombStat[4] = DaBomb;
				Explosion(DaBombStat[1], DaBombStat[2], DaBombStat[4]);
				DaBombStat[5] = TheTimer;
				DaBombStat[0] = 3;
				}
				else if(DaBombStat[0] != 2){
					SetBomb(Direction2, Position2x, Position2y, 2);
				}
			}
		else if((Action2!=0) && (Action2 !=10) && (Action2 !=5)){ 
			if((Action2 == Direction2) && (SetLock2==0)){								//Move for Player 1
				Move(Direction2, Position2x, Position2y, 2);
			}
			else if(Direction2!=Action2 ){
					DisplayDirection2(Action2, Position2x, Position2y); 						//Change our direction for Player 2
					Direction2 = Action2;
					SetLock2 =1;
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
					AddPowerCheck(x, y-1, player);
					Map[x][y-1] = 13; //Change our matrix values 
					Map[x][y] = 0;
					MoveTimer1 = TheTimer;
					Position1y = Position1y-1;
				}
				else if(player ==2){
					ServiceMove2 = direction; //Request servicing
					AddPowerCheck(x, y-1, player);
					Map[x][y-1] = 14; //Change our matrix values 
					Map[x][y] = 0;
					MoveTimer2 = TheTimer;
					Position2y = Position2y-1;
				}
			}
		}
		
		//Scenario for moving down
		else if (direction == 2){
			if(((y+1) <= 7) && (Map[x][y+1] <10)){ // Check if you can set there
				if(player == 1){
					Map[x][y+1] = 13; //Change our matrix values 
					AddPowerCheck(x, y+1, player);
					ServiceMove1 = direction; //Request servicing
					Map[x][y] = 0;
					MoveTimer1 = TheTimer;
					Position1y = Position1y+1;
				}
				else if(player ==2){
					ServiceMove2 = direction; //Request servicing
					AddPowerCheck(x, y+1, player);
					Map[x][y+1] = 14; //Change our matrix values 
					Map[x][y] = 0;
					MoveTimer2 = TheTimer;
					Position2y = Position2y+1;
				}		
			}
		}
		
		//Scenario for moving left 
		else if (direction == 3){
			if(((x-1) >= 0) && (Map[x-1][y] <10)){ // Check if you can set there
				if(player == 1){
					ServiceMove1 = direction; //Request servicing
					AddPowerCheck(x-1, y, player);
					Map[x-1][y] = 13; //Change our matrix values
					Map[x][y] = 0;
					MoveTimer1 = TheTimer;
					Position1x = Position1x-1;
					
				}
				else if(player ==2){
					ServiceMove2 = direction; //Request servicing
					AddPowerCheck(x-1, y, player);
					Map[x-1][y] = 14; //Change our matrix values 
					Map[x][y] = 0;
					MoveTimer2 = TheTimer;
					Position2x = Position2x-1;
				}
			}
		}
		
		//Scenario for moving right
		else if (direction == 4){
			if(((x+1) <= 7) && (Map[x+1][y] <10)){ // Check if you can set there
				if(player == 1){
					ServiceMove1 = direction; //Request servicing
					AddPowerCheck(x+1, y, player);
					Map[x+1][y] = 13; //Change our matrix values 
					Map[x][y] = 0;
					MoveTimer1 = TheTimer;
					Position1x = Position1x+1;
				}
				else if(player ==2){
					ServiceMove2 = direction; //Request servicing
					Map[x+1][y] = 14; //Changeour matrix values 
					AddPowerCheck(x+1, y, player);
					Map[x][y] = 0;
					MoveTimer2 = TheTimer;
					Position2x = Position2x+1;
				}
			}
		}
	}

//This is our main bomb handler 
//It checks whether we need to change the color of the bomb or if it should explode
//It's input is the number of the array for our make shift structure 
//If enough time has passes, as given by Timer, it explodes 
void BombHandler(int number){
	if(BombStatus[number]<5){
		if(TheTimer>= (BombTimer[number] + 15)){
			BombStatus[number]++;
			if(BombStatus[number]%2 == 0){
				TurnBombRed(BombX[number], BombY[number]);
			}
			if(BombStatus[number]%2 == 1){
				TurnBombBlack(BombX[number], BombY[number]);
			}
			BombTimer[number] = TheTimer;
		}
	}
	else if(BombStatus[number]>=5 && BombStatus[number]<= 10){
		if(TheTimer>= (BombTimer[number] + 3)){
			BombStatus[number]++;
			if(BombStatus[number]%2 == 0){
				TurnBombRed(BombX[number], BombY[number]);
			}
			if(BombStatus[number]%2 == 1){
				TurnBombBlack(BombX[number], BombY[number]);
			}
			BombTimer[number] = TheTimer;
		}
	}	
	else if(BombStatus[number]>10 && BombStatus[number]!=100){
		Explosion(BombX[number], BombY[number], PowerUp[BombPlayer[number]]);
		BombStatus[number] = 100;
		BombTimer[number] = TheTimer;
	}
	else if(BombStatus[number] ==100){
		if(TheTimer>= (BombTimer[number] + 15)){
			ExplosionFixer(BombX[number], BombY[number], PowerUp[BombPlayer[number]]);
			BombX[number] = 0;
			BombY[number] = 0;
			BombTimer[number] = 0;
			BombStatus[number] = 0;
		}
	}
}

//This is the function that prints explosions 
//It is detemiens by the position of the bomb and the distance/powerup of the player 
//It checks whether a place should explode or not and then prints the fire
void Explosion(int x, int y, int distance){ int i = 1; int Up =0; int Down = 0; int Left = 0; int Right = 0;
	Play_Sound(2);
	PrintExplosion(x,y);
	while(i<=distance){
		if(((y-i) >= 0) && (Map[x][y-i] != 11 && Up == 0)){
			PrintExplosion(x, y-i);
		}
		else if((Map[x][y-i]) == 11 && Up == 0){
			Up = 1;
		}
		if(((y+i) <= 7) && (Map[x][y+i] != 11 && Down == 0)){
			PrintExplosion(x, y+i);
		}
		else if((Map[x][y+i]) == 11 && Down == 0){
			Down = 1;
		}
		if(((x+i) <= 7) && (Map[x+i][y] != 11 && Left == 0)){
			PrintExplosion(x+i, y);
		}
		else if((Map[x-i][y]) == 11 && Left == 0){
			Left = 1;
		}
		if(((x-i) >= 0) && (Map[x-i][y] != 11 && Right == 0)){
			PrintExplosion(x-i, y);
		}
		else if((Map[x+i][y]) == 11 && Right == 0){
			Right = 1;
		}
		i++;
	}
}

//This is the function that checks whether a block needs to be reprintes after an explosion
//It uses the same mechanism as the Explosion itself 
//Distance is determines by the power up of the player
//If a new block is needed it uses the NewGround function to print
void ExplosionFixer(int x, int y, int distance){ int i = 1;
	PrintEmpty(x,y);
	Map[x][y] = 0;
	while(i<=distance){
		if(((y-i) >= 0) && (Map[x][y-i] != 11)){
			NewGround(x, y-i);
		}
		if(((y+i) <= 7) && (Map[x][y+i] != 11)){
			NewGround(x, y+i);
		}
		if(((x+i) <= 7) && (Map[x+i][y] != 11)){
			NewGround(x+i, y);
		}
		if(((x-i) >= 0) && (Map[x-i][y] != 11)){
			NewGround(x-i, y);
		}
		i++;
	}
}

//This function reverses the damage of the explosions 
//Blocks that used to be bushes are now empty
//There's a chance for there to be a PowerUp in the place of a bush 
//It also updates the hearts each player has
void NewGround(int x, int y){ int m = rand();
	if (Map[x][y] ==10 || Map[x][y] == 5 || Map[x][y] == 6){
		if( m%10 == 0 && DaBombStat[0] == 0){
			PrintDaBombItem(x, y);
			Map[x][y] = 6;
		}
		else if( m%5 == 0){
			PrintPowerUp(x, y);
			Map[x][y] = 5;
		}
		else{
			Map[x][y] = 0;
			PrintEmpty(x, y);
		}
	}
	else if (Map[x][y] ==13){
		DisplayDirection1(Direction1, x,y);
		Lives1--;
		LivesOne(Lives1);
		Play_Sound(3);
	}
	else if (Map[x][y] ==14){
		DisplayDirection2(Direction2, x,y);
		Lives2--;
		LivesTwo(Lives2);
		Play_Sound(3);
	}
	else if(Map[x][y] ==0){
		PrintEmpty(x, y);
	}
}

//It checks whether we get a new power up or not 
void AddPowerCheck(int x, int y, int player){
	if( Map[x][y] == 5 && PowerUp[player] <5 ){
		PowerUp[player]++;
		PrintPower(player, PowerUp[player]);
	}
	
	if( Map[x][y] == 6){
		DaBombStat[0] = 1;

		DaBombStat[3] = player;
	}
}
