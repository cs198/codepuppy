/* Name: Lindsey Txakeeyang
 * Course: CS106B, Wednesday 4:15 Section
 * Assignment: The Game of Life
 * This program intakes specific text files and then models the life cycle of bacteria
 * using a two-dimensional grid of cells built from the text file.
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

void intro();
Grid<bool> buildInitialGrid(ifstream &input, Grid<bool>& lifeGrid);
Grid<bool> populateCellsUsingFile(ifstream& input, Grid<bool>& lifeGrid);
void newGeneration(Grid<bool>& lifeGrid);
int checkNeighbors(Grid<bool>& lifeGrid, int r, int c);
bool isActuallyNeighbor(int x, int y, int r, int c);
void printGrid(Grid<bool>& lifeGrid);
void handleUserChoices(Grid<bool>& lifeGrid);
void animate(Grid<bool>& lifeGrid);
void tick(Grid<bool>& lifeGrid);
ifstream getInputFileFromUser();

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    intro();
    Grid<bool> lifeGrid;
    //ifstream input = getInputFileFromUser(); see function comment below
    ifstream input;
    while (true) {
        string filename;
        cout << "Grid input file name? ";
        cin >> filename;
        if(fileExists(filename)) {
            input.open(filename.c_str());
            break;
        } else {
            cout << "File not found; please try again." << endl;
        }
    }
    lifeGrid = buildInitialGrid(input, lifeGrid);
    printGrid(lifeGrid);
    handleUserChoices(lifeGrid);

    cout << "Have a nice Life!" << endl;
    return 0;
}

/*
 * Prints a welcome message explaining the program.
 */
void intro() {
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << endl;
}

/*
 * Tried creating a function to open the file outside of the main however I kept getting the
 * error, "use of deleted function" and then couldn't figure out what was wrong.
ifstream getInputFileFromUser() {
    ifstream input;
    while (true) {
        string filename;
        cout << "Grid input file name? ";
        cin >> filename;
        if(fileExists(filename)) {
            input.open(filename.c_str());
            break;
        } else {
            cout << "File not found; please try again." << endl;
        }
    }
    return input;
}*/

/*
 * Builds the starting point for the grid by reading in the file and setting the
 * cells of the grid accordingly.
 */
Grid<bool> buildInitialGrid(ifstream& input, Grid<bool>& lifeGrid) {
    int rows;
    int columns;
    string line;
    for (int n = 2; n > 0; n--) {
        getline(input, line);
        istringstream tokens(line);
        if (n==2) tokens >> rows;
        if (n==1) tokens >> columns;
    }
    lifeGrid.resize(rows, columns);
    lifeGrid = populateCellsUsingFile(input, lifeGrid);
    return lifeGrid;
}

/*
 * Sets the cell in the grid of booleans to either true or false depending on
 * what the file being read has set for that particular cell.
 */
Grid<bool> populateCellsUsingFile(ifstream& input, Grid<bool>& lifeGrid) {
    string line;
    for (int r = 0; r < lifeGrid.numRows(); r++) {
        getline(input, line);
        for (int c = 0; c < lifeGrid.numCols(); c++) {
            if (line[c] == '-') {
                lifeGrid.set(r, c, false);
            }
            if (line[c] != '-') {
                lifeGrid.set(r, c, true);
            }
        }
    }
    return lifeGrid;
}

/*
 * Builds the new grid after taking into considerations the rules of the game.
 */
void newGeneration(Grid<bool>& lifeGrid) {
    Grid<bool> tempGrid(lifeGrid.numRows(), lifeGrid.numCols());
    for (int r = 0; r < lifeGrid.numRows(); r++) {
        for (int c = 0; c < lifeGrid.numCols(); c++) {
            int numNeighbors = checkNeighbors(lifeGrid, r, c);
            if (numNeighbors <= 1) {
                tempGrid.set(r, c, false);
            } else if (numNeighbors == 2) {
                tempGrid[r][c] = lifeGrid[r][c];
            } else if (numNeighbors == 3) {
                tempGrid.set(r, c, true);
            } else if (numNeighbors >= 4) {
                tempGrid.set(r, c, false);
            }
        }
    }
    lifeGrid = tempGrid;
}

/*
 * Checks for how many neighbors a particular square on the grid has.
 * Only works if the square being checked is within the grid's bounds.
 */
int checkNeighbors(Grid<bool>& lifeGrid, int r, int c) {
    int counter = 0;
    for (int x = r - 1; x <= r + 1; x++) {
        for (int y = c - 1; y <= c + 1; y++) {
            if (lifeGrid.inBounds(x, y) && lifeGrid[x][y] == true && isActuallyNeighbor(x, y, r, c)) {
                counter++;
            }
        }
    }
    return counter;
}

/*
 * Checks to make sure the square being checked is actually a neighbor.
 */
bool isActuallyNeighbor(int x, int y, int r, int c) {
    if (x != r || y != c) return true;
    else return false;
}

/*
 * Prints the grid to the console.
 */
void printGrid(Grid<bool>& lifeGrid) {
    for (int r = 0; r < lifeGrid.numRows(); r++) {
        for (int c = 0; c < lifeGrid.numCols(); c++) {
            if (lifeGrid[r][c] == true) {
                cout << "X";
            } else {
                cout << "-";
            }
        }
        cout << endl;
    }
}

/*
 * Provides the user with the options of animating, ticking, or quitting the program.
 */
void handleUserChoices(Grid<bool>& lifeGrid) {
    string answer;
    while (true) {
        cout << "a)nimate, t)ick, q)uit? ";
        cin >> answer;
        if (answer == "q") {
            break;
        } else if (answer == "a") {
            animate(lifeGrid);
        }
        else if (answer == "t") {
            tick(lifeGrid);
        }
        else {
            cout << "Invalid choice; please try again." << endl;
        }
    }
}

/*
 * Asks user to input how many frames to comment then animates it accordingly.
 */
void animate(Grid<bool>& lifeGrid) {
    int frames;
    cout << "How many frames? ";
    cin >> frames;
    for (int n = 0; n <= frames; n++) {
        tick(lifeGrid);
        pause(100);
        clearConsole();
    }
}

/*
 * Ticks the program by generating a new generation grid and printing it.
 */
void tick(Grid<bool>& lifeGrid) {
    newGeneration(lifeGrid);
    printGrid(lifeGrid);
}
