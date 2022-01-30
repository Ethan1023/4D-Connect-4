#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

int main (int argc, char* argv[]){
	rungame();
}

void rungame(){
	uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE];
	set(state, 0);

	int endpoints = 2;
	int endamount = 0;

	int points = 2;
	int players = 0;
	printf("Supports from 1 to 254 players,\n1 to 6 will be shown in colour,\nand extra spacing will be used once player 100 plays\n\n");
	printf("Enter number of players: ");
	while(players<1 || players >254){
		if(scanf("%u", &players)==0)
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
	print(state);

	while(run){//input!=exit
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
		if(strcmp(input, "exit")==0){
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
	}
}

uint32_t haswon(uint8_t state[ARRAYSIZE][ARRAYSIZE][ARRAYSIZE][ARRAYSIZE], uint8_t player, uint8_t scoring){
	//check 0,0,0,0 in all 15 directions
	//check all coordinates with 3 zeros in 7 directions
	//check all coordinates with 2 zeros in 3 directions
	//check all coordinates with 1 zero in 1 direction
	//one less than the power of 2 to the number of 0s, cos going 0 in all directions is not a direction
	//go through all indicies, skip if no coordinates are 0, incrememnt from 1 to 15, define temporary tracking variables, skip value if all positive bits in increment are not 0 in i j k l, count in line and see if all have players number
	//lol watch out for the bit level crap, easier than messing around with a bunch of variables
	uint32_t points = 0;
	for(uint8_t i=0;i<ARRAYSIZE;i++){
		for(uint8_t j=0;j<ARRAYSIZE;j++){
			for(uint8_t k=0;k<ARRAYSIZE;k++){
				for(uint8_t l=0;l<ARRAYSIZE;l++){
					if(state[i][j][k][l]==player && !(!!i&!!j&!!k&!!l)){
						for(uint8_t t=1; t<16; t++){
							if(!((!!i & !!(t&0x01)) | (!!j&!!(t&0x02)) | (!!k&!!(t&0x04)) | (!!l&!!(t&0x08)))){
								uint8_t I=i;uint8_t J=j;uint8_t K=k;uint8_t L=l;
								int line = 1;
								for(int s=1;s<ARRAYSIZE;s++){
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
					if(state[i][j][k][l]>players)
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
						default:
							printf("%u", state[i][j][k][l]);
					}
					if(state[i][j][k][l]<10)
						printf(" ");
					if(players>99 && state[i][j][k][l]<100)
						printf(" ");
				}
				printf("   ");
			}
			printf("\n");
		}
	printf("\n");
	}
}
