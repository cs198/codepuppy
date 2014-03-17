/*
 * Emma Daugherty; CS106B; Section Leader: Jimmy Lee
 *
 * This program allows users to play The Game of Life, a simulation of the life cycle of bacteria.
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

//Methods
void printIntro();
void readFile(Grid<char>& grid);
void displayGrid(Grid<char>& grid);
void animate(Grid<char>& grid, int numFrames);
void tick(Grid<char>& grid);
int numNeighbors(Grid<char>&, int row, int col);
void performAction(Grid<char>& grid);


int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    printIntro();

    Grid<char> grid;
    readFile(grid);

    displayGrid(grid);

    performAction(grid);

    cout << "Have a nice Life!" << endl;
    return 0;
}

/*
 * This function prints an introduction to the game on the screen that includes the rules.
 */
void printIntro() {
    cout << "Welcome to the CS106B Game of Life," << endl;
    cout << "A simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << "" << endl;
}

/*
 * This function takes in a grid for the data from the file to be loaded into
 * as the paramater. It reads the data from the file, and formats it into the grid. The grid that is passed in is
 * permanantly changed.
 */
void readFile(Grid<char> & grid) {
    string r;
    string c;
    int rows;
    int columns;

    ifstream infile;
    promptUserForFile(infile, "Grid input file name? ");
    string line;
    getline(infile, r);
    rows = stringToInteger(r);
    getline(infile, c);
    columns = stringToInteger(c);
    grid.resize(rows, columns);
    for (int j = 0; j < rows; j++) {
         getline(infile, line);
         for (int i = 0; i < columns; i++) {
             char cell = line[i];
             grid[j][i] = cell;
         }
    }
    infile.close();
}

/*
 * This function displays a grid on the screen. It takes in a grid as a parameter.
 */
void displayGrid(Grid<char>& grid) {
    for (int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

/*
 * This function takes in a grid as a parameter. It is assumed that the grid contains data consistent with the
 * Game of Life and represents bacteria with 'X' and blank spaces with '-'. Following the Game of Life rules, it
 * advances the grid by one turn.
 */
void tick(Grid<char>& grid) {
    Grid<char> copy = grid;

    for (int i = 0; i < grid.numRows() - 1; i++) {
        for (int j = 0; j < grid.numCols() - 1; j++) {
            int neighbors = numNeighbors(grid, i, j);
            if (neighbors <= 1 || neighbors >= 4) {
                copy[i][j] = '-';
            } else if (neighbors == 3) {
                copy[i][j] = 'X';
            }
        }
    }

    grid = copy;
    displayGrid(grid);
}

/*
 * This function is a helper method for the tick() function. It takes in a grid that holds the game configuration,
 * and one space on that grid defined by its row and column. This method checks to see how many neighbors, denoted
 * by 'X', that the space has. It returns the number of neighbors.
 */
int numNeighbors(Grid<char>& grid, int row, int col) {
    int result = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (grid.inBounds(i, j) && grid[i][j] == 'X' && !(i == row && j == col)) {
                result++;
            }
        }
    }
    return result;
}

/*
 * This function animates the grid by ticking foward a user specified number of frames. The parameters are a grid and
 * the number of frames to tick foward by.
 */
void animate(Grid<char>& grid, int numFrames) {
    for (int i = 0; i < numFrames; i++) {
        clearConsole();
        tick(grid);
        pause(50);
    }
}

/*
 * This function takes in user input and based on that input decides which action to perform. The parameter is a grid
 * that the action will be performed on.
 */
void performAction(Grid<char>& grid) {
    string act = getLine("a)nimate, t)ick, q)uit? ");

    while (act != "q") {

        if (act == "a") {
            int numFrames = stringToInteger(getLine("How many frames? "));
            animate(grid, numFrames);
        } else if (act == "t") {
            tick(grid);
        } else {
            cout << "Invalid choice; please try again." << endl;
        }

        act = getLine("a)nimate, t)ick, q)uit? ");
    }
}

