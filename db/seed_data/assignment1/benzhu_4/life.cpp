//Name: Benzheng Zhu
//CS 106B Assignment 1
//Citation: lecture slides
//This is a Game of Life program, which reads the initial state of grid from the given .txt file
//and models the life cycle of bacteria from one generation to another.

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

//function prototypes
void welcome();
void openFile(ifstream& input);
void readFile(ifstream& input, string& line, int& row, int& col);
void initializeGrid(Grid<string>& grid, string line, int row, int col);
void printGrid(Grid<string> grid);
int check(string cell);
int countNeighbor(Grid<string> grid, int r, int c);
void tick(Grid<string>& grid);
void animation(Grid<string>& grid);
void run(string& choice,Grid<string>& grid);

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    welcome();

    ifstream input;
    openFile(input);

    //read information from the file
    string line;
    int row; int col;
    readFile(input, line, row, col);

    Grid<string> grid;
    initializeGrid(grid, line, row, col);

    printGrid(grid);

    string choice = "";
    //run the program until the user stops it
    while (choice != "q"){
        run(choice,grid);
    }
    return 0;
}

void welcome() {
    //this function prints the greeting and the rules of the game

    cout << "Welcome to the CS 106B Game of Life," << endl
         << "a simulation of the lifecycle of a bacteria colony." << endl
         << "Cells (X) live and die by the following rules:" << endl
         << "- A cell with 1 or fewer neighbors dies." << endl
         << "- Locations with 2 neighbors remain stable." << endl
         << "- Locations with 3 neighbors will create life." << endl
         << "- A cell with 4 or more neighbors dies." << endl << endl;
}

void openFile(ifstream& input) {
    //this function asks for a file name untila a valid name is provided
    //and then opens that file

    string name = getLine("Grid input file name? ");

    if (fileExists(name)) {
        input.open(name);
    }
    else {
        cout << "File not found; please try again."<< endl;
        openFile(input);
    }
}

void readFile(ifstream& input, string& line, int& row, int& col) {
    //this function takes the ifstream, the pre-defined string and the two integers as inputs
    //it reads the first two lines of the file, converts the text to integers, and stores them by reference
    //then it reads the rest of the file and store it into the string

    string temp;
    getline(input, temp);
    row = stringToInteger(temp);
    getline(input, temp);
    col = stringToInteger(temp);

    while (getline(input, temp)) {
        line += temp;
    }
    input.close();
}

void initializeGrid(Grid<string>& grid, string line, int row, int col) {
    //this function takes a grid by reference, a string and two integers as inputs
    //it first resizes the grid using the two integers, and then converts the string into a grid

    grid.resize(row,col);
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            grid[i][j]=line[j+col*i];
        }
    }
}

void printGrid(Grid<string> grid) {
    //this function takes a grid as input, and prints out that grid

    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            cout << grid[r][c];
        }
        cout << endl;
    }
}

int check(string cell) {
    //this function check if the input string is "X"
    //if yes, it returns integer 1; if not, it returns 0.

    if (cell == "X") {
        return 1;
    }
    else {
        return 0;
    }
}

int countNeighbor(Grid<string> grid, int r, int c) {
//this function takes the grid, the row index and the column index as inputs
//it counts the neighbor of a certain cell, specified by the two indices,
    int count = 0;
    if (r==0) { //if the cell is located in the first row
        if (c==0) { //if in the upper left corner
            count = check(grid[r+1][c])+ check(grid[r+1][c+1])+check(grid[r][c+1]);
        }
        else if (c==grid.numCols()-1) { // if in the upper right corner
            count = check(grid[r+1][c])+ check(grid[r+1][c-1])+check(grid[r][c-1]);
        }
        else { //on the upper boundary (but not in the corners)
            count = check(grid[r][c-1])+ check(grid[r][c+1])+check(grid[r+1][c-1])+check(grid[r+1][c])+check(grid[r+1][c+1]);
        }
    }
    else if (r==grid.numRows()-1) { //if the cell is located in the last row
        if (c==0) { //if in the lower left corner
            count = check(grid[r-1][c])+ check(grid[r-1][c+1])+check(grid[r][c+1]);
        }
        else if (c==grid.numCols()-1) { //if in the lower right corner
            count = check(grid[r-1][c])+ check(grid[r-1][c-1])+check(grid[r][c-1]);
        }
        else {//if in the lower boundary (but not in the corners)
            count = check(grid[r][c-1])+ check(grid[r][c+1])+check(grid[r-1][c-1])+check(grid[r-1][c])+check(grid[r-1][c+1]);
        }
    }
    else {
        if (c==0) {//if on the left boundary
            count = check(grid[r-1][c])+ check(grid[r-1][c+1])+check(grid[r][c+1])+check(grid[r+1][c+1])+check(grid[r+1][c]);
        }

        else if (c==grid.numCols()-1) {//if on the right boundary
            count = check(grid[r-1][c])+ check(grid[r-1][c-1])+check(grid[r][c-1])+check(grid[r+1][c-1])+check(grid[r+1][c]);
        }
        else {//if in the middle area
            for (int i=r-1; i<=r+1; i++) {
                for (int j=c-1; j<=c+1; j++) {
                    count += check(grid[i][j]);
                }
            }
            count = count - check(grid[r][c]); //minus the cell itself from the count of neighbors
        }
    }
    return count;
}

void tick(Grid<string>& grid) {
    //this function takes a grid by reference as input, and performs one generation of simulation

    Grid<string> tempgrid = grid;
    for (int r = 0; r < tempgrid.numRows(); r++) {
        for (int c = 0; c < tempgrid.numCols(); c++) {
            int neighbor = countNeighbor(tempgrid,r,c);
            if (neighbor == 3) {
                grid[r][c]="X";
            }
            else if ((neighbor <= 1)||(neighbor >= 4)) {
                grid[r][c]="-";
            }
        }
    }
    printGrid(grid);
}

void animation(Grid<string>& grid) {
    //this function takes the grid by reference as an input
    //asks for the number of frames, and performs that many generations of simulation

    int frame = getInteger("How many frames? ");

    for (int i=0; i<frame; i++ ) {
        clearConsole();
        tick(grid);
        pause(50); //pause for 50 milliseconds so that the user can watch the animation
    }
}

void run(string& choice,Grid<string>& grid) {
    //this function passes the grid and user's command by reference,
    //asks for user's command, and calls the corresponding function

    choice = getLine("a)nimate, t)ick, q)uit? ");

    if (choice == "a") {
        animation(grid);
    }
    else if (choice == "t") {
        tick(grid);
    }
    else if (choice == "q") {
        cout << "Have a nice Life!";
    }
    else {
        cout << "Invalid choice; please try again." << endl;
        run(choice, grid);
    }
}
