#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

char board[3][3] ;
 int scoreP1=0;
int scoreP2=0;
int draw=0;

void resetBoar(
{
char num = '1';
for( int i=0 ; i<3 ; i++){
for(int j=0 ; j<3 ; j++) {
board[i][j] = num++;
}
}
}

void displayBoard() {
  printf("\n");
printf("
