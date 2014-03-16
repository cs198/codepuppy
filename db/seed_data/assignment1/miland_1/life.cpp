/**
 * Name: Milan Doshi
 * Assignment: life.cpp
 * TA: Jimmy Lee
 *
 * This program runs the game of life. It allows the user to see the instructions, type in a file name, and then either quit,
 * tick, or animate the generation.
 */

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
#include "strlib.h"
using namespace std;

Grid<char>colony;
Grid<char>temp;
int nrows, ncols;

void message();
void readAndPrintGrid();
void printGrid();
void updateGeneration();
void animate();
void tick();

int main() {
    setConsoleSize(700, 400);
    message();
    readAndPrintGrid();
    updateGeneration();
    return 0;
}
/**
 * method: message()
 * return: void
 * This method prints out the initial message the user will see when they start the program.
 */
void message() {
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << "" << endl;
}

/**
 * method: voidAndPrintGrid()
 * return: void
 * This method reads in the initial grid from the file and then prints it out.
 */
void readAndPrintGrid() {
    ifstream input;
    string filename;
    cout << "Grid input file name?";
    cin >> filename;
    input.open(filename.c_str());

    string rows;
    getline(input, rows);
    istringstream rtokens(rows);
    rtokens >> nrows;
    string cols;
    getline(input, cols);
    istringstream ctokens(cols);
    ctokens >> ncols;

    string line;
    colony.resize(nrows,ncols);
    for(int r = 0; r < nrows; r++) {
        getline(input, line);
        istringstream tokens(line);
        for(int c = 0; c < ncols; c++) {
            tokens >> colony[r][c];
            cout << colony[r][c];
        }
        cout << " " << endl;
    }
    input.close();
}

/**
 * method: printGrid()
 * return: void
 * This method prints out the grid.
 */
void printGrid() {
    for(int r = 0; r < nrows; r++) {
        for(int c = 0; c < ncols; c++) {
            cout << colony[r][c];
        }
        cout << " " << endl;
    }
}

/**
 * method: message()
 * return: void
 * This method allows the user to continue going through the program until "q" has been typed in. Based on the response provided
 * by the user, the program decides what action to take based on the if/else statements (animate, tick, or end the program if "q"
 * is typed).
 */
void updateGeneration() {
    string answer;
    temp.resize(colony.numRows(), colony.numCols());
    while (answer != "q") {
        cout << "a)nimate, t)ick, q)uit?";
        cin >> answer;
        if(answer == "a") {
            animate();
        } else if (answer == "t") {
            tick();
            printGrid();
        } else if (answer == "q") {
            cout << "Have a nice Life!" << endl;
        }
    }
}

/**
 * method: animate()
 * return: void
 * This method clears the console and then animates the generation based on the user provided number of frames, showing each
 * generation, and then ending on the last animated frame.
 */
void animate() {
    int frames;
    cout << "How many frames?";
    cin >> frames;
    clearConsole();
    for(int i = 0; i < frames; i++){
        tick();
        printGrid();
        pause(50);
        if(i != frames -1) clearConsole();
    }
}

/**
 * method: tick()
 * return: void
 * This method checks all the neighbors of a certain location, counts the number of neighbors, and then based  on the number of
 * neighbors, the location either stays the same, dies, or gives life.
 */
void tick() {
    int neighbors = 0;
    for(int r = 0; r < nrows; r++) {
        for(int c = 0; c < ncols; c++) {

            if(colony.inBounds(r-1,c-1)){ //topLeft
                if(colony[r-1][c-1] == 'X') neighbors++;
            }
            if(colony.inBounds(r-1,c)) {   //top
                if(colony[r-1][c] == 'X') neighbors++;
            }
            if(colony.inBounds(r-1,c+1)) { //topRight
                if(colony[r-1][c+1] == 'X') neighbors++;
            }
            if(colony.inBounds(r+1,c-1)) { //botLeft
                if(colony[r+1][c-1] == 'X') neighbors++;
            }
            if(colony.inBounds(r+1,c)) {   //bot
                if(colony[r+1][c] == 'X') neighbors++;
            }
            if(colony.inBounds(r+1,c+1)) { //botRight
                if(colony[r+1][c+1] == 'X') neighbors++;
            }
            if(colony.inBounds(r,c-1)) {   //leftSide
                if(colony[r][c-1] == 'X') neighbors++;
            }
            if(colony.inBounds(r,c+1)) {   //rightSide
                if(colony[r][c+1] == 'X') neighbors++;
            }

            if(neighbors <= 1) {
                temp[r][c] = '-';
                neighbors = 0;
            } else if (neighbors == 2) {
                temp[r][c] = colony.get(r,c);
                neighbors = 0;
            } else if (neighbors == 3) {
                temp[r][c] = 'X';
                neighbors = 0;
            } else if (neighbors >= 4) {
                temp[r][c] = '-';
                neighbors = 0;
            }
        }
    }
    colony = temp;
}







