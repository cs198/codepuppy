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

void printOut(){
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony."<< endl;
    cout << "Cells (X) live and die by the following rules:"<< endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
}

// the function is to print a grid in a grid structure.
// inputs are the grid which needs to be printed (pgrid)
// the row num of the grid and col num of the grid
void printGrid(Grid<char>& pGrid, int row, int col){
        for (int r = 0; r < row; r++){
            string s1 = "";
            for (int c = 0; c < col; c++){
                s1 += pGrid[r][c];
            }
            cout << s1 << endl;
        }
}


// fileInput function to read the file
// get the grid of the file
// and print the grid out
Grid<char> fileInput(){
    ifstream testFile;
    string fileName = getLine("Grid input file name?");
    while (fileExists(fileName) == false) {
        cout << "File not found; please try again" << endl;
        fileName = getLine("Grid input file name?");
    }
        openFile(testFile, fileName);
    string line;
    getline(testFile,line);
    int row = stringToInteger(line);
    getline(testFile,line);
    int col = stringToInteger(line);
    Grid<char> bacteriaGrid;
    bacteriaGrid.resize(row,col);
    for (int r = 0; r < row; r++){
        getline(testFile,line);
        for (int c = 0; c < col; c++){
            bacteriaGrid[r][c] = line[c];
        }
    }
    printGrid (bacteriaGrid, row, col);
    testFile.close();
    return bacteriaGrid;
}

// check an elment (r th row, c th col) postion in a grid
// to see whether it's at the corners or the side line or in the middle
// inputs are the row num and col num of the element in the grid
// and the size of the grid (gridRows & gridCols)
string checkPosition(int r, int c, int gridRows, int gridCols) {
    gridRows = gridRows - 1; // change the total number to the order
    gridCols = gridCols - 1; // change the total number to the order
    if (r == 0) {
        if (c == 0){
        return "lefttop";
    } else if (c == gridCols){
        return "righttop";
    } else if (c > 0 && c < gridCols){
        return "toprow";
    }
    } else if (r == gridRows) {
        if (c == 0){
            return "leftbottom";
        } else if (c == gridCols){
            return "rightbottom";
        } else if (c > 0 && c < gridCols){
            return "bottomrow";
        }
    } else if (r > 0 && r < gridRows){
        if (c == 0){
            return "leftcol";
        } else if (c == gridCols) {
            return "rightcol";
        } else if (c > 0 && c < gridCols){
            return "middle";
        }
    }
}

// to find the neighbours of a cell
// and put the neighbours into a vecotr
Vector<char> NeighbourVector (Grid<char>& bacteriaGrid, int r, int c){
    string position = checkPosition(r, c, bacteriaGrid.numRows(), bacteriaGrid.numCols());
    Vector<char> neighbourVector;
    if (position == "lefttop") {
       neighbourVector += bacteriaGrid[r][c+1];
       neighbourVector += bacteriaGrid[r+1][c];
       neighbourVector += bacteriaGrid[r+1][c+1];
       return neighbourVector;
    } else if (position == "righttop"){
        neighbourVector += bacteriaGrid[r][c-1];
        neighbourVector += bacteriaGrid[r+1][c];
        neighbourVector += bacteriaGrid[r+1][c-1];
        return neighbourVector;
    } else if (position == "leftbottom"){
        neighbourVector += bacteriaGrid[r-1][c];
        neighbourVector += bacteriaGrid[r][c+1];
        neighbourVector += bacteriaGrid[r-1][c+1];
        return neighbourVector;
    } else if (position == "rightbottom"){
        neighbourVector += bacteriaGrid[r-1][c];
        neighbourVector += bacteriaGrid[r][c-1];
        neighbourVector += bacteriaGrid[r-1][c-1];
        return neighbourVector;
    } else if (position == "toprow"){
        neighbourVector += bacteriaGrid[r][c-1];
        neighbourVector += bacteriaGrid[r][c+1];
        neighbourVector += bacteriaGrid[r+1][c-1];
        neighbourVector += bacteriaGrid[r+1][c];
        neighbourVector += bacteriaGrid[r+1][c+1];
        return neighbourVector;
    } else if (position == "bottomrow"){
        neighbourVector += bacteriaGrid[r][c-1];
        neighbourVector += bacteriaGrid[r][c+1];
        neighbourVector += bacteriaGrid[r-1][c-1];
        neighbourVector += bacteriaGrid[r-1][c];
        neighbourVector += bacteriaGrid[r-1][c+1];
        return neighbourVector;
    } else if (position == "leftcol"){
        neighbourVector += bacteriaGrid[r-1][c];
        neighbourVector += bacteriaGrid[r-1][c+1];
        neighbourVector += bacteriaGrid[r][c+1];
        neighbourVector += bacteriaGrid[r+1][c+1];
        neighbourVector += bacteriaGrid[r+1][c];
        return neighbourVector;
    } else if (position == "rightcol"){
        neighbourVector += bacteriaGrid[r-1][c];
        neighbourVector += bacteriaGrid[r-1][c-1];
        neighbourVector += bacteriaGrid[r][c-1];
        neighbourVector += bacteriaGrid[r+1][c];
        neighbourVector += bacteriaGrid[r+1][c-1];
        return neighbourVector;
    } else if (position == "middle"){
        neighbourVector += bacteriaGrid[r-1][c-1];
        neighbourVector += bacteriaGrid[r-1][c];
        neighbourVector += bacteriaGrid[r-1][c+1];
        neighbourVector += bacteriaGrid[r][c-1];
        neighbourVector += bacteriaGrid[r][c+1];
        neighbourVector += bacteriaGrid[r+1][c-1];
        neighbourVector += bacteriaGrid[r+1][c];
        neighbourVector += bacteriaGrid[r+1][c+1];
        return neighbourVector;
    }
}

// check how many living cells in the neighbour vectors
int livingNeighbourNum (Vector<char>& neighbourVector){
    int livingNeighbourNum = 0;
    for (int i = 0; i < neighbourVector.size(); i++){
        if (neighbourVector[i] == 'X'){
            livingNeighbourNum ++;
        }
    }
    return livingNeighbourNum;
}

// get the new status of a cell
//according to the living cell nums and its original status
char newCellStatus (int livingNeighbourNum, char oldCellStatus){
    char newCell;
    if (livingNeighbourNum <= 1){
        newCell = '-';
    } else if (livingNeighbourNum == 2) {
        newCell = oldCellStatus;
    } else if (livingNeighbourNum == 3) {
        newCell = 'X';
    } else if (livingNeighbourNum >= 4){
        newCell = '-';
    }
    return newCell;
}

// after all cells change, get the new grid
Grid<char> newBateriaGrid(Grid<char>& bacteriaGrid) {
    int rows = bacteriaGrid.numRows();
    int cols = bacteriaGrid.numCols();
    Grid<char> newBateriaGrid;
    newBateriaGrid.resize(rows,cols);
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            Vector<char> Neighbours;
            Neighbours = NeighbourVector(bacteriaGrid, r, c);
            int livingNum = livingNeighbourNum(Neighbours);
            newBateriaGrid[r][c] = newCellStatus(livingNum,bacteriaGrid[r][c]);
        }
    }
    return newBateriaGrid;
}

// make a one step function (tick)
Grid<char> tick(Grid<char>& bacteriaGridold) {
    Grid<char> bacteriaGridnew = newBateriaGrid(bacteriaGridold);
    return bacteriaGridnew;
}

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    // TODO: Finish the program!
    printOut();
    Grid<char> bacteriaGrid = fileInput();
    while (true){
        string command = getLine("a)nimate, t)ick, q)uit ");
    if (command == "a"){
        int animateNum = getInteger("How many frames? ");
        for (int i = 0; i < animateNum; i++){
            bacteriaGrid = tick(bacteriaGrid);
            clearConsole();
            printGrid (bacteriaGrid, bacteriaGrid.numRows(), bacteriaGrid.numCols());
            pause(50);
        }
    } else if (command == "t"){
        bacteriaGrid = tick(bacteriaGrid);
        printGrid (bacteriaGrid, bacteriaGrid.numRows(), bacteriaGrid.numCols());
    } else if (command == "q"){
        break;
    }
    else {
        cout << "Invalid choice; please try again." << endl;
    }
    }
  cout << "Have a nice Life!" << endl;
  return 0;
}
