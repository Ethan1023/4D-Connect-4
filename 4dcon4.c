#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define ANSI_COLOUR_RED     "\x1b[31m"
#define ANSI_COLOUR_GREEN   "\x1b[32m"
#define ANSI_COLOUR_YELLOW  "\x1b[33m"
#define ANSI_COLOUR_BLUE    "\x1b[34m"
#define ANSI_COLOUR_MAGENTA "\x1b[35m"
#define ANSI_COLOUR_CYAN    "\x1b[36m"
#define ANSI_COLOUR_RESET   "\x1b[0m"
#define ARRAYSIZE 4

void rungame();
void set(uint8_t [ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t);
void print(uint8_t [ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE]);
uint32_t haswon(uint8_t [ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t, uint8_t);
uint32_t findfourth(uint8_t [ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t, uint8_t*, uint8_t*, uint8_t*, uint8_t*);
uint8_t aiplay(uint8_t [ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t, uint8_t, uint8_t*, uint16_t);
void linecount(uint8_t [ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*, uint8_t*);
int rungameauto(int aiplayers, int points, int endpoints, int endamount, uint8_t [ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE]);
uint8_t contains(uint8_t [ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t);
void runautogames();

int main (int argc, char* argv[]){
	rungame();
	//runautogames();
}

void runautogames(){
	srand((unsigned) time(NULL)); // Begin random number generator for AI
	int wins[3] = {0, 0, 0};
	for(int i=0; i<1000; i++){
		uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE];
		set(state, 0);
		int winner = rungameauto(2, 0, 0, 0, state);
		wins[winner] += 1;
		//if(winner > 0)
		//	printf("AI %u won!\n", winner);
		//else
		//	printf("Draw\n");
		//print(state);
	}
	printf("%u draws\n", wins[0]);
	for(int i=1; i<3; i++){
		printf("AI %u won %u times\n", i, wins[i]);
	}
}

int rungameauto(int aiplayers, int points, int endpoints, int endamount, uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE]){
	// players - total
	// aiplayers - how many of these are ai
	// points - whether to use points
	// endpoints - how many points to end at (ignored if points is 0)

	int players = aiplayers;

	int player = 1;
	uint8_t run = 1;
	char input[10];

	while(run){//input!=exit
		//printf("Player %u is computer controlled\n", player);
		uint8_t played[4];
		uint16_t r = rand();
		uint8_t didplay = aiplay(state, player, players, played, r);
		if(didplay)
			//printf("Computer playing at %u, %u, %u, %u\n", played[2]+1, played[0]+1, played[3]+1, played[1]+1);
			if(points==0){
				if(haswon(state, player, 0)!=0){
					//printf("PLAYER %u HAS WON!!!\n", player);
					//print(state);
					//haswon(state, player, 1);
					return player;
				}
			}else{
				//printf("Player %u has %u points\n", player, haswon(state, player, 0));
				if(endpoints && haswon(state, player, 0)==endamount)
					//printf("Player %u has won the game!\n", player);
					return player;
			}
		//print(state);
		if(player>=players)
			player=1;
		else
			player++;
		run = contains(state, 0);
	}
	//print(state);
	return 0;
}


void rungame(){
	uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE];
	set(state, 0);

	int endpoints = 2;
	int endamount = 0;

	int points = 2;
	int players = 0;
	int aiplayers = -1;
	printf("Supports from 1 to 254 players,\n1 to 6 will be shown in colour,\nand extra spacing will be used once player 100 plays\n\n");
	printf("Enter number of players: ");
	while(players<1 || players >254){
		if(scanf("%u", &players)==0)
			getchar();
	}
	printf("How many of these players should be computer controlled? ");
	while(aiplayers<0 || aiplayers > players){
		if(scanf("%u", &aiplayers)==0)
			getchar();
	}
	printf("Points?(y/n) ");
	while(points==2){
		char input[10];
		scanf("%s", input);
		if(strcmp(input, "y")==0){	points=1;}
		else if(strcmp(input, "yes")==0){	points=1;}
		else if(strcmp(input, "Yes")==0){	points=1;}
		else if(strcmp(input, "YES")==0){	points=1;}
		else if(strcmp(input, "n")==0){	points=0;}
		else if(strcmp(input, "no")==0){	points=0;}
		else if(strcmp(input, "No")==0){	points=0;}
		else if(strcmp(input, "NO")==0){	points=0;}
	}
	if(points){
		while(endpoints==2){
			printf("Choose end mode, points/turns/never: ");
			char input[10];
			scanf("%s", input);
			if(strcmp(input, "points")==0){
				endpoints = 1;
				while(endamount==0){
					printf("How many points? ");
					if(scanf("%u", &endamount)==0)
						getchar();
				}
			}else if(strcmp(input, "turns")==0)
				endpoints = 0;
			else if(strcmp(input, "never")==0){
				endpoints = 1;
				endamount = 369;
			}
		}
	}
	int player = 1;
	uint8_t run = 1;
	char input[10];

	srand((unsigned) time(NULL)); // Begin random number generator for AI

	print(state);

	while(run){//input!=exit
		if(player <= players-aiplayers){
			if(points==1){
				for(int i=0;i<players;i++){
					printf("Player %u has %u points\n", i+1, haswon(state, i+1, 0));
				}
			}
			if(points)
				printf("Enter command player %d (show/play/exit/print/score): ", player);
			else
				printf("Enter command player %d (show/play/exit/print): ", player);
			scanf("%s", input);
		}
		if(player > players-aiplayers){
			printf("Player %u is computer controlled\n", player);
			uint8_t played[4];
			uint16_t r = rand();
			uint8_t didplay = aiplay(state, player, players, played, r);
			if(didplay)
				printf("Computer playing at %u, %u, %u, %u\n", played[2]+1, played[0]+1, played[3]+1, played[1]+1);
				if(points==0){
					if(haswon(state, player, 0)!=0){
						printf("PLAYER %u HAS WON!!!\n", player);
						print(state);
						haswon(state, player, 1);
						exit(0);
					}
				}else{
					printf("Player %u has %u points\n", player, haswon(state, player, 0));
					if(endpoints && haswon(state, player, 0)==endamount)
						printf("Player %u has won the game!\n", player);
				}
			print(state);
			if(player>=players)
				player=1;
			else
				player++;
		}else if(strcmp(input, "exit")==0){
			run=0;
		}else if(strcmp(input, "print")==0){
			printf("Currently player %u's turn\n", player);
			print(state);
		}else if(strcmp(input, "show")==0){
			int Y=0;
			int y=0;
			int X=0;
			int x=0;
			printf("Enter coordinates: ");
			scanf("%u %u %u %u", &X, &Y, &x, &y);
			if(X>0 && X<ARRAYSIZE+1 && Y>0 && Y<ARRAYSIZE+1 && x>0 && x<ARRAYSIZE+1 && y>0 && y<ARRAYSIZE+1){
				X-=1;Y-=1;x-=1;y-=1;
				int temp = state[Y][y][X][x];
				if(temp!=0)
					printf("Position is currently taken by player %u\n", temp);
				else
					printf("Position is free to take\n");
				state[Y][y][X][x] = 255;
				print(state);
				state[Y][y][X][x] = temp;
			}else{
				printf("Error, value out of range\n");
			}
		}else if(strcmp(input, "play")==0){
			int Y=0;
			int y=0;
			int X=0;
			int x=0;
			printf("Enter coordinates: ");
			scanf("%u %u %u %u", &X, &Y, &x, &y);
			if(X>0 && X<ARRAYSIZE+1 && Y>0 && Y<ARRAYSIZE+1 && x>0 && x<ARRAYSIZE+1 && y>0 && y<ARRAYSIZE+1){
				X-=1;Y-=1;x-=1;y-=1;
				int temp = state[Y][y][X][x];
				if(temp!=0)
					printf("Position is already taken by player %u\n", temp);
				else{
					state[Y][y][X][x]=player;
					printf("Position sucessfully taken by player %u\n", player);
					if(points==0){
						if(haswon(state, player, 0)!=0){
							printf("PLAYER %u HAS WON!!!\n", player);
							print(state);
							haswon(state, player, 1);
							exit(0);
						}
					}else{
						printf("Player %u has %u points\n", player, haswon(state, player, 0));
						if(endpoints && haswon(state, player, 0)==endamount)
							printf("Player %u has won the game!\n", player);
					}
					if(player>=players)
						player=1;
					else
						player++;
					printf("Player %u's turn\n", player);
					print(state);
				}
			}else{
				printf("Error, value out of range\n");
			}
		}else if(points && (strcmp(input, "score")==0)){//print how all scores were calculated
			for(int i=1;i<=players;i++){
				if(haswon(state, player, 1)==endamount)
					printf("Player %u has won the game!\n", player);
			}
		}else{
			printf("Error, invalid command\n");
		}
		if(contains(state, 0) == 0){
			printf("No more moves possible\n");
			run = 0;
		}
	}
}

uint8_t aiplay(uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t player, uint8_t players, uint8_t* played, uint16_t r){
	// if can win, play
	// if other about to win, block
	// if a point has 2 in 2 different directions, play  (set score to 100
	// if another player has 2 in 2 different directions, play  (set score to 80
	// for each point; for each direction clear; add 2 score for 1 of own, and 6 for 2 of own, subtract 1 for any of other (max 16 directions, 1 with 2 and 15 with 1, max score is 17
	// play at highest score
	// if all scores 0, play at random location with least other player pieces per direction
	uint8_t Ycoord, ycoord, Xcoord, xcoord;
	uint8_t fourthfound = findfourth(state, player, &Ycoord, &ycoord, &Xcoord, &xcoord);
	if(fourthfound == 1){
		state[Ycoord][ycoord][Xcoord][xcoord] = player;
		played[0] = Ycoord;
		played[1] = ycoord;
		played[2] = Xcoord;
		played[3] = xcoord;
		return 1;
	}
	for(int play = 1; play <= players; play++){
		if(play != player){
			uint8_t fourthfound = findfourth(state, play, &Ycoord, &ycoord, &Xcoord, &xcoord);
			if(fourthfound == 1){
				state[Ycoord][ycoord][Xcoord][xcoord] = player;
				played[0] = Ycoord;
				played[1] = ycoord;
				played[2] = Xcoord;
				played[3] = xcoord;
				return 1;
			}
		}
	}
	// uint8_t self1[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE];
	// uint8_t self2[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE];
	// uint8_t other1[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE];
	// uint8_t other2[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE];
	// set(self1, 0);
	// set(self2, 0);
	// set(other1, 0);
	// set(other2, 0);
	uint8_t score[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE];
	uint8_t baselevel = 20;
	set(score, baselevel);
	uint8_t maxscore = baselevel;
	uint32_t nummaxscore = 0;
	for(uint8_t i=0;i<ARRAYSIZE;i++){																					// Iterate through all positions
		for(uint8_t j=0;j<ARRAYSIZE;j++){
			for(uint8_t k=0;k<ARRAYSIZE;k++){
				for(uint8_t l=0;l<ARRAYSIZE;l++){
					if(state[i][j][k][l]==0){																			// If position clear 
						uint8_t self1 = 0;
						uint8_t self2 = 0;
						uint8_t other1 = 0;
						uint8_t other2 = 0;
						for(uint8_t t=1; t<16; t++){																	// Iterate through all directions
							uint8_t self = 0;
							uint8_t other = 0;
							linecount(state, player, i, j, k, l, t, &self, &other);
							if(self == 1 && other == 0)
								self1 += 1;
							if(self == 2 && other == 0)
								self2 += 1;
							if(self == 0 && other == 1)
								other1 += 1;
							if(self == 0 && other == 2)
								other2 += 1;
						}
						if(self2 >= 2)
							score[i][j][k][l] = 100;
						else if(other2 >= 2)
							score[i][j][k][l] = 80;
						else {
							score[i][j][k][l] = baselevel + 6 * self2 + 2 * self1 - (other1 + other2);
							if(score[i][j][k][l] > 150)
								score[i][j][k][l] = 0;
						}
						if(score[i][j][k][l] > maxscore){
							maxscore = score[i][j][k][l];
							nummaxscore = 1;
						} else if(score[i][j][k][l] == maxscore)
							nummaxscore ++;
					} else {
						score[i][j][k][l] = 0;
					}
				}
			}
		}
	}
	//printf("maxscore = %u", maxscore);
	//print(score);
	uint32_t nummaxscorecount = 0;
	for(uint8_t i=0;i<ARRAYSIZE;i++){																					// Iterate through all positions
		for(uint8_t j=0;j<ARRAYSIZE;j++){
			for(uint8_t k=0;k<ARRAYSIZE;k++){
				for(uint8_t l=0;l<ARRAYSIZE;l++){
					if(score[i][j][k][l] == maxscore){
						if(nummaxscorecount == (r%nummaxscore)){
							state[i][j][k][l] = player;
							played[0] = i;
							played[1] = j;
							played[2] = k;
							played[3] = l;
							return 1;
						} else
							nummaxscorecount ++;
					}
				}
			}
		}
	}
	printf("Error! AI failed\n");
	print(state);


	return 0;
}

void linecount(uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t player, uint8_t Ycoord, uint8_t ycoord, uint8_t Xcoord, uint8_t xcoord, uint8_t direction, uint8_t* self, uint8_t* other){
	// Accept coordinates, player and direction, find number of player and other player pieces
	uint8_t I=Ycoord; uint8_t J=ycoord; uint8_t K=Xcoord; uint8_t L=xcoord;
	while(I < ARRAYSIZE && J < ARRAYSIZE && K < ARRAYSIZE && L < ARRAYSIZE){
		if(state[I][J][K][L]==player){
			*self += 1;
		}
		if(state[I][J][K][L]!=player && state[I][J][K][L]!=0){
			*other += 1;
		}
		I+=(direction & 0x01);
		J+=(direction & 0x02)>>1;
		K+=(direction & 0x04)>>2;
		L+=(direction & 0x08)>>3;
	}
	I=Ycoord; J=ycoord; K=Xcoord; L=xcoord;
	I-=(direction & 0x01);
	J-=(direction & 0x02)>>1;
	K-=(direction & 0x04)>>2;
	L-=(direction & 0x08)>>3;
	while(I < 255 && J < 255 && K < 255 & L < 255){  // Can't just do >=0, as it is unsigned, so 0-1 wraps around to 255
		if(state[I][J][K][L]==player){
			*self += 1;
		}
		if(state[I][J][K][L]!=player && state[I][J][K][L]!=0){
			*other += 1;
		}
		I-=(direction & 0x01);
		J-=(direction & 0x02)>>1;
		K-=(direction & 0x04)>>2;
		L-=(direction & 0x08)>>3;
	}
}

uint32_t haswon(uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t player, uint8_t scoring){
	//check 0,0,0,0 in all 15 directions
	//check all coordinates with 3 zeros in 9 directions
	//check all coordinates with 2 zeros in 3 directions
	//check all coordinates with 1 zero in 1 direction
	//one less than the power of 2 to the number of 0s, cos going 0 in all directions is not a direction
	//go through all indicies, skip if no coordinates are 0, incrememnt from 1 to 15, define temporary tracking variables, skip value if all positive bits in increment are not 0 in i j k l, count in line and see if all have players number
	//lol watch out for the bit level crap, easier than messing around with a bunch of variables
	uint32_t points = 0;																								// Track total points
	for(uint8_t i=0;i<ARRAYSIZE;i++){																					// Iterate through all positions
		for(uint8_t j=0;j<ARRAYSIZE;j++){
			for(uint8_t k=0;k<ARRAYSIZE;k++){
				for(uint8_t l=0;l<ARRAYSIZE;l++){
					if(state[i][j][k][l]==player &&	(!i||!j||!k||!l)){													// If player played in location and at least one of coordinates is 0
						for(uint8_t t=1; t<16; t++){																	// Iterate through all directions	
							if((!i|!(t&0x01)) && (!j|!(t&0x02)) && (!k|!(t&0x04)) && (!l|!(t&0x08))){					// Can only go 4 in a direction if starting from zero - each coord must be zero or corresponding coord in vector must be zero
								uint8_t I=i;uint8_t J=j;uint8_t K=k;uint8_t L=l;
								int line = 1;
								for(int s=1;s<ARRAYSIZE;s++){															// Check if all states along vector are the same
									I+=(t & 0x01);
									J+=(t & 0x02)>>1;
									K+=(t & 0x04)>>2;
									L+=(t & 0x08)>>3;
									if(state[I][J][K][L]!=player)
										line = 0;
								}
								if(line){
									points++;
									if(scoring){
										printf("Player point is %u\n", player);
										printf("Starting from %u %u %u %u in direction %u %u %u %u to %u %u %u %u\n", k+1, i+1, l+1, j+1, (t&0x04)>>2, (t&0x01), (t&0x08)>>3, (t&0x02)>>1, K+1, I+1, L+1, J+1);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return points;
}

uint8_t contains(uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t search){
	for(uint8_t i=0;i<ARRAYSIZE;i++){																					// Iterate through all positions
		for(uint8_t j=0;j<ARRAYSIZE;j++){
			for(uint8_t k=0;k<ARRAYSIZE;k++){
				for(uint8_t l=0;l<ARRAYSIZE;l++){
					if(state[i][j][k][l] == search)
						return 1;
				}
			}
		}
	}
	return 0;
}

uint32_t findfourth(uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t player, uint8_t* icoord, uint8_t* jcoord, uint8_t* kcoord, uint8_t* lcoord){
	uint8_t returnvalue = 0;
	for(uint8_t i=0;i<ARRAYSIZE;i++){																					// Iterate through all positions
		for(uint8_t j=0;j<ARRAYSIZE;j++){
			for(uint8_t k=0;k<ARRAYSIZE;k++){
				for(uint8_t l=0;l<ARRAYSIZE;l++){
					if(state[i][j][k][l]==player){																		// If player played in location
						for(uint8_t t=1; t<16; t++){																	// Iterate through all directions
							uint8_t I=i;uint8_t J=j;uint8_t K=k;uint8_t L=l;
							int line = 0;
							//for(int s=1;s<ARRAYSIZE;s++){															// Check if all states along vector are the same
							returnvalue = 0;
							while(I < ARRAYSIZE && J < ARRAYSIZE && K < ARRAYSIZE && L < ARRAYSIZE){
								if(state[I][J][K][L]==player){
									line +=1;
								}
								if(state[I][J][K][L]!=player){
									*icoord = I; *jcoord = J; *kcoord = K; *lcoord = L;
									if(state[I][J][K][L] == 0){
										returnvalue = 1;
									}
								}
								I+=(t & 0x01);
								J+=(t & 0x02)>>1;
								K+=(t & 0x04)>>2;
								L+=(t & 0x08)>>3;
							}
							if(line == ARRAYSIZE-1){  // Player has 3 in a row, find 4th spot
								if(returnvalue){  // If 4th spot already found
									return returnvalue;
								}
								uint8_t I=i;uint8_t J=j;uint8_t K=k;uint8_t L=l;
								while(I < 255 && J < 255 && K < 255 && L < 255){  // Can't just do >=0, as it is unsigned, so 0-1 wraps around to 255
									if(state[I][J][K][L] != player){
										*icoord = I; *jcoord = J; *kcoord = K; *lcoord = L;
										if(state[I][J][K][L] == 0)
											return 1;
									}
									I-=(t & 0x01);
									J-=(t & 0x02)>>1;
									K-=(t & 0x04)>>2;
									L-=(t & 0x08)>>3;
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
	


void set(uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t setto){
	for(int i=0; i<ARRAYSIZE; i++){
		for(int j=0; j<ARRAYSIZE; j++){
			for(int k=0; k<ARRAYSIZE; k++){
				for(int l=0; l<ARRAYSIZE; l++){
					state[i][j][k][l] = setto;
				}
			}
		}
	}
}

void print(uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE]){
	int players = 99;
	for(uint8_t i=0; i<ARRAYSIZE; i++){
		for(uint8_t j=0; j<ARRAYSIZE; j++){
			for(uint8_t k=0; k<ARRAYSIZE; k++){
				for(uint8_t l=0; l<ARRAYSIZE; l++){
					if(state[i][j][k][l]>players && state[i][j][k][l] != 255)
						players = state[i][j][k][l];
				}
			}
		}
	}
	for(uint8_t i=0; i<ARRAYSIZE; i++){
	printf("\n");
		for(uint8_t j=0; j<ARRAYSIZE; j++){
			for(uint8_t k=0; k<ARRAYSIZE; k++){
				for(uint8_t l=0; l<ARRAYSIZE; l++){
					switch (state[i][j][k][l]){
						case 0:
							printf("0");
							break;
						case 1:
							printf(ANSI_COLOUR_RED "1" ANSI_COLOUR_RESET);
							break;
						case 2:
							printf(ANSI_COLOUR_GREEN "2" ANSI_COLOUR_RESET);
							break;
						case 3:
							printf(ANSI_COLOUR_YELLOW "3" ANSI_COLOUR_RESET);
							break;
						case 4:
							printf(ANSI_COLOUR_BLUE "4" ANSI_COLOUR_RESET);
							break;
						case 5:
							printf(ANSI_COLOUR_MAGENTA "5" ANSI_COLOUR_RESET);
							break;
						case 6:
							printf(ANSI_COLOUR_CYAN "6" ANSI_COLOUR_RESET);
							break;
						case 255:
							printf(" ");
							break;
						default:
							printf("%u", state[i][j][k][l]);
					}
					if(state[i][j][k][l]<10 || state[i][j][k][l] == 255)
						printf(" ");
					if(players>99 && (state[i][j][k][l]<100 || state[i][j][k][l]==255))
						printf(" ");
				}
				printf("   ");
			}
			printf("\n");
		}
	printf("\n");
	}
}
