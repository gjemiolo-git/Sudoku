all: sudoku 

sudoku:
	cl SudokuGame.cpp
	
lib:
	cl /c SudokuGame.cpp
	lib SudokuGame.obj
	
clean:
	del *.obj
	del *.exe
	del *.asm