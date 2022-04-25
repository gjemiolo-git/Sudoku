#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////

#define N 9 // Arr Size
#define UNASSIGNED 0
using namespace std;
///////////////////////////////////////////////////
HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
enum difficulty
{
    beginner = 10,
    easy = 20,
    medium = 35,
    hard = 53
};

////CLASSES////////////////////////////////////////
class PlayerMove {
public:
    int x, y;
    int initValue;
    int curValue;

    PlayerMove(int locx, int locy, int currValue, int intValue) {
        this->x = locx;
        this->y = locy;
        this->curValue = currValue;
        this->initValue = intValue;
    }
};
class GameBoard {
public:
////CONSTRUCTORS, MAIN GAMEBOARD FUNCTIONS////////
    GameBoard();
    ~GameBoard();

    vector<PlayerMove> vec;
    int curIndex = -1;
    bool hintsOn = false;
//////////////////////////////////////////////////
// 
//////Main GameBoard Functions////////////////////
    void generateBoard();
    bool emptyRand(int number);
    void displayBoard();
//////////////////////////////////////////////////

/////Helper GameBoard Functions///////////////////
    int forwardBackTrack();
    int solve(int i, int j, int count);

    PlayerMove isEmpty();
    void clearBoard();
    int countEmpty();
    bool isWon();
    int countFlag(int flag);
    void clearFlags();

    void undo();
    void redo();
    void replay();
 //////////////////////////////////////////////////

/////Getters///////////////////////////////////////
    bool getIsInGame();
    int getValue(int row, int col);
    int getSolutionValue(int row, int col);
    int getFlag(int row, int col);
    int getDifficulty();
    int getHintStore(int index);
/////Setters///////////////////////////////////////
    void assignFlag(int row, int col, int value);
    void updateHintStore();
    void setInGame(bool value);
    void setDifficulty(int value);
    void setCellValue(int x, int y, int value);
    void setValue(int x, int y, int value);
    bool isRepeat(int x, int y, int value);
///////////////////////////////////////////////////

/////PRIVATE VARIABLES/////////////////////////////
private:
    int difficulty = beginner;
    int hintStore[9];
    int board[9][9];
    int solutionBoard[9][9];
    bool isInGame = false;

    // 1 - unmovable, 0 user movable, 2 - user wrong 3 - user right 4 - In process of inserting input
    int constFlag[9][9];
};
///////////////////////////////////////////////////

////FUNCTION DECLARATIONS//////////////////////////
void myPause();
int generateRandomNumber(int mode);
void displayMenu(GameBoard& game);
void enquireInput(GameBoard& game);
void playGame(GameBoard& game);
void startNewGame(GameBoard& game);
///////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function returining incremnting position of empty cell in a GameBoard
// If Pos.value == 1, board full as no x,y, has value 0;
PlayerMove GameBoard::isEmpty(){
    PlayerMove pos(0, 0, 1, 0);

    for (int i = 0; i < 9; i++)
    {
        for (short j = 0; j < 9; j++)
        {
            if (board[i][j] == 0)
            {
                pos.x = i;
                pos.y = j;
                pos.curValue = 0;
                return pos;
            }
        }
    }
    return pos;
}

// Function returining board value from GameBoard object
int GameBoard::getValue(int row, int col) { return board[row][col]; }

// Function returining solutionBoard value from GameBoard object
int GameBoard::getSolutionValue(int row, int col) { return solutionBoard[row][col]; }

// Function returining constFlag value from GameBoard object
int GameBoard::getFlag(int row, int col) { return constFlag[row][col]; }

// Function setting difficulty value in GameBoard object
void GameBoard::setDifficulty(int value) { this->difficulty = value; }

// Function returining difficulty value from GameBoard object
int GameBoard::getDifficulty() { return this->difficulty; }

// Function returining hintStore value from GameBoard object
int GameBoard::getHintStore(int index) { return hintStore[index]; }

// Function returining isInGame value from GameBoard object
bool GameBoard::getIsInGame() { return isInGame; }

// Function setting isInGame value in GameBoard object
void GameBoard::setInGame(bool value) { isInGame = value; }

// Function updating hintStore value in GameBoard object
void GameBoard::updateHintStore()
{
    for (int i = 0; i < 9; i++)
    {
        hintStore[i] = 0;
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (solutionBoard[i][j] != board[i][j])
            {
                hintStore[(solutionBoard[i][j] - 1)]++;
            }
        }
    }
}

// GameBoard Constructor
GameBoard::GameBoard()
{
    for (int i = 0; i < 9; ++i)
    {
        hintStore[i] = 0;
        for (int j = 0; j < 9; ++j)
        {
            board[i][j] = 0;
            solutionBoard[i][j] = 0;
            constFlag[i][j] = 0;
        }
    }
    difficulty = beginner;
    cout << "Board is set\n";
};

// GameBoard Destructor
GameBoard::~GameBoard()
{
};

// Function clearing console
void Clear() { system("cls"); }
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////FUNCTION DEFINITIONS////////////////////////////////////////////////////////////
// Function returining game status from GameBoard object
bool GameBoard::isWon()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == solutionBoard[i][j])
            {
                if (i == 8 && j == 8) return true;
            }
            else return false;
        }
    }
    return false;
}

// Function setting individual board[x][y] value in GameBoard object
// It is distinct from setValue as it assigns the flags and does not move the solutionBoard
void GameBoard::setValue(int x, int y, int value) {

    board[x][y] = value;
    if(board[x][y] == 0) assignFlag(x, y, 0);
    else if (board[x][y] != solutionBoard[x][y]) assignFlag(x,y,2);
    else if (board[x][y] == solutionBoard[x][y]) assignFlag(x, y, 3);

}

// Function setting individual board[x][y] and solutionBoard values in GameBoard object
// It is distinct from setValue as it doesnt set any flags and keeps solutionBoard values
void GameBoard::setCellValue(int x, int y, int value) {
    board[x][y] = value;
    solutionBoard[x][y] = value;
}

// Function counting number of flags, based on value
// 0 user movable, 1 - unmovable,  2 - user wrong 3 - user right 4 - In process of inserting input
int GameBoard::countFlag(int flag)
{
    int counter = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (getFlag(i, j) == flag) counter++;
        }
    }
    return counter;
}

// Function displaying GameBoard and hints
void GameBoard::displayBoard()
{
    cout << "Sudoku\n\n";

    cout << "[ ";
    SetConsoleTextAttribute(consoleHandle, 14);
    cout << "-1";
    SetConsoleTextAttribute(consoleHandle, 7);
    cout << " - Move History ] [ ";
    cout << "[ ";
    SetConsoleTextAttribute(consoleHandle, 14);
    cout << "-2";
    SetConsoleTextAttribute(consoleHandle, 7); 
    cout << " - Undo ][ ";
    cout << "[ ";
    SetConsoleTextAttribute(consoleHandle, 14);
    cout << "-3";
    SetConsoleTextAttribute(consoleHandle, 7);
    cout << "- Redo]\n[ ";
    SetConsoleTextAttribute(consoleHandle, 14);
    cout << "-4 ";
    SetConsoleTextAttribute(consoleHandle, 7);
    if(!hintsOn) cout << "- Enable Hints]";
    else cout << "- Disable Hints]";
    cout << "[ ";
    SetConsoleTextAttribute(consoleHandle, 14);
    cout << "-5 ";
    SetConsoleTextAttribute(consoleHandle, 7);
    cout << "- Replay Game] ";
    cout << "[ ";
    SetConsoleTextAttribute(consoleHandle, 14);
    cout << "-6 ";
    SetConsoleTextAttribute(consoleHandle, 7);
    cout << "- Finish Game] \n";
   

    cout << "====================================================\n";
    for (int i = 0; i < 9; ++i)
    {
        int tempBoardValue = i;
        if (i == 0)
        {
            cout << " ";
            for (int k = 0; k < 9; k++)
            {
                if (k % 3 == 0) cout << "  ";
                cout << "| ";
                cout << k;
                cout << " |";
            }
            cout << "\n====================================================\n";
        }
        if ((i % 3 == 0)) cout << "====================================================\n";

        cout << tempBoardValue;
        for (int j = 0; j < 9; ++j)
        {
            if (j % 3 == 0) cout << "  ";
            cout << "| ";

            
            if(constFlag[i][j] == 0 || ((constFlag[i][j] == 2 && !hintsOn) || (constFlag[i][j] == 3 && !hintsOn))) SetConsoleTextAttribute(consoleHandle, 14);
            if(constFlag[i][j] == 1) SetConsoleTextAttribute(consoleHandle, 6); 
            if (constFlag[i][j] == 2 && hintsOn) SetConsoleTextAttribute(consoleHandle, 4);
            if (constFlag[i][j] == 3 && hintsOn) SetConsoleTextAttribute(consoleHandle, 2);
            if (constFlag[i][j] == 4) SetConsoleTextAttribute(consoleHandle, 11);



            cout << board[i][j];
            SetConsoleTextAttribute(consoleHandle, 7);
            cout << " |";
        }
        cout << "\n====================================================\n";
    }
    if (hintsOn)
    {
        cout << "Current progress: [ " << (N * N - countEmpty() - countFlag(2)) << " / " << (N * N) << " ]" << endl;
        updateHintStore();
        cout << "Missing numbers: ";
        for (int i = 0; i < 9; i++)
        {
            cout << "[";
            SetConsoleTextAttribute(consoleHandle, 6);
            cout << (i + 1);
            SetConsoleTextAttribute(consoleHandle, 7);
            cout << " - ";
            SetConsoleTextAttribute(consoleHandle, 14);
            cout << getHintStore(i);
            SetConsoleTextAttribute(consoleHandle, 7);
            cout << "] ";
        }
        cout << endl;
    }
    
};

// Function determining whether given value can be placed on the selected cell
// Based on the rules of sudoku
bool GameBoard::isRepeat(int x, int y, int value) {
    for (int i = 0; i < 9; i++)
    {
        // Vertical Repeat Check
        if (board[x][i] == value) {
            return true;
        }

        // Horizontal Repeat Check
        if (board[i][y] == value) {
            return true;
        }
    }

    int startRow = (x) / 3 * 3;
    int startCol = (y) / 3 * 3;

    // 3x3 BoxCheck
    for (int i = startRow; i < startRow + 3; i++)
        for (int j = startCol; j < startCol + 3; j++) {
            if (board[(i)][(j)] == value) {
                return true;
            }
        }

    return false;
}

// Function used to solve the sudoku using incremental backtracking
int GameBoard::forwardBackTrack() {
    int row, col;
    int counter = 0;
    PlayerMove pos = isEmpty();
    row = pos.x;
    col = pos.y;

    if (pos.curValue == 1)
        return 1;

    for (int num = 1; num <= 9; num++)
    {
        if (!isRepeat(row, col, num))
        {
            setCellValue(row, col, num);
            if (forwardBackTrack())
            {
                counter++;
                return counter;
            }
            board[row][col] = 0;
        }
    }
    return counter;
}

// Function used to count possible solutions to the sudoku based on the recursive backtracking
int GameBoard::solve(int i, int j, int count) {
    if (i == 9) {
        i = 0;
        if (++j == 9)
            return 1 + count;
    }
    if (board[i][j] != 0)  // skip filled cells
        return solve(i + 1, j, count);

    for (int val = 1; val <= 9 && count < 2; ++val) {
        if (!isRepeat(i, j, val)) {
            board[i][j] = val;
            count = solve(i + 1, j, count);
            //board[i][j] = 0; // reset on backtrack
        }
    }
    board[i][j] = 0;
    return count;
}

// Helper function, for clearing the Board, before setting it
void GameBoard::clearBoard()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            board[i][j] = 0;
        }
    }
}
int GameBoard::countEmpty()
{
    int counter = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == 0) counter++;
        }
    }
    return counter;
}

// Function used for the generation of Board
// The Board generation first randomly generates 3 x 3 * 3 boxes in a diagonal fashion, then randomly assigns the rest of the values
// Following Sudoku Rules
void GameBoard::generateBoard()
{
    curIndex = -1;
    vec.clear();
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            constFlag[i][j] = 0;

            if (i < 3 && j < 3)
            {
                if (board[i][j] == 0)
                {
                    int tVal = generateRandomNumber(1);
                    while (isRepeat(i, j, tVal))
                    {
                        tVal = generateRandomNumber(1);
                    }
                    setCellValue(i, j, tVal);
                }
            }
            else if (i >= 3 && i < 6 && j >= 3 && j < 6)
            {
                if (board[i][j] == 0)
                {
                    int tVal = generateRandomNumber(1);
                    while (isRepeat(i, j, tVal))
                    {
                        tVal = generateRandomNumber(1);
                    }
                    setCellValue(i, j, tVal);
                }
            }
            else if (i >= 6 && i < 9 && j >= 6 && j < 9)
            {
                if (board[i][j] == 0)
                {
                    int tVal = generateRandomNumber(1);
                    while (isRepeat(i, j, tVal))
                    {
                        tVal = generateRandomNumber(1);
                    }
                    setCellValue(i, j, tVal);
                }
            }
        }
    }

    if (!forwardBackTrack()) {
        clearBoard();
        generateBoard();
    }
}

// Function used to assign flags used for the board display and hints
void GameBoard::assignFlag(int row, int col, int value) { constFlag[row][col] = value; }

// Function used to clear flags to 1 - Unmovable status
void GameBoard::clearFlags()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] != 0) constFlag[i][j] = 1;
        }
    }
}

// Function used in random removal of the sudoku cells
// It first looks for random cell, checks if removal is going to leave the GameBoard with 1 viable sollution and if so proceeds
// Otherwise looks for another cell
bool GameBoard::emptyRand(int number)
{
int stuckCounter = -1;
int curEmpty = countEmpty();
if ((curEmpty - number) <= (N * N))
{
    while ((curEmpty + number > countEmpty()))
    {
        int tmpRow = generateRandomNumber(0);
        int tmpCol = generateRandomNumber(0);
        if (board[tmpRow][tmpCol] != 0)
        {
            int tmpBoardValue = board[tmpRow][tmpCol];
            board[tmpRow][tmpCol] = 0;
            while (solve(0, 0, 0) != 1)
            {
                board[tmpRow][tmpCol] = tmpBoardValue;
                tmpRow = generateRandomNumber(0);
                tmpCol = generateRandomNumber(0);
                tmpBoardValue = board[tmpRow][tmpCol];

                if (board[tmpRow][tmpCol] != 0)
                {
                    board[tmpRow][tmpCol] = 0;
                }
                else
                {
                    stuckCounter++;
                    if (stuckCounter > 81)
                    {
                        this->clearBoard();
                        this->generateBoard();
                        this->emptyRand(number);
                        return false;
                    }

                }
            }
        }
    }
}
clearFlags();
return true;
}

// Helper function for Game Flow Control
void playGame(GameBoard& game)
{
    game.displayBoard();
    enquireInput(game);
}

// Function used to facilitate undo functionality
void GameBoard::undo()
{
    if (curIndex > -1)
    {
        setValue(vec[curIndex].x, vec[curIndex].y, vec[curIndex].initValue);
        curIndex--;
    }
}
// Function used to facilitate redo functionality
void GameBoard::redo()
{
    if ((int)vec.size() > 0 && curIndex < (int)(vec.size() - 1))
    {
    curIndex++;
    setValue(vec[curIndex].x, vec[curIndex].y, vec[curIndex].curValue);
    }
}
// Function used to facilitate replay functionality
void GameBoard::replay()
{
    using namespace std::this_thread; 
    using namespace std::chrono;

    if ((int)vec.size() <= 0)
    {
        cout << "There is no game history available";
        myPause();
        if(!isInGame)displayMenu(*this);
    }
    while (curIndex != -1)
    {
        undo();
    }
    Clear();
    displayBoard();
    sleep_for(seconds(1));
    while (curIndex != (int)(vec.size() - 1))
    {
        redo();
        Clear();
        displayBoard();
        sleep_for(seconds(1));
    }
    if (isWon() || !isInGame)
    {
        myPause();
        displayMenu(*this);
    }
}

// Helper function used to pause console
void myPause()
{
    cout << "\n ... Press Enter to continue ... \n";
    cin.ignore().get();
}

// Function used to facilitate control of user input
void enquireInput(GameBoard &game)
{
    int rowInput, columnInput, valueInput;

    while (true)
    {
        if (game.countEmpty() == 0)
        {
            if (game.isWon())
            {
                cout << "[WIN !] Congratulations you solved the board !" << endl;
                myPause();
                displayMenu(game);
            }
        }
        do {
            cout << "Enter row number (0 - 8): \n";
            cin >> rowInput;
        } 
        while (!cin || rowInput < -10 || rowInput > 8);

        Clear();
        game.displayBoard();


        if (rowInput == -1 && (int)game.vec.size() > -1)
        {
            cout << "Current Index: " << game.curIndex << endl;
            if (game.curIndex == -1) SetConsoleTextAttribute(consoleHandle, 14);;
            cout << "Initial Board State" << endl;
            SetConsoleTextAttribute(consoleHandle, 7);
            for (int i = 0; i < (int)game.vec.size(); i++)
            {
                if (game.curIndex == i) SetConsoleTextAttribute(consoleHandle, 14);;
                cout << "[ " << game.vec[i].x << "," << game.vec[i].y << " ]" << " - " << "Initial Value: " << game.vec[i].initValue << " Set Value: " << game.vec[i].curValue   << endl;
                SetConsoleTextAttribute(consoleHandle, 7);
            }
            enquireInput(game);
        }

        if (rowInput == -2)
        {
                game.undo();
                Clear();
                game.displayBoard();
                enquireInput(game);
        }
        if (rowInput == -3 && (int)game.vec.size() > 0 && game.curIndex < (int)(game.vec.size() - 1) )
        {
                game.redo();
                Clear();
                game.displayBoard();
                enquireInput(game);
        }
        if (rowInput == -4)
        {
            if (!game.hintsOn) game.hintsOn = true;
            else game.hintsOn = false;
            Clear();
            game.displayBoard();
            enquireInput(game);
        }
        if (rowInput == -5)
        {
            game.replay();
            enquireInput(game);
        }
        if (rowInput == -6)
        {
            game.setInGame(false);
            displayMenu(game);
        }
        if (rowInput >= 0 && rowInput <= 8)
        {
            do {
                cout << "Enter column number (0 - 8): " << endl;
                cin >> columnInput;
            }         while (!cin || columnInput < 0 || columnInput > 8);


            if (columnInput >= 0 && columnInput <= 8)
            {
                if (game.getFlag(rowInput, columnInput) != 1 || game.getValue(rowInput, columnInput) == 0)
                {
                    game.assignFlag(rowInput, columnInput, 4);
                    Clear();
                    game.displayBoard();

                    do {
                        cout << "Enter desired value (1 - 9)" << endl;
                        cin >> valueInput;
                    } while (!cin || valueInput < 1 || valueInput > 9);

                    if (valueInput >= 1 && valueInput <= 9)
                    {
                        PlayerMove pos = PlayerMove(rowInput, columnInput, valueInput, game.getValue(rowInput, columnInput));

                        if (((int)game.vec.size()) == game.curIndex + 1) game.vec.push_back(pos);
                        else
                        {
                            game.curIndex++;
                            game.vec[game.curIndex] = pos;
                            game.curIndex--;
                        }

                        game.setValue(rowInput, columnInput, valueInput);
                        game.curIndex++;

                        if (game.getValue(rowInput, columnInput) != game.getSolutionValue(rowInput, columnInput)) game.assignFlag(rowInput, columnInput, 2);
                        else if (game.getValue(rowInput, columnInput) == game.getSolutionValue(rowInput, columnInput)) game.assignFlag(rowInput, columnInput, 3);
                        Clear();
                        game.displayBoard();
                    }
                }
                else cout << "Box already filled" << endl;
            }
            enquireInput(game);
        }

        else break;
    }
    //enquireInput(game);
}

// Function used to facilitate control of user input
void displayMenu(GameBoard &game)
{
    int menuChoice;

    Clear();
    cout << "Welcome to Sudoku Game" << endl << endl;
    cout << "RULES" << endl;
    cout << "1. Each digit from 1 - 9 should be present in a row once." << endl;
    cout << "2. Each digit from 1 - 9 should be present in columnt once." << endl;
    cout << "3. Each digit should not be present in a box 3x3 more than once." << endl << endl << endl;
    SetConsoleTextAttribute(consoleHandle, 14);
    cout << "Main Menu\n\n";
    SetConsoleTextAttribute(consoleHandle, 7);
    cout << "1) Play Sudoku\n";
    cout << "2) Set Difficulty: ";
    SetConsoleTextAttribute(consoleHandle, 14);
    switch (game.getDifficulty())
    {
        case beginner: { cout << "Beginner \n"; break; }
        case easy: { cout << "Easy \n"; break; }
        case medium: { cout << "Medium \n"; break; }
        case hard:{cout << "Hard \n"; break; }
        default: cout << ""; break;
    }
    SetConsoleTextAttribute(consoleHandle, 7);
    cout << "3) Replay Last Game\n";
    cout << "9) Exit app\n";
    cout << "\n";

    cin >> menuChoice;
    if (menuChoice == 1 || menuChoice == 2 || menuChoice == 3 || menuChoice == 9)
    {
        switch (int(menuChoice))
        {
            case 1:
            {
                game.clearBoard();
                game.generateBoard();
                game.emptyRand(game.getDifficulty());
                game.setInGame(true);
                playGame(game);
            }
            case 2:
            {
                switch (game.getDifficulty())
                {
                case beginner: game.setDifficulty(easy); break;
                case easy: game.setDifficulty(medium); break;
                case medium: game.setDifficulty(hard); break;
                case hard: game.setDifficulty(beginner); break;
                }
                displayMenu(game);
            }
            case 3:
            {
                game.replay();
            }
            case 9:
            {
                exit(0);
            }
        }
    }
    else
    {
        cout << "Your input needs to be either 1,2,3, or 9" << endl;
        myPause();
        displayMenu(game);
    }
   
 }

// Function used for random number generation
// Function has 2 modes, mode 0 rng(0,8) - For board sizes
// mode 1 - (1,9) for value generation
int generateRandomNumber(int mode = 0) {
    int val = N + 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> uint_dist9(1, val);
    std::uniform_real_distribution<> uint_distBoard(0, 9);

    if (!mode) return (int)uint_distBoard(gen);
    else return (int)uint_dist9(gen);
}

// Function used fo initialisation of the sudoku object
int main(int argc, char** argv)
{
    GameBoard *sudoku = new GameBoard();
    displayMenu(*sudoku);

    delete sudoku;
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////