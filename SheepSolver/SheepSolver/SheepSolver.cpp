// SheepSolver.cpp : Defines the entry point for the console application.
// SheepSolver has been designed to find every possible solution to a predetermined 4x4 Scramble Squares puzzle
// It uses a recursive backtracking algorithm to place individual pieces, check validitiy of each piece, rotate, and replace as necessary
// The recursive method eleminates invalid solutions before they are completely formed, vastly reducing the amount of time needed to find
// a solution. If the solver can't proceed with a certain piece in a certain spot, it discounts the remaining possibilites with that particular
// piece in that location, and moves onto the next piece for that location.

#include "stdafx.h"
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;

//Function Declaration
int insertp(int piece_num, int x, int y);
int checkp(int x, int y);
int rotatep(int x, int y);
int removep(int piece_num, int x, int y);
int nextp();
int back_up();


//Variable Declaration
int first_piece, current_piece, piece_correct, starting_piece, new_piece;
int side;
int i, j;
int piece_color, piece_half, check_color, check_half;
int t = 1;
int l = 0;
int r = 2;
int b = 3;
int color = 0;
int half = 1;
int attempt;
int goback = 0;
int a;
int row, spot;
int draw_piece;
int no_sol = 0;
int sol_num = 0;
//Define Pieces
//Sheep pieces defined here
//Color: 1 = White, 2 = Black, 3 = Grey, 4 = Yellow
//Side: 1 = left, 2 = top, 3 = right, 4 = left
//Half: 1 = Head, 2 = Rear
//Sheep defined in order of piece number, side, color, half

int pieces[16][4][2] = {
	{1,2,4,1,2,1,3,2},
	{2,2,2,1,1,1,3,2},
	{2,2,1,2,4,1,3,1},
	{4,2,2,2,3,1,1,1},
	{2,1,4,2,1,2,3,1},
	{2,1,3,1,4,2,1,2},
	{2,2,3,1,4,1,1,2},
	{1,2,3,1,2,1,3,2},
	{2,1,4,2,1,2,3,1},
	{2,2,4,1,1,1,3,2},
	{2,2,4,2,1,1,3,1},
	{1,1,2,2,4,2,3,1},
	{3,2,1,1,2,1,4,2},
	{2,2,4,1,1,1,4,2},
	{1,2,4,1,2,1,3,2},
	{2,2,4,1,1,1,3,2}
};

int puzzle[6][6][4][2]; // Create empty puzzle grid with border of empty pieces (4x4 puzzle, with pieces around the outside, becomes 6x6 puzzle, so pieces can be compared with empty)

int piece_used[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; //Array keeping track of whether a pieces has been used or not

int piece_rotation[6][6] = { //Stores orientation information for places pieces
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0}
};

int puzzle_piece_number[6][6] = { //Keeps track of which piece is placed where on the grid
	{ 0,0,0,0,0,0 },
	{ 0,0,0,0,0,0 },
	{ 0,0,0,0,0,0 },
	{ 0,0,0,0,0,0 },
	{ 0,0,0,0,0,0 },
	{ 0,0,0,0,0,0 }
};

int orient[4][2] = { 0,0,0,0,0,0,0,0 }; //Temporary orientation storage for rotation

int goback_tried[16][16] = { //Keeps track of which pieces have been tried in the spot before the current position. Used to decided when to goback another level
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
};

int position = 0;





int main()
{	
	//Starting point for solution
	cout << "First piece of solution?(0-15)";
	cin >> first_piece;

	system("cls");

	ofstream solutions;
	solutions.open("solutions.txt", ios::trunc);
	solutions << "SheepSolver v0.1 -- Solutions" << endl;
	
	//Set up Puzzle -- Fill puzzle with 0's, and any combination of color and half can match a zero
	for (int a = 0; a < 6; a++)
	{
		for (int b = 0; b < 6; b++)
		{
			for (int c = 0; c < 4; c++)
			{
				puzzle[a][b][c][color] = 0;
				puzzle[a][b][c][half] = 0;
			}
		}
	}
	// Define which piece we are on for algorithm
	current_piece = first_piece;
	//Main Solution Loop:
	
	for (j = 1; j < 5; j++)
	{
		for (i = 1; i < 5; i++)
		{
			position = 4 * (j - 1) + i - 1;
			starting_piece = current_piece;
			new_piece = 0;
			do
			{
				while (new_piece != 1)
				{
					if (no_sol == 1)
					{
						cout << "NO MORE SOLUTIONS FOUND" << endl;
						solutions << "NO MORE SOLUTIONS FOUND" << endl;
						solutions.close();
						system("pause");
						return 0;
					}
					if (goback == 1)
					{
						back_up();
					}
				if (piece_used[current_piece] == 0 && goback_tried[position][current_piece] == 0)
				{
						insertp(current_piece, i, j);
						new_piece = 1;
				}
				else
				{
					nextp();
				}
			}
				piece_correct = checkp(i, j);
				if (piece_correct == 1 && position == 15)
				{
					sol_num++;
					cout << "A solution has been found!" << endl;
					solutions << "A solution has been found!" << endl;

					cout << "==========Solution #" << sol_num << "===========" << endl;
					solutions << "==========Solution #" << sol_num << "===========" << endl;

					for (row = 1; row < 5; row++)
					{
						//Print Top
						for (spot = 1; spot < 5; spot++)
						{
							cout << "   ";
							solutions << "   ";
							//Print Color
							switch (puzzle[spot][row][t][color])
							{
							case 0:
								cout << " ";
								solutions << " ";
								break;
							case 1:
								cout << "W";
								solutions << "W";
								break;
							case 2:
								cout << "B";
								solutions << "B";
								break;
							case 3:
								cout << "G";
								solutions << "G";
								break;
							case 4:
								cout << "Y";
								solutions << "Y";
								break;
							}
							// Print Half
							switch (puzzle[spot][row][t][half])
							{
							case 0:
								cout << " ";
								solutions << " ";
								break;
							case 1:
								cout << "H";
								solutions << "H";
								break;
							case 2:
								cout << "A";
								solutions << "A";
								break;
							}
							cout << "   ";
							solutions << "   ";
						}
						cout << endl;
						solutions << endl;
						//Print Middle
						for (spot = 1; spot < 5; spot++)
						{
							cout << " ";
							solutions << " ";
							//Print Left Color
							switch (puzzle[spot][row][l][color])
							{
							case 0:
								cout << " ";
								solutions << " ";
								break;
							case 1:
								cout << "W";
								solutions << "W";
								break;
							case 2:
								cout << "B";
								solutions << "B";
								break;
							case 3:
								cout << "G";
								solutions << "G";
								break;
							case 4:
								cout << "Y";
								solutions << "Y";
								break;
							}
							//Print Left Half
							switch (puzzle[spot][row][l][half])
							{
							case 0:
								cout << " ";
								solutions << " ";
								break;
							case 1:
								cout << "H";
								solutions << "H";
								break;
							case 2:
								cout << "A";
								solutions << "A";
								break;
							}
							//Print Piece Number
							draw_piece = puzzle_piece_number[spot][row];
							if (draw_piece < 10)
							{
								cout << " " << draw_piece;
								solutions << " " << draw_piece;
							}
							else
							{
								cout << draw_piece;
								solutions << draw_piece;
							}
							//Print Right Color
							switch (puzzle[spot][row][r][color])
							{
							case 0:
								cout << " ";
								solutions << " ";
								break;
							case 1:
								cout << "W";
								solutions << "W";
								break;
							case 2:
								cout << "B";
								solutions << "B";
								break;
							case 3:
								cout << "G";
								solutions << "G";
								break;
							case 4:
								cout << "Y";
								solutions << "Y";
								break;
							}
							//Print Right Half
							switch (puzzle[spot][row][r][half])
							{
							case 0:
								cout << " ";
								solutions << " ";
								break;
							case 1:
								cout << "H";
								solutions << "H";
								break;
							case 2:
								cout << "A";
								solutions << "A";
								break;
							}
							cout << " ";
							solutions << " ";
						}
						cout << endl;
						solutions << endl;
						//Print Bottom
						for (spot = 1; spot < 5; spot++)
						{
							cout << "   ";
							solutions << "   ";
							//Print Color
							switch (puzzle[spot][row][b][color])
							{
							case 0:
								cout << " ";
								solutions << " ";
								break;
							case 1:
								cout << "W";
								solutions << "W";
								break;
							case 2:
								cout << "B";
								solutions << "B";
								break;
							case 3:
								cout << "G";
								solutions << "G";
								break;
							case 4:
								cout << "Y";
								solutions << "Y";
								break;
							}
							// Print Half
							switch (puzzle[spot][row][b][half])
							{
							case 0:
								cout << " ";
								solutions << " ";
								break;
							case 1:
								cout << "H";
								solutions << "H";
								break;
							case 2:
								cout << "A";
								solutions << "A";
								break;
							}
							cout << "   ";
							solutions << "   ";
						}
						cout << endl;
						solutions << endl;
					}

					piece_correct = 0;
				}
				if (piece_correct != 1 && piece_rotation[i][j] == 3)
				{
					removep(current_piece, i, j);

					nextp();

					new_piece = 0;

					continue;
				}
				if (piece_correct == 0 && piece_rotation[i][j] < 3)
				{
					rotatep(i, j);
				}
			} while (piece_correct != 1);
		}
	}

    return 1;
}


int insertp(int piece_num, int x, int y)
{
	for (side = 0; side < 4; side++)
	{
		puzzle[x][y][side][color] = pieces[piece_num][side][color];
		puzzle[x][y][side][half] = pieces[piece_num][side][half];
	}
	piece_used[piece_num] = 1;
	puzzle_piece_number[x][y] = piece_num;
	return 0;
}

int checkp(int x, int y)
{
	//To check a piece, we need to make sure each of the 4 sides fits:
	
		for (side = 0; side < 4; side++)
		{
			piece_color = puzzle[x][y][side][color];
			piece_half = puzzle[x][y][side][half];

			switch (side) {
			case 0:
				// For the left side, we want to check i-1,j right side
				check_color = puzzle[x - 1][y][r][color];
				check_half = puzzle[x - 1][y][r][half];
				break;
			case 1:
				// For the top, we want to check i,j-1, bottom
				check_color = puzzle[x][y - 1][b][color];
				check_half = puzzle[x][y - 1][b][half];
				break;
			case 2:
				// For right side, we want to check i+1,j, left side
				check_color = puzzle[x + 1][y][l][color];
				check_half = puzzle[x + 1][y][l][half];
				break;
			case 3:
				// For bottom, we want to check i,j+1, top
				check_color = puzzle[x][y + 1][t][color];
				check_half = puzzle[x][y + 1][t][half];
				break;
			}
			if (piece_color != check_color && check_color != 0)
			{
				return 0;
			}
			if (piece_half == check_half)
			{
				return 0;
			}
		}
		return 1;
}

int rotatep(int x, int y)
{
	//Use this function to rotate a placed piece
	//Read current orientation
	for (side = 0; side < 4; side++)
	{
		orient[side][color] = puzzle[x][y][side][color];
		orient[side][half] = puzzle[x][y][side][half];
	}

	//Place orient data back into puzzle, except rotated
	puzzle[x][y][l][color] = orient[b][color];
	puzzle[x][y][l][half] = orient[b][half];
	puzzle[x][y][t][color] = orient[l][color];
	puzzle[x][y][t][half] = orient[l][half];
	puzzle[x][y][r][color] = orient[t][color];
	puzzle[x][y][r][half] = orient[t][half];
	puzzle[x][y][b][color] = orient[r][color];
	puzzle[x][y][b][half] = orient[r][half];

	//Update rotation table

	piece_rotation[x][y] ++;

	return 0;

}

int removep(int piece_num, int x, int y)
{
	for (side = 0; side < 4; side++)
	{
		puzzle[x][y][side][color] = 0;
		puzzle[x][y][side][half] = 0;
	}
	piece_rotation[x][y] = 0;
	piece_used[piece_num] = 0;
	puzzle_piece_number[x][y] = 0;
	return 0;
}

int nextp()
{
	current_piece++;
	if (current_piece > 15)
	{
		current_piece = 0;
	}
	if (current_piece == starting_piece)
	{
		goback = 1;
	}
	if (position == 0 && current_piece == first_piece)
	{
		no_sol = 1;
	}
	return 0;
}

int back_up()
{
	for (a = 0; a < 16; a++)
	{
		goback_tried[position][a] = 0;
	}
	i--;
	if (i == 0)
	{
		i = 4;
		j--;
	}

	position = 4 * (j - 1) + i - 1;

		current_piece = puzzle_piece_number[i][j];

		goback_tried[position][current_piece] = 1;

		removep(current_piece, i, j);

		starting_piece = current_piece;

		new_piece = 0;

		goback = 0;

	return 0;
}