#define _CRT_SECURE_NO_DEPRECATE// XandOs.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int width;
const int height;
const int O = 1;
const int X = 2;
//blank, not used yet
const int B = 0;
//unusable, blacked out
const int U = -1;
const int BLOCK = 254;

void PrintGrid(int** grid);
char GetChar(int i);
int GetIntFromChar(char c);
void setElementFromInput(int*** grid, int x, int y, char value);
void setElement(int*** grid, int x, int y, int value);
int getElement(int** grid, int x, int y);
int inverse(int value);


int main()
{

	char* fileName = "grid.txt";
	printf("Enter width: ");
	scanf("%i", &width);
	printf("Enter height: ");
	scanf("%i", &height);

	FILE* file;
	file = fopen(fileName, "w");
	if (!file) {
		printf("File could not be opened, exiting.");
		return 1;
	}

	//printing empty grid to file
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			fprintf(file, "%c", 'E');
		}
		fprintf(file, "\n");
	}
	fclose(file);

	//advising user
	printf("\n\n==========\nE = Empty\nU = unusable\nX and O are self explanatory.");
	printf("\nMake changes to 'grid.txt', save it.\n=========");
	//confirming user is finished editing
	char i = 'a';
	while (i != 'c') {
		if (i != '\n') {
			printf("\nenter C to continue: ");
		}
		scanf("%c", &i);
	};

	//reading into grid
	//coords start top left, grid = [y][x]
	file = fopen(fileName, "r");

	int** grid = malloc(height * sizeof(int));
	if (grid == NULL) { return 1; }
	for (int i = 0; i < height; i++) { grid[i] = malloc(width * sizeof(int)); }

	//assigning to grid
	int input;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			input = fgetc(file);
			if (input != '\n') {
				grid[y][x] = GetIntFromChar(input);
			}
			else {
				x--;
			}
		}
	}

	PrintGrid(grid);

	bool unsolved = true;
	int iterations = 0;
	const int maxIters = width * height;
	while (unsolved) {
		//break infinite loops
		++iterations;
		if (iterations > maxIters) {
			break;
		}

		unsolved = false;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int value = grid[y][x];
				//checking cell has a value to analyze, if not loop again
				if (value < 1) {
					if (value == B) {
						unsolved = true;
					}
					continue;
				}
				int inv = inverse(value);
				for (int offX = -1; offX <= 1; offX++) {
					for (int offY = -1; offY <= 1; offY++) {
						//if cell empty make program skip it and loop again
						if (offY == 0 && offX == 0) {continue;}


						//checking directly adjacent values
						if (getElement(grid, x + offX, y + offY) == value) {
							setElement(&grid, x + (2 * offX), y + (2 * offY), inv);
							setElement(&grid, x - offX, y - offY, inv); 
						}


						//elements 1 block away
						if (getElement(grid, x + (offX * 2), y + (offY * 2)) == value) {
							setElement(&grid, x + offX, y + offY, inv);
						}
					}
				}
			}
		}
	}

	if (iterations > maxIters) {
		printf("\nRan out of iterations:\n");
	}
	else {
		printf("\nComplete:\n");
	}
	PrintGrid(grid);

	//freeing all memory
	for (int i = 0; i < height; i++) { free(grid[i]); }
	free(grid);
}

//takes a grid and prints it
void PrintGrid(int** grid) {
	printf("\n ");
	for (int i = 0; i < width; i++) { printf(" %i", i%10); }
	printf("\n");

	for (int y = 0; y < height; y++) {
		printf("%i", y%10);
		for (int x = 0; x < width; x++) {
			printf(" %c", GetChar(grid[y][x]));
		}
		printf("\n");
	}
}

//converts int to corresponding character, used for printing
char GetChar(int i) {
	switch (i) {
	case 1:
		return 'O';
		break;
	case 2:
		return 'X';
		break; 
	case 0:
		return ' ';
		break;
	case -1:
		return BLOCK;
		break;
	}
}

int GetIntFromChar(char c) {
	c = tolower(c);

	if (c == 'e') { return B; }
	else if (c == 'u') { return U; }
	else if (c == 'x') { return X; }
	else if (c == 'o') { return O; }
}

//returns the integer at coordinates
int getElement(int** grid, int x, int y) {
	if (x > width - 1 || x < 0) {
		return -1;
	}
	if (y > height - 1 || y < 0) {
		return -1;
	}

	return grid[y][x];
}

//converts user input to function friendly values
void setElementFromInput(int*** grid, int x, int y, char value) {
	if (x > width - 1 || x < 0) { return; }
	if (y > height - 1 || y < 0) { return; }
	
	int val = 0;
	if (tolower(value) == 'x') {val = X;} 
	else if (tolower(value) == 'o') {val = O;}
	else if(tolower(value) == 'b') {val = B;}
	else if(tolower(value) == 'u') {val = U;}

	(*grid)[y][x] = val;
}

//changes the value of a specific cell in the grid, checks for errors
void setElement(int*** grid, int x, int y, int value) {
	if (x > width - 1 || x < 0) { return; }
	if (y > height - 1 || y < 0) { return;}

	int current = (*grid)[y][x];
	if (current == U) { return; }
	(*grid)[y][x] = value;
}

//returns the opposite value of a square
int inverse(int value) {
	if (value == O) {
		return X;
	}
	return O;
}
