# Sheep-Solver
A C++ recursive algorithm for solving 4 x 4 Scramble Squares Puzzles

This was built using VS Express 2015, Visual C++ Console Application, requires VC redistributal to run EXE.

For this particular puzzle, the pieces are composed of 4 sides, each side with a different half and color of a sheep.
The pieces are defined as follows, in the pieces[16][4][2] array:

Pieces are numbered from 0 to 15 for identification

Each piece has sides numbered from 0 to 3 as follows:
1 - left side
2 - top side
3 - right side
4 - bottom side

Each side has a color, defined as follows:
1 = White
2 = Black
3 = Grey
4 = Yellow

Each side also has a half of a sheep, defined as follows:
1 = Head
2 = Rear

Pieces are defined in 16 groups of 8 numbers. Beginning with the left side, and moving clockwise for each piece Each pair of numbers represents part of a sheep eg. 1,2 is White Rear, while 3,1 is Grey Head.

Defining your puzzle and running the resulting console program with administrator access will result in a txt file containing all possible solutions for the puzzle.
