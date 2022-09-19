#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<signal.h>

#define key 1234

int memory, semaphores;
char* board;
char winner;

void finish();
void show();
void play(char);
int check();
int readSign();

int main(){

	char player;
	struct sembuf wait_0 = {0, -1, 0}, signal_1 = {1, 1, 0}, 
		      wait_1 = {1, -1, 0}, signal_0 = {0, 1, 0}, 
		      *wait, *signal;
	
	memory = shmget(key, 256, 0777| IPC_CREAT);
	board = shmat(memory, 0, 0);

	if((semaphores = semget(key, 2, 0777 | IPC_CREAT | IPC_EXCL)) != -1){
		player = 'X';
	
		wait = &wait_0;
		signal = &signal_1;
		semctl(semaphores, 0, SETVAL, 1);
		semctl(semaphores, 1, SETVAL, 0);
	}
	else{
		semaphores = semget(key, 2, 0777 | IPC_CREAT);
		system("clear");
		printf("WAITING FOR THE OPPONENT TO START THE GAME.\nYOUR SIGN [O]\n");
		player = 'O';
		wait = &wait_1;
		signal = &signal_0;
	}

	for(int i = 0; i < 9; i++){
		board[i] = i + 48;
	}

	show();

	while(1){

		semop(semaphores, wait, 1);
		system("clear");
		
		if(board[0] == 'L'){
			printf("LOSER\n");
			finish();
		}
		else if(board[0] == 'D'){
			printf("DRAW\n");
			finish();
		}

		//check(player);
		play(player);
		//check(player);
		
		if(check() == 1){
			printf("WINNER\n");
			board[0] = 'L';
			finish();
		}
		else if(check() == 2){
			printf("DRAW\n");
			board[0] = 'D';
			finish();
		}
		
		semop(semaphores, signal, 1);
		show();
	}

	return 0;
}
void finish(){
	semctl(semaphores, 0, IPC_RMID, 0);
	shmdt(board);
	shmctl(memory, IPC_RMID, 0);
	exit(0);
}

void show(){
	printf("     |     |\n");
	printf("  %c  |  %c  |  %c\n", *(board), *(board + 1), *(board + 2));
	printf("_____|_____|_____\n");
	printf("     |     |\n");
	printf("  %c  |  %c  |  %c\n", *(board + 3), *(board + 4), *(board + 5));
	printf("_____|_____|_____\n");
	printf("     |     |\n");
	printf("  %c  |  %c  |  %c\n", *(board + 6), *(board + 7), *(board + 8));
	printf("     |     |\n");
}
void play(char player){
	show();
	printf("YOUR TURN. INSERT [%c]\n", player);
	*(board + readSign()) = player;
	system("clear");
}
int check(){
	//horizontal
	if(board[0] == board[1] && board[1] == board[2]){return 1;}
	else if(board[3] == board[4] && board[4] == board[5]){return 1;}
	else if(board[6] == board[7] && board[7] == board[8]){return 1;}
	//vertical
	else if(board[0] == board[3] && board[3] == board[6]){return 1;}
	else if(board[1] == board[4] && board[4] == board[7]){return 1;}
	else if(board[2] == board[5] && board[5] == board[8]){return 1;}
	//diagonaly
	else if(board[0] == board[4] && board[4] == board[8]){return 1;}
	else if(board[2] == board[4] && board[4] == board[6]){return 1;}
	//draw
	else if(board[0] != '0' && board[1] != '1' && board[2] != '2' && 
			board[3] != '3' && board[4] != '4' && board[5] != '5' &&
			board[6] != '6' && board[7] != '7' && board[8] != '8'){return 2;}
	return 0;
}
int readSign(){
	do{
		int field;
		scanf("%d", &field);
		if(field >= 0 && field <= 8){
			for(int i = 0; i < 9; i++){
				if(board[i] == field + 48){
					return field;
				}
			}
		}
		printf("ENTER CORRECT NUMBER:\n");
	}while(1);	
}
