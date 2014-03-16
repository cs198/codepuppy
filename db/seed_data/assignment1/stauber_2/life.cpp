//Author: Mark Stauber
//Date: 1/14/14
//Course: CS 106B Win' 14
//File: life.cpp
//Description: This file contains the student-generated code
//  to play the game of life including user i/o and supporting
//  functions.

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

void loadBoard(Grid<bool>& board);
void displayBoard(Grid<bool>& board);
bool gameStep(Grid<bool>& board, char step);
void updateBoard(Grid<bool>& board);
int getNeighbors(Grid<bool>& board, int row, int col);

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    // board - Contains the current game grid.
    Grid<bool> board;

    // gameOn - Tracks gameplay status.
    bool gameOn = true;

    // loadData - Loads in data from user file into board.
    loadBoard(board);

    // Display initial board.
    displayBoard(board);

    // While game is still on.
    while(gameOn){
        char step;

        cout<<"a)nimate, t)ick, q)uit? ";
        cin>>step;
        cin.get();

        // gameStep - Runs user command. returns gameplay status.
        gameOn = gameStep(board, step);
    }

    cout << "Have a nice Life!" << endl;
    return 0;
}

//Displays welcome sequence and prompts user for fileName of
//initial board info. If fileName is not a file in the
//resource directory user sees an error and is prompted for a
//new name. Loads information of initial board info from file.
//Assumes file has the following format:
//    line 1: # rows
//    line 2: # cols
//    line 3 on: initial grid setup with '-' for dead cell
//               and 'X' for live cell
void loadBoard(Grid<bool>& board){

    string fileName;
    ifstream boardFile;
    string strNumRows;
    string strNumCols;
    int numRows;
    int numCols;

    cout<<"Welcome to the CS 106B Game of Life,\n"
        <<"a simulation of the lifecycle of a bacteria colony.\n"
        <<"Cells (X) live and die by the following rules:\n"
        <<"- A cell with 1 or fewer neighbors dies.\n"
        <<"- Locations with 2 neighbors remain stable.\n"
        <<"- Locations with 3 neighbors will create life.\n"
        <<"- A cell with 4 or more neighbors dies.\n\n";

    // Prompt for fileName until fileExists.
    while(1){
        cout<<"Grid input file name? ";
        getline(cin,fileName);
        if(fileExists(fileName)) break;
        cout<<"File not found; please try again.\n";
    }

    openFile(boardFile, fileName);

    // Read in row and column values and initialize board
    getline(boardFile, strNumRows);
    getline(boardFile, strNumCols);
    numRows = stringToInteger(strNumRows);
    numCols = stringToInteger(strNumCols);
    board.resize(numRows,numCols);

    // Fill board with file data
    for(int i = 0; i < numRows; i++){
        string row;
        getline(boardFile, row);
        for(int j = 0; j < numCols; j++){
            if(row[j] == 'X'){
                board[i][j] = true;
            }else{
                board[i][j] = false;
            }
        }
    }
    return;
}

// Processes user command step.
bool gameStep(Grid<bool>& board, char step){
    switch(step){

    // Tick forward one iteration and display new board.
    case 't':
        updateBoard(board);
        displayBoard(board);
        break;

    // Tick forward frames times, pausing and updating display
    // each step. Assumes an integer input is nonnegative
    case 'a':
        int frames;
        cout<<"How many frames? ";
        frames = getInteger();
        for(int i = 0; i < frames; i++){
            updateBoard(board);
            displayBoard(board);
            pause(50);
        }
        break;

    // End the game. return false for gameplay status gameOn
    case 'q':
        return false;
        break;

    // User's command was not recognized.
    default:
        cout<<"Invalid choice; please try again."<<endl;
    }
    return true;
}

// Print out the current board to the display
void displayBoard(Grid<bool>& board){
    clearConsole();
    for(int i = 0; i < board.numRows(); i++){
        for(int j = 0; j < board.numCols(); j++){
            if(board[i][j]){
                cout<<'X';
            }else{
                cout<<'-';
            }
        }
        cout<<endl;
    }
}

// Update board
void updateBoard(Grid<bool>& board){
    // tempBoard - temporary Grid to hold new board values.
    Grid<bool> tempBoard = board;
    int rows = board.numRows();
    int cols = board.numCols();

    // Assign new values to each cell based on number of living neighbors.
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            // getNeighbors - returns living neighbors of cell i,j but
            // returns 4 if there are >=4 neighbors.
            int neighbors = getNeighbors(board, i, j);
            switch(neighbors){ 
            // For 0, 1, >=4 neighbors the cell is dead.
            case 0:
            case 1:
            case 4:
                tempBoard[i][j]=false;
                break;
            // For 2 neighbors the cell stays the same.
            case 2:
                break;
            // For 3 neighbors the cell.
            case 3:
                tempBoard[i][j]=true;
                break;
            }
        }
    }
    // Set board to the new values in tempBoard.
    board = tempBoard;
    return;
}

// Counts number of neighbors of cell row,col. returns the number of living neighbors
// but returns 4 if there are >= 4 neighbors
int getNeighbors(Grid<bool>& board, int row, int col){
    // checkRow, checkCol refer to neighbor cell under inspection.
    int checkRow;
    int checkCol;
    int neighbors = 0;

    // Check for all living neighbors
    for (int i = -1; i<2; i++){
        checkRow = row + i;

        //if checkRow is out of bounds for the grid skip
        if(checkRow < 0 || checkRow >= board.numRows()){
            continue;
        }

        for(int j = -1; j<2; j++){
            checkCol = col + j;
            // if checkCol is out of bounds for the grid or
            // if checkRow,checkCol is the center cell skip
            if(checkCol < 0 || checkCol >= board.numCols()
                    ||(checkRow == row && checkCol == col)){
                continue;
            }else if(board[checkRow][checkCol]){
                //if checkRow,checkCol neighbor is alive then increment neighbors
                neighbors++;
            }
        }
    }
    // If there are greater than 4 neighbors, return 4
    if(neighbors > 4){
        return 4;
    }else{ // Else return value of neighbors
        return neighbors;
    }
}



