//// This is the CPP file you will edit and turn in.
//// Name: Dylan Nguyen. This program plays game of life on console. Prints welcome statement and rules, and then creates
//// a grid based on a file that the user chooses. Players can choose between three commands: "animate" the gameboard,"tick"
//// the gameboard, and quit the game altogether.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"

using namespace std;
string getCommandInput();
void introStatement();
Grid<char> createGrid();
void displayGrid(Grid<char>& grid);
void checkForFile(ifstream& input, string file);
bool updateConsole(Grid<char>& grid, int time);
int checkNeighbors(Grid<char>& grid, int row, int column);
string calculateNextFrame(int neighborCount, Grid<char>actualGrid, Grid<char>& temporaryGrid);
int calculateFrameInput(string command);

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    introStatement();
    Grid<char> gameBoard = createGrid();
    displayGrid(gameBoard);
    while (true) {
        string command = getCommandInput();
        int frameCount = calculateFrameInput(command);
        bool continueGame = updateConsole(gameBoard, frameCount);
        if (continueGame == false) {
            break;
        }
    }
}

/*Prints welcome statement and explanation of game to console.*/
void introStatement() {
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << "" << endl;
}

/*creates Grid by asking for user to enter a filename*/
Grid<char> createGrid() {
    ifstream input;
    string fileName;
    string line;
    checkForFile(input, fileName);
    getline(input, line);
    int rowNumber = stringToInteger(line);
    getline(input, line);
    int columnNumber = stringToInteger(line);
    Grid<char> gameBoard;
    gameBoard.resize(rowNumber, columnNumber);
    for (int i = 0; i < gameBoard.numRows(); i++) {
        for(int j = 0; j < gameBoard.numCols(); j++) {
            input >> gameBoard[i][j];
        }
    }
    return gameBoard;
}

/*displays grid to console*/
void displayGrid(Grid<char>& grid) {
    for (int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

/*Gets command from user (a for animate, t for tick, q for quit). Returns command as a string.*/
string getCommandInput() {
    string command;
    while (true) {
        command = getLine("a)nimate, t)ick, q)uit? ");
        //cout << "a)nimate, t)ick, q)uit? ";
        //cin >> command;
        if (command == "t" || command =="q" || command == "a") {
            return command;
            break;
        }
        else {
            cout << "Invalid choice; please try again." << endl;
        }
    }
}

/*Calculates how many generations will be displayed (calculates how many frames will be shown). If the user
 * previously chose to animate the gameboard, then this method asks the user how many frames to animate.*/
int calculateFrameInput(string command) {
    int frameCount;
    if (command == "a") {
        frameCount = getInteger("How many frames?");
        //cout << "How many frames?";
    }
    if (command == "t") {
        frameCount = 1;
    }
    if (command == "q") {
        frameCount = 0;
    }
    return frameCount;
}

/* updates and then displays grid contents. Uses frameCount paramter to determine how many times the grid is updated and displayed.
 * Returns a boolean whether or not the program should continue. If the user previously prompted q (quit), then the program returns
 * false. Otherwise, the program returns true.*/
bool updateConsole(Grid<char>& grid, int frameCount) {
    if (frameCount == 0) {
        cout << "Have a nice life!" << endl;
        return false;
    }
    Grid<char> tempGrid;
    tempGrid.resize(grid.numRows(), grid.numCols());
    for (int z = 0; z < frameCount; z++) {
        for (int i = 0; i < grid.numRows(); i++) {
            for (int j = 0; j < grid.numCols(); j++) {
                int neighbors = checkNeighbors(grid, i, j);
                switch (neighbors) {
                case 0: case 1: case 4:
                    tempGrid[i][j] = '-';
                    break;
                case 2:
                    tempGrid[i][j] = grid[i][j];
                    break;
                case 3:
                    tempGrid[i][j] = 'X';
                    break;
                default:
                    tempGrid[i][j] = '-';
                    break;
                }
            }
        }
        grid = tempGrid;
        if (frameCount > 1) {
            pause(50);
            clearConsole();
        }
        displayGrid(grid);
    }
    return true;
}

/*Counts neighbors of a cell. Returns int neighborcount.*/
int checkNeighbors(Grid<char>& grid, int row, int column) {
    int neighborCount = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            if (grid.inBounds(row+i, column+j)) {
                if (grid[row+i][column+j] == 'X') {// && grid[row+i][column+j] != grid[row][column]) {
                    neighborCount++;
                }
            }
        }
    }
    return neighborCount;
}

/*Checks if the string the user enters is a valid file. If it's not, then the user is prompted to try again.*/
void checkForFile(ifstream& input, string file) {
    while (true) {
        file = getLine("Grid input file name? ");
        openFile(input, file);
        if (!fileExists(file)) {
            cout << "File not found; please try again." << endl;
            continue;
        }
        break;
    }
}
