/*
SUDOKU SOLVER
Using Graph Coloring
///////////////////
Code by:
GiVe (jee-vee)
May 25, 2016
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define N 9
#define sqrtN 3

struct node{
	int row, col;
	char color[10];
	struct node *templink;
	struct node *rowmates[8];
	struct node *colmates[8];
	struct node *boxmates[4];//overlapping allowed
};

char color_array[10][7] = {"Black","Brown","Red","Orange","Yellow","Green","Blue","Violet","Grey","White"};

int board[N][N]; //Sudoku Board

struct node * FindUnassignedVertex(struct node **h);

int isSafe(struct node *t, int num);

int SolveSudoku(struct node **h);

int UsedInRow(struct node *t, int n);

int UsedInCol(struct node *t, int n);

int UsedInBox(struct node *t, int n);

void insert_templink(struct node *e,struct node **h);

void print_templink(struct node **h);

void link_rowmates(struct node **h);

struct node * search_rowcol(struct node **h, int i, int j);

void link_everything(struct node **h);

void print_rowmate(struct node *h);

void print_colmate(struct node *h);

void print_boxmate(struct node *h);

int convertcolor(struct node **h);

void printBoard(int board[N][N]);

int main (){
	//////////////////////Initialization Section///////////////
	int i=0,n=0,j=0,k=0;
	char cchar;
	char tempString[N]; //accepts only 4 digits max
	
	//////////////PARSING Section//////////////////
	for(i=0;i<N;i++){//sets all elements of board to zero
		for(j=0;j<N;j++){ 
			board[i][j] = 0;
		}
	}
	FILE *strm = NULL;
	strm = fopen("board.csv","r");
	if(strm==NULL){
		printf("File not found.\n");
		usleep(500000);
		printf("Program Terminated.");
		return 0;
	}
	else{
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				n=0;
				do{
					cchar = fgetc(strm);
					tempString[n] = cchar;
					n++;
					if(cchar == '\n' || cchar == EOF){//prevents uhmmm yeah xD
						break;
					}
				}while(cchar!=',');
				tempString[n-1] = '\0';
				sscanf(tempString,"%d",&board[i][j]);
			}
		}
		fclose(strm);
		///////////////////Initializing Data Structure///////////////////////
		struct node **head = (struct node **) malloc(sizeof(struct node *));
		*head = NULL;
		struct node *temp = (struct node *) malloc(sizeof(struct node));
		///////////////Coloring Vertices and Linking Pointers////////////////
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				//assign row and col
				//assign color
				//insert_templink
				temp = (struct node *) malloc(sizeof(struct node));
				temp->row = i;
				temp->col = j;
				strcpy(temp->color,color_array[board[i][j]]);
				temp->templink = NULL;
				for(k=0;k<8;k++){
					temp->rowmates[k] = NULL;
					temp->colmates[k] = NULL;
				}
				for(k=0;k<4;k++){
					temp->boxmates[k] = NULL;
				}
				insert_templink(temp,head);
			}
		}
		///////////////////Linking Rowmates, Colmates and Boxmates/////////////////////
		link_everything(head);
		///////////////////Solving the Sudoku using Backtracking
		printf("\nSolve Sudoku Puzzle:\n");
		printBoard(board);
		if(SolveSudoku(head) == 2){
			printf("\nSudoku Puzzle Solved:\n");
			board[N][N] = convertcolor(head);
			printBoard(board);
		}
		else{
			printf("No Solution.\n");
		}
		///////////////////
	}
	return 0;
}

void printGrid(int board[N][N]){
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			printf("%d ",board[i][j]);
		}
		printf("\n");
	}
}

void insert_templink(struct node *e,struct node **h){
	if(*h == NULL){//if empty
		*h = e;
	}
	else{
		struct node *p = *h;
		while(p->templink != NULL){
			p = p->templink;
		}
		e->templink = p->templink;
		p->templink = e;
	}
}

void print_templink(struct node **h){
	struct node *p = *h;
	if(p == NULL){
		printf("No elements in list.\n");
	}
	else{
		printf("\tLIST\n");
		while(p != NULL){
			printf("RC: %d,%d %s, Next: %p\n",p->row, p->col, p->color, p->templink);
			p = p->templink;
		}
	}
}

struct node * search_rowcol(struct node **h, int i, int j){
	struct node *p = *h;
	while(p != NULL){
		if(p->row == i && p->col == j){
			return p;
		}
		p = p->templink;
	}
	return p;
}

void link_everything(struct node **h){
	int i,j,k,boxStartRow,boxStartCol;
	struct node *p = *h;
	struct node *t;
	while(p != NULL){
		//link rowmates
		//link colmates
		//link boxmates
		i = p->row;
		j = p->col;
		k = 0;
		for(j=0;j<N;j++){
			if(j != p->col){
				t = search_rowcol(h,i,j);
				p->rowmates[k] = t;
				k++;
			}
		}
		i = p->row;
		j = p->col;
		k = 0;
		for(i=0;i<N;i++){
			if(i != p->row){
				t = search_rowcol(h,i,j);
				p->colmates[k] = t;
				k++;
			}
		}
		boxStartRow = p->row - (p->row)%3;
		boxStartCol = p->col - (p->col)%3;
		k = 0;
		for(i=0;i<sqrtN;i++){
			for(j=0;j<sqrtN;j++){
				if(i+boxStartRow != p->row && j+boxStartCol != p->col){
					t = search_rowcol(h,i+boxStartRow,j+boxStartCol);
					p->boxmates[k] = t;
					k++;
				}
			}
		}
		p = p->templink;
	}
}

void print_rowmate(struct node *h){
	int i;
	struct node *p = h;
	if(p == NULL){
		printf("No elements in list.\n");
	}
	else{
		printf("\tRowmates\n");
		for(i=0;i<8;i++){
			printf("RC: %d,%d %s\n",p->rowmates[i]->row, p->rowmates[i]->col, p->rowmates[i]->color);
		}
	}
}

void printBoard(int board[N][N]){
	int i,j;
	printf("\n--------------------------\n");
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(j%3 == 0){
				printf(" | ");
			}
			else{
				printf(" ");
			}
			printf("%d",board[i][j]);
		}
		printf(" |");
		if((i+1)%3 == 0){
			printf("\n--------------------------\n");
		}
		else{
			printf("\n");
		}
	}
}

void print_colmate(struct node *h){
	int i;
	struct node *p = h;
	if(p == NULL){
		printf("No elements in list.\n");
	}
	else{
		printf("\tColmates\n");
		for(i=0;i<8;i++){
			printf("RC: %d,%d %s\n",p->colmates[i]->row, p->colmates[i]->col, p->colmates[i]->color);
		}
	}
}

void print_boxmate(struct node *h){
	int i;
	struct node *p = h;
	if(p == NULL){
		printf("No elements in list.\n");
	}
	else{
		printf("\tBoxmates\n");
		for(i=0;i<4;i++){
			printf("RC: %d,%d %s\n",p->boxmates[i]->row, p->boxmates[i]->col, p->boxmates[i]->color);
		}
	}
}

struct node * FindUnassignedVertex(struct node **h){
	struct node *p = *h;
	while(p != NULL){
		if(strcmp(p->color,color_array[0]) == 0){
			return p;
		}
		p = p->templink;
	}
	return p;
}

int isSafe(struct node *t, int num){
	int a,b,c;
	a = UsedInRow(t,num);
	b = UsedInCol(t,num);
	c = UsedInBox(t,num);
	if(a==1 && b==1 && c==1){//if unused, it is safe
		return 2;
	}
	else{//if one ore more is used, it is unsafe
		return 1;
	}
}

int UsedInRow(struct node *t, int n){
	int i;
	for(i=0;i<8;i++){
		if(strcmp(t->rowmates[i]->color,color_array[n]) == 0){
			return 2;
		}
	}
	return 1;
}

int UsedInCol(struct node *t, int n){
	int i;
	for(i=0;i<8;i++){
		if(strcmp(t->colmates[i]->color,color_array[n]) == 0){
			return 2;
		}
	}
	return 1;
}

int UsedInBox(struct node *t, int n){
	int i;
	for(i=0;i<4;i++){
		if(strcmp(t->boxmates[i]->color,color_array[n]) == 0){
			return 2;
		}
	}
	return 1;
}


int convertcolor(struct node **h){//converts the board coloring into numbers
	int i;
	struct node *p = *h;
	while(p != NULL){
		for(i=0;i<10;i++){
			if(strcmp(p->color,color_array[i]) == 0){
				board[p->row][p->col] = i;
				break;
			}
		}
		p = p->templink;
	}
	return board[N][N];
}

int SolveSudoku(struct node **h){
	//find unassigned
	//if there is, perform for loop
	//if none, return 2(done)
	int n;
	struct node *t = FindUnassignedVertex(h);
	if(t == NULL){
		return 2;
	}
	for(n=1;n<=9;n++){
		if(isSafe(t,n) == 2){
			strcpy(t->color,color_array[n]);
			if(SolveSudoku(h) == 2){
				return 2;
			}
			strcpy(t->color,color_array[0]);
		}
	}
	return 1;//backtrack
}