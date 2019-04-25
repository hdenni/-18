#include <stdio.h>
#define MAX_STACK_SIZE 100
#define TRUE 1
#define FALSE 0

typedef struct {
	short int vert, horiz;
}offsets;

typedef struct {
	short int row, col, dir;
}element;

/* global variables */
offsets move[8];
element stack[MAX_STACK_SIZE];
int **maze, **mark;
int START_ROW, START_COL;
int EXIT_ROW, EXIT_COL;

/* about stack */
int top = -1;
int capacity = 1;

void push(element item) {
	if (top >= MAX_STACK_SIZE - 1)
	{
		printf("Stack error!\n");
		exit(-1);
	}
	stack[++top] = item;
}

element pop()
{
	if (top == -1)
	{
		printf("Stack error!\n");
		exit(-1);
	}
	return stack[top--];
}

int main()
{
	int i, j;
	FILE* fp = fopen("MAZE.txt", "r");
	int row, col;
	int count = 0;

	fscanf(fp, "%d %d", &row, &col);
	fscanf(fp, "%d %d", &START_ROW, &START_COL);
	fscanf(fp, "%d %d", &EXIT_ROW, &EXIT_COL);
	
	maze = (int**)malloc(row*sizeof(int*));
	mark = (int**)malloc(row*sizeof(int*));
	for (i = 0; i < row; i++)
	{
		maze[i] = (int*)malloc(col*sizeof(int));
		mark[i] = (int*)malloc(col*sizeof(int));
	}

	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
		{
			fscanf(fp, "%d", &maze[i][j]);
			mark[i][j] = 0;
		}
	
	fclose(fp);
	
	move[0].vert = -1; move[0].horiz = 0;
	move[1].vert = -1; move[1].horiz = 1;
	move[2].vert = 0; move[2].horiz = 1;
	move[3].vert = 1; move[3].horiz = 1;
	move[4].vert = 1; move[4].horiz = 0;
	move[5].vert = 1; move[5].horiz = -1;
	move[6].vert = 0; move[6].horiz = -1;
	move[7].vert = -1; move[7].horiz = -1;

	/*move[7].vert = -1; move[7].horiz = 0;
	move[0].vert = -1; move[0].horiz = -1;
	move[1].vert = 0; move[1].horiz = -1;
	move[2].vert = 1; move[2].horiz = -1;
	move[3].vert = 1; move[3].horiz = 0;
	move[4].vert = 1; move[4].horiz = 1;
	move[5].vert = 0; move[5].horiz = 1;
	move[6].vert = -1; move[6].horiz = 1;*/

	if (path())
	{
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				if (i == START_ROW && j == START_COL) printf(" S");
				else if (i == EXIT_ROW && j == EXIT_COL) printf(" E");
				else if (maze[i][j] == 1) printf("бс");
				else if (mark[i][j] == 1) printf("бр");
				else printf("  ");
			}
			printf("\n");
		}
	}
	else printf("There is no path\n");

	return 0;

}

int path()
{
	int i;
	int row, col, nextRow, nextCol, dir;
	int found = FALSE;
	element position;

	mark[START_ROW][START_COL] = 1;
	top = 0;
	stack[0].row = START_ROW;		stack[0].col = START_COL;	stack[0].dir = 1;

	while (top > -1 && !found) {
		position = pop();
		row = position.row;	col = position.col;		dir = position.dir;
		while (dir < 8 && !found) {
			nextRow = row + move[dir].vert;
			nextCol = col + move[dir].horiz;

			if (nextRow == EXIT_ROW && nextCol == EXIT_COL)
				found = TRUE;
			else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) 
			{
				mark[nextRow][nextCol] = 1;
				position.row = row;	position.col = col;		position.dir = ++dir;
				push(position);
				row = nextRow;	col = nextCol;		dir = 0;
			}
			else ++dir;
		}
	}
	return found;
}