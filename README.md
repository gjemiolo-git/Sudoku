# Sudoku Game
Sudoku Game is a C++-based program that lets you play Sudoku Game based on the console input,
and selectively outputs both console and *.txt outputs.
Compiled using command prompt from:
Microsoft (R) Program Maintenance Utility Version 14.28.29336.0

## Installation
To instal application:
1. Unzip Sudoku_Game.zip
2. Open compile-supported command prompt
3. Compile using either:
nmake all
nmake sudoku
cl Sudoku_Game.cpp

## Usage
Program uses console input for determination of values placed in the cells
There is also:
-1 - Game History
-2 - Undo
-3 - Redo
-4 - Game Hints - Color and Number Ones
-5 - Game Replay - Automatically Replays the game 
-6 - Back to start menu

In addition to previously mentioned nmake commands, makefile also supports:
nmake clean 	- Removes *.obj and *.asm and *.exe files prsent in the current directory
nmake lib 	- Cretes Sudoku_Game.obj and then makes Sudoku Game.lib


## Sources
Sources used during development:
1.	Program for Sudoku Generator - GeeksforGeeks [Internet]. GeeksforGeeks. 2022 [cited 26th February 2022]. Available from: https://www.geeksforgeeks.org/program-sudoku-generator/

2.	Array class in C++ - GeeksforGeeks [Internet]. GeeksforGeeks. 2022 
[cited 1st  March 2022]. Available from: https://www.geeksforgeeks.org/array-class-c/

3.	Flamig B. Practical data structures in C++. John Wiley & Sons, Inc.; 1993 May 21.

4.	Kothari A. While programming a Sudoku generator, what is the best logic or algorithm to verify that the Sudoku puzzle created is a true Sudoku? [Internet]. Quora. 2017. Available from: https://www.quora.com/While-programming-a-Sudoku-generator-what-is-the-best-logic-or-algorithm-to-verify-that-the-Sudoku-puzzle-created-is-a-true-Sudoku

5. Zambon G. Sudoku Programming with C. Apress; 2015.



## Version
Currently the final version.
