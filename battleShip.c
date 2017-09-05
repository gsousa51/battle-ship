
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <unistd.h>
#include <string.h>
# include <ctype.h>
int userBoard[6][6]= {{0}};
int compBoard[6][6]= {{0}};
int pilotSunk=0;
int bShipSunk=0;
int aCarrSunk=0;
int gameOver=0;

void buildCompBoard();
void buildUserBoard();
int checkInsertComp(int, int, int,int);
int checkInsertUser(int, char*);
int checkInsertHor(int[][6], int, int, int);
int checkInsertVert(int[][6], int, int, int);
void insertHor(int[][6], int, int, int);
void insertVert(int[][6], int, int, int);
void printUser();
void printComp();
void userTurn();
void checkGameWonUser();
int checkSunkShip(int);
void checkSunkUser();
void compTurn();
void randomTurn();
void coordinatedStrike();
void checkSunkComp();
int checkSunkShipComp(int);
int validMove();
int checkCompWin();
int checkUserWin();

typedef struct n{
   int sRow;
   int sCol;
   int lsRow;
   int lsCol;
   int rDirect;
   int cDirect;
   int hit;
   int acSunk;
   int bsSunk;
   int pSunk;
   
}node;

struct n* comp;

int main(){
   comp=malloc(sizeof(node));
   comp->hit = 0;
   comp->lsRow=-1;
   comp->lsCol=-1;
   comp->sRow=-1;
   comp->sCol=-1;
   comp->acSunk=0;
   comp->bsSunk=0;
   comp->pSunk=0;
   buildUserBoard();
   sleep(1);
   buildCompBoard();
   printf("\e[1;1H\e[2J");
   printf("\n\n\n BEGIN!\n");
   sleep(1);
   printComp();
   printUser();

  while(1){
     printf("\e[1;1H\e[2J");
     printComp();
     printUser();
     userTurn();
     checkSunkUser();
     sleep(.5);
     if(checkUserWin()){
        break;
     }//end if
     printf("\e[1;1H\e[2J");
     printComp();
     printUser();
     compTurn();
     if(checkCompWin()){
        break;
     }//end if
   }//end while
   return 0;
}//end main

int checkUserWin(){
   if(pilotSunk && bShipSunk && aCarrSunk){
     printf("\e[1;1H\e[2J");
     printComp();
     printUser();
     sleep(1);
     printf("\e[1;1H\e[2J");
     printf("\n\n\nYOU WON!!!\n");
     return 1;
   }//end if
   return 0;

}

int checkCompWin(){
   if(comp->acSunk&&comp->bsSunk&&comp->pSunk){
     printf("\e[1;1H\e[2J");
     printComp();
     sleep(1);
     printf("\e[1;1H\e[2J");
     printf("\n\n\n COMPUTER WON! YOU SUCK!\n");
     return 1;
   }
   return 0;
}
void compTurn(){
   if(!comp->hit){
      randomTurn();
   }//end if
   else{
      coordinatedStrike();
   }
}//end compTurn

void coordinatedStrike(){
       //if we missed.
     while(1){
       if(!validMove()||
          userBoard[comp->lsRow+comp->rDirect][comp->lsCol+comp->cDirect]==0 ||
          userBoard[comp->lsRow+comp->rDirect][comp->lsCol+comp->cDirect]==-1||
          userBoard[comp->lsRow+comp->rDirect][comp->lsCol+comp->cDirect]==1){
           //if they are equal, we've only had a hit from random.
          if(comp->lsRow==comp->sRow&&comp->lsCol==comp->sCol){
             if(comp->rDirect==1){
                comp->rDirect=0;
                comp->cDirect=1;
             }//end if
             else if(comp->rDirect==-1){
                comp->rDirect=0;
                comp->cDirect=-1;
             }//end if
             else{
                comp->rDirect=-1;
                comp->cDirect=0;
             }//end if
          }//end if
          else{//we had a hit, should continue on other side.
            comp->cDirect*=-1;
            comp->rDirect*=-1;
            comp->lsRow=comp->sRow;
            comp->lsCol=comp->sCol;
          }//end else
         if(userBoard[comp->lsRow+comp->rDirect][comp->lsCol+comp->cDirect]==0){
           userBoard[comp->lsRow+comp->rDirect][comp->lsCol+comp->cDirect]=-1;
           printf("Computer missed!\n");
           sleep(1);
           break;
         }//end if 
       }//end if
      else{
         userBoard[comp->lsRow+comp->rDirect][comp->lsCol+comp->cDirect]=1;
         comp->lsRow+=comp->rDirect;
         comp->lsCol+=comp->cDirect;
         printf("Computer hit!\n");
         sleep(1);
         checkSunkComp();
         break;
      }//end else
  }//end while
}//end coordStrike
int validMove(){
   if(comp->lsRow+comp->rDirect<0 || comp->lsRow+comp->rDirect>5){
      return 0;
   }//end if
   if(comp->lsCol+comp->cDirect<0 || comp->lsCol+ comp->cDirect >5){ 
      return 0;
   }//end if
   return 1;
}//end validMove

void randomTurn(){
   int row, col;
   srand(time(NULL));
   row=rand()%6;
   col=rand()%6;
   while(userBoard[row][col]==-1 || userBoard[row][col]==1){
      row=rand()%6;
      col=rand()%6;  
   }
   if(userBoard[row][col]==0){
      printf("Computer missed!\n");
      sleep(1);
      userBoard[row][col]=-1;
   }//end if
   else{
      userBoard[row][col]=1;
      printf("Computer hit!\n");
      sleep(1);
      comp->hit=1;
      comp-> sRow=row;
      comp->lsRow=row;
      comp->sCol=col;
      comp->lsCol=col;
      comp->rDirect=1;
      comp->cDirect=0;
   }//end else
}//end randomTurn

void userTurn(){
   char* buff=NULL;
   char buffer;
   size_t size = 0;
   int row;
   int col;
   int validInput=0;
   while(!validInput){
      getline(&buff, &size, stdin);
      if(sscanf(buff,"%c %d", &buffer, &col)==2){
          switch(toupper(buffer)){
		
		case 'A' : 
			row=0;
			break;
                case 'B' : 
			row=1;
			break;
                case 'C' :
			row =2;
			break;
		case 'D' :
			row = 3;
			break;
		case 'E' :	
			row = 4;
			break;
		case 'F': 
			row =5;
			break;
		default:	
			printf("Please enter valid input...\n");

	} //end switch
         if(row>=0 && row <=5 && col >=0 && col<=5){
            if(compBoard[row][col]!= 1 && compBoard[row][col] != -1){
               if(compBoard[row][col]==0){
                  compBoard[row][col]=-1;
               }//end if
               else{
                  compBoard[row][col]=1;
               }//end else
               validInput=1;
               break;
            }//end if
         }//end if
      }//end while
     printf("Please enter valid input\n");
  }//end while
  
}//end userTurn

void checkSunkUser(){
   if(!pilotSunk){
      if(checkSunkShip(2)){
         pilotSunk=1;
         printf("\n \n \nSUNK PILOT SHIP!\n");
         sleep(2);
      }//end if
   }//end if
   if(!bShipSunk){
      if(checkSunkShip(3)){
         bShipSunk=1;
         printf("\n\n\n SUNK BATTLE SHIP!\n");
         sleep(2);
      }//end if
   }//end if
   if(!aCarrSunk){
      if(checkSunkShip(4)){
         aCarrSunk=1;
         printf("\n\n\n SUNK AIRCRAFT CARRIER!\n");
         sleep(2);
      }//end if
   }//end if
}//end checkSunk

int checkSunkShip(int size){
   int i, j;
   for(i=0; i<6; i++){
      for(j=0; j<6; j++){
         if(compBoard[i][j]==size){
            return 0;
         }//end if
      }//end for
   }//end for
   return 1;
}//end checkUserWon
////////////////////////////////////




void checkSunkComp(){
   if(!comp->pSunk){
      if(checkSunkShipComp(2)){
         comp->pSunk=1;
         comp->hit=0;
         printf("\n \n \nSUNK YOUR PILOT SHIP!\n");
         sleep(2);
      }//end if
   }//end if
   if(!comp->bsSunk){
      if(checkSunkShipComp(3)){
         comp->bsSunk=1;
         comp->hit=0;
         printf("\n\n\n SUNK YOUR BATTLE SHIP!\n");
         sleep(2);
      }//end if
   }//end if
   if(!comp->acSunk){
      if(checkSunkShipComp(4)){
         comp->acSunk=1;
         comp->hit=0;
         printf("\n\n\n SUNK YOUR AIRCRAFT CARRIER!\n");
         sleep(2);
      }//end if
   }//end if
}//end checkSunk

int checkSunkShipComp(int size){
   int i, j;
   for(i=0; i<6; i++){
      for(j=0; j<6; j++){
         if(userBoard[i][j]==size){
            return 0;
         }//end if
      }//end for
   }//end for
   return 1;
}//end checkUserWon
void printComp(){
   int i, j;
   printf("   CompBoard    \n");
   printf("  ----------------\n");
   printf("    0 1 2 3 4 5   \n");
   for(i=0; i<6; i++){
      for(j=0; j<6; j++){
         if(j==0){
            printf("%c| ", i+65);
         }//end if
         if(compBoard[i][j]== -1){
            printf(" o");
         }
         else if(compBoard[i][j]==1){
            printf(" x");
         }//end if
         else{
            printf(" ~");
         }//end if
      }//end j
      printf("\n"); 
   }// end i for
  printf("Ships Sunk P : %d BS: %d AC: %d\n", pilotSunk, bShipSunk, aCarrSunk);
}//end print

void printUser(){
   int i, j;
   printf("   USER BOARD    \n");
   printf("  ----------------\n");
   printf("    0 1 2 3 4 5   \n");
   for(i=0; i<6; i++){
      for(j=0; j<6; j++){
         if(j==0){
            printf("%c| ", i+65);
         }//end if
         if(userBoard[i][j]== -1){
            printf(" o");
         }
         else if(userBoard[i][j]==1){
            printf(" x");
         }//end if
         else if (userBoard[i][j]==2){
            printf(" p");
         }//end if
         else if(userBoard[i][j]==3){
            printf(" b");
         }//end if
         else if(userBoard[i][j]==4){
  	    printf(" a");
         }//end if
         else{
            printf(" ~");
         }//end else
      }//end j
      printf("\n"); 
   }// end i for
  printf("Ships Sunk P : %d BS: %d AC: %d\n", comp->pSunk, comp->bsSunk, comp->acSunk);
}//end print



//horizontal =1 
// vertical =0

void buildCompBoard(){
   int direction, row, col, size;
   srand(time(NULL));
   row=rand()%6;
   col=rand()%6;
   size=2;
   direction=rand()%2;
   while(size<5){
      while(!checkInsertComp(direction, size, row, col)){
         row=rand()%6;
         col=rand()%6;
      }//end while
      if(direction==1){
         insertHor(compBoard, size, row, col);
      }//end if
      else {
        insertVert(compBoard, size, row, col);
      }//end else
      size++;
      direction=rand()%2;
      row=rand()%6;
      col=rand()%6;
   }//end while
}//end buildCompBoard

////////////////////////////////
void buildUserBoard(){
   int direction, row, col, size;
   char* buff=NULL;
   size_t sizes = 0;
   char direc, buffer;
   size=2;
   while(size<5){
      printf("\e[1;1H\e[2J");
      printUser();
      if(size==2){
         printf("Type direc row col to insert PILOT SHIP\n");
	 printf("Format : Direction Row Col\n");
         printf("h for horizonatal, v for vertical\n");
       }//end if
      else if(size==3){
         printf("Type direc row col to insert BATTLE SHIP\n");
 	 printf("Format : Direction Row Col\n");
         printf("h for horizonatal, v for vertical\n");
      }//end if   
      else{
         printf("Type direc row col to insert AIRCRAFT CARRIER\n");
	 printf("Format : Direction Row Col\n");
         printf("h for horizonatal, v for vertical\n");
      }//end else
       getline(&buff, &sizes, stdin);
      if(!checkInsertUser(size, buff)){
         printf("\n\nInvalid placement\n");
         sleep(1);
         continue;
      }//end while
      //finally got good input
      sscanf(buff,"%c %c %d",&direc,&buffer , &col);
      switch(toupper(buffer)){
		
	case 'A' : 
		row=0;
		break;
        case 'B' : 
		row=1;
		break;
        case 'C' :
		row =2;
		break;
	case 'D' :
		row = 3;
		break;
	case 'E' :	
		row = 4;
		break;
	case 'F': 
		row =5;
		break;	
		

	} //end switch
    switch(toupper(direc)){
       case 'H' : 
		 direction = 1;
		 break;
       case 'V' : 
		 direction =0;
		 break;
       
		 
   }//end switch
      if(direction==1){
            insertHor(userBoard,size,row, col);
      }//end if
      else{
           insertVert(userBoard, size, row, col);
        }
      size++;
      printf("\e[1;1H\e[2J");
      printUser();
   }//end while
}//end buildUserBoard




int checkInsertComp(int direction, int size, int row, int col){
  //if we're inserting vertically
   if(direction==0){
      if(row > 6-size){
         return 0;
      }//end if
   }//end if
   else{
      if(col> 6-size){
         return 0;
      }//end if
   }//end else
   if(direction==0){
      return checkInsertVert(compBoard, size, row, col);
   }//end if
   else{
      return checkInsertHor(compBoard, size, row, col);
   }//end else
}//end checkInsert

/////////////////////

int checkInsertUser(int size, char* buff){
  //if we're inserting vertically
   int res, row, col, direction;
   char buffer, direc;
   res=sscanf(buff,"%c %c %d",&direc,&buffer , &col);
   if(res!=3){
      return 0;
   }//end if
   switch(toupper(buffer)){
		
		case 'A' : 
			row=0;
			break;
                case 'B' : 
			row=1;
			break;
                case 'C' :
			row =2;
			break;
		case 'D' :
			row = 3;
			break;
		case 'E' :	
			row = 4;
			break;
		case 'F': 
			row =5;
			break;
		default:	
			return 0;

	} //end switch
   switch(toupper(direc)){
       case 'H' : 
		 direction = 1;
		 break;
       case 'V' : 
		 direction =0;
		 break;
       default: 
		 return 0;
   }//end switch
   if(direction==0){
      if(row > 6-size){
         return 0;
      }//end if
   }//end if
   else{
      if(col> 6-size){
         return 0;
      }//end if
   }//end else
   if(direction==0){
       return checkInsertVert(userBoard, size, row, col);
   }//end if
   else{
      return checkInsertHor(userBoard, size, row, col);
   }//end else
   
}//end checkInsert



int checkInsertHor(int board[][6], int size, int row, int col){
   int i;
   for(i=col; i< col+size;i++){
      if(board[row][i]!=0){
         return 0;
      }//end if
   }//end for
   return 1;
}//end checkInsertHor


int checkInsertVert(int board[][6], int size, int row, int col){
   int i;
   for(i=row; i< row+size;i++){
      if(board[i][col]!=0){
         return 0;
      }//end if
   }//end for
   return 1;
}//end checkInsertVert



void insertHor(int board[][6], int size, int row, int col){
   int i;
   for(i=col; i< col+size;i++){
      board[row][i]=size;
   }//end for
}//end checkInsertHor


void insertVert(int board[][6], int size, int row, int col){
   int i;
   for(i=row; i< row+size;i++){
      board[i][col]=size;
   }//end for
}//end insertVert

