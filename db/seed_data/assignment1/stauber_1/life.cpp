//// This is the CPP file you will edit and turn in.
//// Also remove these comments here and add your own.
//// TODO: remove this comment header!

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

void gameInit(string& fileName);
void loadBoard(string& fileName, Grid<bool>& board);
void displayBoard(Grid<bool>& board);
bool gameStep(Grid<bool>& board);
void updateBoard(Grid<bool>& board);
int getNeighbors(Grid<bool>& board, int row, int col);

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    // TODO: Finish the program!

    //
    string fileName;
    Grid<bool> board;
    bool gameOn = true;

    // gameInit - prints a game description and prompts user for file name
    // returns user filename
    gameInit(fileName);

    // loadData - loads in data from file into the board. returns nothing
    loadBoard(fileName, board);

    clearConsole();
    displayBoard(board);

    while(gameOn){

        // gameStep - prompts user to tick forward, advance, or quit. returns game
        // status
        gameOn = gameStep(board);

    }

    cout << "Have a nice Life!" << endl;
    return 0;
}

void gameInit(string& fileName){

    cout<<"Welcome to the CS 106B Game of Life,\n"
        <<"a simulation of the lifecycle of a bacteria colony.\n"
        <<"Cells (X) live and die by the following rules:\n"
        <<"- A cell with 1 or fewer neighbors dies.\n"
        <<"- Locations with 2 neighbors remain stable.\n"
        <<"- Locations with 3 neighbors will create life.\n"
        <<"- A cell with 4 or more neighbors dies.\n\n";

    while(1){
        cout<<"Grid input file name? ";
        getline(cin,fileName);
        if(fileExists(fileName)) return;
        cout<<"File not found; please try again.\n";
    }

}

void loadBoard(string& fileName, Grid<bool>& board){

    ifstream boardFile;
    string strNumRows;
    string strNumCols;
    int numRows;
    int numCols;

    openFile(boardFile, fileName);
    getline(boardFile, strNumRows);
    getline(boardFile, strNumCols);

    numRows = stringToInteger(strNumRows);
    numCols = stringToInteger(strNumCols);
    board.resize(numRows,numCols);

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

void displayBoard(Grid<bool>& board){

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

bool gameStep(Grid<bool>& board){

    char step;
    int frames;
    GetStep:

    cout<<"a)nimate, t)ick, q)uit? ";
    cin>>step;
    cin.get();

    switch(step){

    case 't':

        updateBoard(board);
        clearConsole();
        displayBoard(board);
        break;

    case 'a':

        cout<<"How many frames? ";
        frames = getInteger();

        for(int i = 0; i < frames; i++){

            updateBoard(board);
            clearConsole();
            displayBoard(board);
            pause(50);

        }
        break;

    case 'q':
        return false;
        break;

    default:
        cout<<"Command not Recognized"<<endl;
        goto GetStep;

    }

    return true;

}

void updateBoard(Grid<bool>& board){

    Grid<bool> tempBoard = board;
    int rows = board.numRows();
    int cols = board.numCols();

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            int neighbors = getNeighbors(board, i, j);

            /*cout<<"row "<<i
                <<"col "<<j
                <<"has "<<neighbors<<endl;*/

            switch(neighbors){

            case 0:
            case 1:
            case 4:
                tempBoard[i][j]=false;
                break;

            case 2:
                break;

            case 3:
                tempBoard[i][j]=true;
                break;

            }


        }
    }

    board = tempBoard;
    return;

}

int getNeighbors(Grid<bool>& board, int row, int col){

    int checkRow;
    int checkCol;
    int neighbors = 0;

    for (int i = -1; i<2; i++){
        checkRow = row + i;
        //cout<<"row "<<checkRow<<endl;
        if(checkRow < 0 || checkRow >= board.numRows()){
            //cout<<"exit row"<<endl;
            continue;
        }

        for(int j = -1; j<2; j++){
            checkCol = col + j;
            //cout<<" col"<<checkCol<<endl;
            if(checkCol < 0 || checkCol >= board.numCols()
                    ||(checkRow == row && checkCol == col)){

                //cout<<"exit col"<<endl;
                continue;

            }else if(board[checkRow][checkCol]){

                neighbors++;

            }


        }

    }

    if(neighbors > 4){

        return 4;

    }else{

        return neighbors;

    }
}



