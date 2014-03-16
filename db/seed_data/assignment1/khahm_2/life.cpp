/*
 * Katie Hahm; section leader: Jimmy Lee
 * This program is a simulation of the lifecycle of a bacteria colony.
 * Cells are shown as "X", and dead locations are shown as "-".
 * The cells live and die by the following rules:
 * 1. A cell with 1 or fewer neighbors dies.
 * 2. Locations with 2 neighbors remain stable.
 * 3. Locations with 3 neighbors will create life.
 * 4. A cell with 4 or more neighbors dies." << endl;
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

// Prints the introduction and instructions
void printIntro();
// Translates the grid described in the input file to a grid object
void makeGrid(ifstream &input, Grid<string>& colonyGrid);
// Performs one tick if user wants to advance the colony by one lifecycle
void performTick(Grid<string>& colonyGrid);
// Gets the number of neighbors of each cell, excluding itself
int getNeighbors(Grid<string>& colonyGrid, int r, int c);
// Performs an animation if user wants to animate with the given number of lifecycles
void performAnimation(int frameNum, Grid<string>& colonyGrid);
// Performs the simulation of Game of Life
void performGame(Grid<string>& colonyGrid);
// Returns a temporary grid of the correct size with completely dead cells
Grid<string> getTempGrid(Grid<string> &colonyGrid);

// The desired number of miliseconds the console should pause in between animations
const int PAUSE_TIME = 50;

/*
 * Main method of this program:
 * Sets up the console,
 * prints the introductory messages,
 * establishes the grid from the input file,
 * and plays the game until the user requests the game to quit.
 */
int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    printIntro();
    ifstream input;
    Grid<string> colonyGrid;
    makeGrid(input, colonyGrid);
    performGame(colonyGrid);
    cout << "Have a nice Life!" << endl;
    return 0;
}

/*
 * Prints the introductory statements and instructions.
 */
void printIntro() {
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << " " << endl;
}

/*
 * Translates the grid expressed in the input file to a grid object.
 * parameter: an input, a grid to be modified
 * It asks the user for the file name,
 * and keeps asking until a valid file name is received.
 * It opens the file, and makes the grid passed into the parameter
 * the desired size according to the file's first and second lines,
 * and then it translates the rest of the file into
 * the grid to display the alive and dead cells in the grid.
 */
void makeGrid(ifstream& input, Grid<string>& colonyGrid) {
    string fileName = getLine("Grid input file name? ");
    while (!fileExists(fileName)) {
        cout << "File not found; please try again." << endl;
        fileName = getLine("Grid input file name? ");
    }
    openFile(input, fileName);
    // uses the first line of the file to obtain number of rows
    string firstLine;
    getline(input, firstLine);
    int rowNum = stringToInteger(firstLine);
    // uses the second line of the file to obtain the number of columns
    string secondLine;
    getline(input,secondLine);
    int colNum = stringToInteger(secondLine);
    colonyGrid.resize(rowNum, colNum);
    for (int r = 0; r < rowNum; r++) {
        string line;
        getline(input, line);
        cout << line << endl;
        for (int c = 0; c < colNum; c++) {
            colonyGrid.set(r,c,line.substr(c,1));
        }
    }
}

/*
 * Performs the Game of Life!
 * parameters: the grid representing the initial colony translated from the input file
 * It asks the user whether to animate, tick, or quit the program.
 * If a, then the animation is shown by the given number of times.
 * If t, then the next lifecycle colony is shown.
 * If q, then the program finishes.
 * If the user inputs an invalid choice (not a, t, or q),
 * then it asks the user for another choice until it is valid.
 */
void performGame(Grid<string> &colonyGrid) {
    string commandLetter = getLine("a)nimate, t)ick, q)uit? ");
    while (commandLetter != "q") {
        if (commandLetter == "a") {
            int frameNum = getInteger("How many frames? ");
            performAnimation(frameNum, colonyGrid);
        }
        else if (commandLetter == "t") {
            performTick(colonyGrid);
        }
        else {
            cout << "Invalid choice; please try again." << endl;
        }
        commandLetter = getLine("a)nimate, t)ick, q)uit? ");
    }
}

/*
 * Performs and displays one tick, indicating one lifecycle.
 * parameters: the grid representing the current state of the colony
 * It decides from the current grid which cells should be alive or dead
 * in the next lifecycle, and then translates that decision
 * onto a temporary grid.
 * Then the temporary grid becomes the current state of the colony.
 * Prints the new state of grid.
 */
void performTick(Grid<string> &colonyGrid) {
    Grid<string> tempGrid = getTempGrid(colonyGrid);
    // makes the new grid
    for (int r = 0; r < colonyGrid.numRows(); r++) {
        for (int c = 0; c < colonyGrid.numCols(); c++) {
            int neighborNum = getNeighbors(colonyGrid, r, c);
            if (neighborNum == 3) {
                tempGrid[r][c] = "X";
            }
            else if (neighborNum == 2) {
                tempGrid[r][c] = colonyGrid[r][c];
            }
        }
    }
    colonyGrid = tempGrid;
    // prints the grid
    for (int a = 0; a < colonyGrid.numRows(); a++) {
        string gridLine = "";
        for (int b = 0; b < colonyGrid.numCols(); b++) {
            gridLine += colonyGrid[a][b];
        }
        cout << gridLine << endl;
        gridLine = "";
    }
}

/*
 * Makes the temporary, dead-celled grid.
 * parameter: the current colony grid
 * It calculates the size of the colony grid,
 * then makes a new grid with that same size,
 * and fills it with dead cells.
 * returns: the new grid with dead cells
 */
Grid<string> getTempGrid(Grid<string>& colonyGrid) {
    Grid<string> tempGrid(colonyGrid.numRows(), colonyGrid.numCols());
    for (int r = 0; r < colonyGrid.numRows(); r++) {
        for (int c = 0; c < colonyGrid.numCols(); c++) {
            tempGrid[r][c] = "-";
        }
    }
    return tempGrid;
}

/*
 * Calculates the number of alive neighbors
 * parameter: current state of colony in grid,
 * parameter: the target row and column number
 * It calculates how many neighbors are alive around
 * the target row and column number with a counter,
 * excludes the target cell.
 * returns: the final value of the counter.
 */
int getNeighbors(Grid<string>& colonyGrid, int r, int c) {
    int counter = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (colonyGrid.inBounds(r+i, c+j)) {
                if (colonyGrid.get(r+i,c+j) == "X") {
                    counter++;
                }
            }
        }
    }
    if (colonyGrid[r][c] == "X") {
        counter--;
    }
    return counter;
}

/*
 * Prints the desired number of new lifecycles
 * parameter: number of desired new lifecycles
 * parameter: the current state of colony in a grid
 * It calls the performTick method until the desired number of
 * new lifecycles is satisfied.
 * Between each printing, the console is paused and cleared
 * to display an animation effect.
 */
void performAnimation(int frameNum, Grid<string> &colonyGrid) {
    for (int i = 0; i < frameNum; i++) {
        pause(PAUSE_TIME);
        clearConsole();
        performTick(colonyGrid);
    }
}
