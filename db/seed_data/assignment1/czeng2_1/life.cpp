/* Connie Zeng
 * CS 106B, Winter 2014
 * ---------------------------
 * Assignment 1: Game of Life
 * This program models the growth of bacteria in a two-dimensional grid,
 * with the initial colony read from a file.
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
#include "lifegui.h"
using namespace std;

/* Function prototypes */

void loadBacteria(Grid<char>& colony, Vector<string>& lines);
void growBacteria(Grid<char>& colony);
void animate(Grid<char>& colony);
void tick(Grid<char>& colony);
int countNeighbors(Grid<char>& colony, int row, int column);

/* Main program */

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl << endl;

    ifstream wfile;
    Grid<char> colony;
    Vector<string> lines;

    // Asks user for file to open as initial colony
    promptUserForFile(wfile, "Grid input file name? ");
    readEntireFile(wfile, lines);
    wfile.close();

    // Displays each generation of bacteria to console
    loadBacteria(colony, lines);
    growBacteria(colony);

    cout << "Have a nice Life!" << endl;
    return 0;
}

/* Function: loadBacteria
 * Usage: loadBacteria(colony, lines);
 * ------------------------------------
 * This function determines the initial state of the bacteria colony based
 * on the lines read from the input file. The colony grid is filled in
 * with '-' for empty cells and 'X' for live bacteria. The grid is then
 * printed to the screen for the user to have a visual representation of
 * the initial colony.
 * Files are assumed to have proper formatting: the first line contains the
 * number of rows, the second line contains the number of columns, and
 * following lines contain the information about the initial colony.
 */

void loadBacteria(Grid<char>& colony, Vector<string>& lines){
    //Resizes colony grid according to file specifications
    int nRows = stringToInteger(lines[0]);
    int nCols = stringToInteger(lines[1]);
    colony.resize(nRows, nCols);

    //Displays colony to console
    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols; j++){
            colony[i][j] = lines[i+2][j];
            cout << colony[i][j];
        }
        cout << endl;
    }
}

/* Function: growBacteria
 * Usage: growBacteria(colony);
 * -----------------------------
 * This function prompts the user for a command and proceeds to animate the
 * colony, tick the colony, or quit the simulation. If the user input is
 * not one of the given options, the user is given additional chances to
 * enter a valid command.
 */

void growBacteria(Grid<char>& colony){
    string command;
    while(true){
        command = getLine("a)nimate, t)ick, q)uit? ");
        if(command == "a") animate(colony);
        else if(command == "t") tick(colony);
        else if(command == "q") break;
        else cout << "Invalid choice; please try again." << endl;
    }
}

/* Function: animate
 * Usage: animate(colony);
 * ------------------------
 * This function asks the user for a number of generations to grow the
 * bacteria and animates through that many generations, updating every 50
 * milliseconds.
 */

void animate(Grid<char>& colony){
    int gen = getInteger("How many frames? ");
    for (int i = 0; i < gen; i++){
        clearConsole();
        tick(colony);
        pause(50);
    }
}

/* Function: tick
 * Usage: tick(colony);
 * ---------------------
 * This function ticks the colony forward by one generation, updating each
 * location based on the number of neighbors: 0 or 1 becomes empty,
 * 2 remains the same, 3 generates a live bacteria, and 4+ becomes empty.
 * The resulting colony grid is printed to the screen.
 */

void tick(Grid<char>& colony){
    Grid<char> temp = colony;
    int neighbors;
    for(int i = 0; i < colony.numRows(); i++){
        for(int j = 0; j < colony.numCols(); j++){
            neighbors = countNeighbors(colony, i, j);
            if((neighbors < 2) || (neighbors > 3)) temp[i][j] = '-';
            else if(neighbors == 3) temp[i][j] = 'X';
            cout << temp[i][j];
        }
        cout << endl;
    }
    colony = temp;
}

/* Function: countNeighbors
 * Usage: int count = countNeighbors(colony, row, column);
 * --------------------------------------------------------
 * This function counts and returns the number of neighbors a location at
 * the given row and column in the colony grid has. A neighbor is a live
 * bacteria in one of the eight adjacent locations around the given location.
 */

int countNeighbors(Grid<char>& colony, int row, int column){
    int count = 0;
    int r, c;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            r = row + i;
            c = column + j;
            if(colony.inBounds(r, c) && !(i == 0 && j == 0) && colony[r][c] == 'X'){
                count++;
            }
        }
    }
    return count;
}

