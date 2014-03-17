//// Elle Wilson
/// This program will read in text files containing certain patterns of bacteria.
/// It will then simulate the way the bacteria would behave in real-world conditions, mimicking
/// the way the bacteria would live or die in relation to each other. Users can control the
/// progression of the life cycle, by deciding to tick forward or animate the sequence.
/// Sources used: Textbook, "Programming Abstractions in C++"; help from LAiR

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

void setup();
void fileToGrid(string filename, Grid<char>& grid, int& rows, int& columns);
void displayGrid(Grid<char>& grid, int rows, int columns);
int checkNeighbors(Grid<char>& grid, int row, int column);
void updateGrid(Grid<char>& grid, Grid<char>& grid2, int row, int column, int neighborCounter);
void incrementNeighborCounter(char neighbor, int& neighborCounter);
void aChoice(Grid<char>& grid, int rows, int columns);
void tChoice(Grid<char>& grid, int rows, int columns);
void acceptInput(Grid<char>& grid, int rows, int columns);

int main() {
    setup();
    Grid<char> grid;
    string filename=getLine("Grid input file name? ");
    int rows;
    int columns;
    fileToGrid(filename, grid, rows, columns);
    displayGrid(grid, rows, columns);
    acceptInput(grid, rows, columns);
    cout << "Have a nice Life!" << endl;
    return 0;
}

//This method sets up the console and lays out the opening text at the beginning of the program
void setup() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "-A cell with 1 or fewer neighbors dies." << endl;
    cout << "-Locations with 2 neighbors remain stable." << endl;
    cout << "-Locations will 3 neighbors will create life." << endl;
    cout << "-A cell with 4 or more neighbors dies." << endl;
    cout << "" << endl;
}

//This method reads in a file and converts its contents into a grid. The grid itself, the variable storing the number of rows, and the variable storing the number of columns will all be passed in by reference
void fileToGrid(string filename, Grid<char>& grid, int& rows, int& columns) {
    Grid<string> grid1;
    ifstream infile;
    string line;
    int turncounter=0;
    while(true) {
        openFile(infile, filename);
        if(infile.fail()) {
            cout << "File not found; please try again." << endl;
            filename=getLine("Grid input file name? ");
        } else {
            break;
        }
    }
    while(getline(infile, line)) {
        if(turncounter==0) {
            rows=stringToInteger(line);
        } else if(turncounter==1) {
            columns=stringToInteger(line);
            grid1.resize(rows, 1);
        }else if(turncounter<rows+2){
            grid1.set(turncounter-2, 0, line);
        }
        turncounter++;
    }
    grid.resize(rows, columns);
    for(int i=0; i<rows; i++) {
        string currentLine=grid1[i][0];
        for(int x=0; x<columns; x++) {
            char currentChar=currentLine.at(x);
            grid.set(i, x, currentChar);
        }
    }
}

//This method performs the actions when "a," or "animate," is selected; it shifts the life cycle forward by the user-generated number of frames
void aChoice(Grid<char>& grid, int rows, int columns) {
    int aChoice=getInteger("How many frames? ");
    Grid<char> grid2=grid;
    for(int i=0; i<aChoice; i++) {
        clearConsole();
        for(int i=0; i<rows; i++) {
            for(int j=0; j<columns; j++) {
                int neighborCounter=checkNeighbors(grid, i, j);
                updateGrid(grid, grid2, i, j, neighborCounter);
            }
        }
        grid=grid2;
        displayGrid(grid, rows, columns);
        pause(50);
    }
}

//This method is called when the user types "t" for "tick," to shift the simulation forwards one frame
void tChoice(Grid<char>& grid, int rows, int columns) {
    Grid<char> grid2=grid;
    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++) {
            int neighborCounter=checkNeighbors(grid, i, j);
            updateGrid(grid, grid2, i, j, neighborCounter);
        }
    }
    grid=grid2;
    displayGrid(grid, rows, columns);
}

//This method displays the most recent incarnation of the grid
void displayGrid(Grid<char>& grid, int rows, int columns) {
    for(int i=0; i<rows; i++) {
        for(int x=0; x<columns; x++) {
            cout << grid[i][x];
        }
        cout << endl;
    }
}

//This method updates the counter that keeps track of the number of neighbors that a cell has
void incrementNeighborCounter(char neighbor, int& neighborCounter) {
    if(neighbor=='X') {
        neighborCounter++;
    }
}

//This method checks all of the neighbors of a particular cell and updates a counter to keep track of the total, and returns the integer value of the number of neighbors
//This method will be called each time an individual cell needs to be checked for neighbors, so the row and column of that cell will be passed in, as will the grid itself by reference
int checkNeighbors(Grid<char>& grid, int row, int column) {
    int neighborCounter=0;
    char neighbor;
    if(grid.inBounds(row-1, column-1)) {
        neighbor=grid[row-1][column-1];
    }
    incrementNeighborCounter(neighbor, neighborCounter);
    if(grid.inBounds(row-1, column)) {
        neighbor=grid[row-1][column];
    }
    incrementNeighborCounter(neighbor, neighborCounter);
    if(grid.inBounds(row-1, column+1)) {
        neighbor=grid[row-1][column+1];
    }
    incrementNeighborCounter(neighbor, neighborCounter);
    if(grid.inBounds(row, column-1)) {
        neighbor=grid[row][column-1];
    }
    incrementNeighborCounter(neighbor, neighborCounter);
    if(grid.inBounds(row, column+1)) {
        neighbor=grid[row][column+1];
    }
    incrementNeighborCounter(neighbor, neighborCounter);
    if(grid.inBounds(row+1, column-1)) {
        neighbor=grid[row+1][column-1];
    }
    incrementNeighborCounter(neighbor, neighborCounter);
    if(grid.inBounds(row+1, column)) {
        neighbor=grid[row+1][column];
    }
    incrementNeighborCounter(neighbor, neighborCounter);
    if(grid.inBounds(row+1, column+1)) {
        neighbor=grid[row+1][column+1];
    }
    incrementNeighborCounter(neighbor,neighborCounter);
    return neighborCounter;
}

//This method updates the grid that contains all of the cells. The grid itself, and a duplicate grid, are passed in by reference.
//This method will be called each time an individual cell in the grid is being updated, so the row, column, and number of neighbors of that cell will also be passed in.
void updateGrid(Grid<char>& grid, Grid<char>& grid2, int row, int column, int neighborCounter) {
    if(neighborCounter<=1) {
        if(grid[row][column]=='X') {
            grid2[row][column]='-';
        }
    }
    if(neighborCounter==3) {
        if(grid[row][column]=='-') {
            grid2[row][column]='X';
        }
    }
    if(neighborCounter>=4) {
        if(grid[row][column]=='X') {
            grid2[row][column]='-';
        }
    }
}

//This method accepts user input to run the simulation. The user is given the choice of animating, "ticking," or quitting the application.
//The number of rows and columns of the chosen grid are passed in, as is the grid itself by reference
void acceptInput(Grid<char>& grid, int rows, int columns) {
    string choice;
    while(true) {
        choice=getLine("a)nimate, t)ick, q)uit? ");
        //cin >> choice;
        if(choice!="a" && choice!="t" && choice!="q") {
            cout << "That is not a valid choice, please try again." << endl;
        }
        if(choice=="a") {
            aChoice(grid, rows, columns);
        }
        if(choice=="t") {
            tChoice(grid, rows, columns);
        }
        if(choice=="q") {
            break;
        }
    }

}
