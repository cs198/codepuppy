/* CS 106B Winter 2014
 * Michelle S. Lee
 * Assignment 1
 * January 17, 2014
 *
 * This program simulates Conway's Game of Life.
 * Cells (X) live and die by the following rules:
 *   - A cell with 1 or fewer neighbors dies.
 *   - Locations with 2 neighbors remain stable.
 *   - Locations with 3 neighbors will create life.
 *   - A cell with 4 or more neighbors dies.
 *
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

const int PAUSE_INTERVAL = 50; // Animation interval in miliseconds

const char* HEADER = "Welcome to the CS 106B Game of Life, \n"
        "a simulation of the lifecycle of a bacteria colony. \n"
        "Cells (X) live and die by the following rules: \n"
        "- A cell with 1 or fewer neighbors dies. \n"
        "- Locations with 2 neighbors remain stable. \n"
        "- Locations with 3 neighbors will create life. \n"
        "- A cell with 4 or more neighbors dies.";

//Function prototypes
void getGrid(string &filename, Grid<bool> &grid, int &NROW, int &NCOL);
Grid<bool> updateGeneration(Grid<bool> &grid, int &NROW, int &NCOL);
void printGrid(Grid<bool> &grid, int &NROW, int &NCOL);
void animate(Grid<bool> &grid, int &NROW, int &NCOL);


/* This function takes a file name, empty grid, and two constants.
 * It opens and reads the file, stores the number of rows and columns into the constants,
 * and stores the location of the dead/alive cells into the boolean grid.
*/
void getGrid(string &filename, Grid<bool> &grid, int &NROW, int &NCOL) {
    ifstream infile;
    openFile(infile, filename);
    if (infile.fail()) {
        error("file didn't open");
        infile.clear();
    } else {
        string line;
        getline(infile, line);
        NROW = stringToInteger(line);
        getline(infile, line);
        NCOL = stringToInteger(line);
        grid.resize(NROW, NCOL);

        for (int i = 0; i < NROW; i++) {
                getline(infile, line);
            for (int j = 0; j < NCOL; j++) {
                 grid[i][j] = (line[j] == 'X');
            }
        }
        infile.close();
    }
}

/* This function updates the "generation" of cells. If a cell has two neighbors, it stays as is.
 * If a cell has three neighbors, it is alive (returns true). Otherwise, the cell dies.
 * It returns a boolean grid of the updated generation.
 */
Grid<bool> updateGeneration(Grid<bool> &grid, int &NROW, int &NCOL) {
    Grid<bool> gridcopy;
    gridcopy.resize(NROW, NCOL);
    for (int i = 0; i < NROW; i++) {
        for (int j = 0; j < NCOL; j++) {
            int counter = 0;
            for (int r = i-1; r <= i+1; r++) {
                for (int c = j-1; c <= j+1; c++) {
                    if (grid.inBounds(r, c) && !(r == i && c == j)) {
                        if(grid[r][c] == true) {
                            counter++;
                        }
                    }
                }
            }

            if (counter == 2) {
                gridcopy[i][j] = grid[i][j];
            } else if (counter == 3) {
                gridcopy[i][j] = true;
            } else {
                gridcopy[i][j] = false;
            }
        }
    }
    return gridcopy;
}

/* This function prints the boolean grid into the console with '-' as dead cells and
 * 'X' as living cells */
void printGrid(Grid<bool> &grid, int &NROW, int &NCOL) {
    for (int i = 0; i < NROW; i++) {
        for (int j = 0; j < NCOL; j++) {
            if (grid[i][j] == false) {
                cout << '-' << "";
            } else {
                cout << 'X' << "";
            }
        }
        cout << endl;
    }
}

/* This function prints an animation of the life cycle of the cells. It reads the number of frames
 * from the user and prints that number of "generations" into the console at PAUSE_INTERVAL intervals.*/
void animate(Grid<bool> &grid, int &NROW, int &NCOL) {
    int numFrames;
    numFrames = getInteger("How many frames? ");

    if (cin.fail()) {
        cin.ignore();
        cout << "Illegal integer format. Try again." << endl;
        cout << "How many frames? ";
    }

    for (int i = 0; i <= numFrames; i++) {
        clearConsole();
        grid = updateGeneration(grid, NROW, NCOL);
        printGrid(grid, NROW, NCOL);
        pause(PAUSE_INTERVAL);
    }
}


/* This reads in the file name, creates an empty boolean grid, calls the function to get the grid
 * from the file, and prints the grid.  It takes an input from the user to animate or tick
 * and runs the requested function until the user quits the program. */
int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    cout << HEADER << endl;
    string filename;
    cout << "Enter filename: ";
    getline(cin, filename);
    while (!fileExists(filename)) {
        cout << "File not found. Enter file name: " << endl;
        getline(cin, filename);
    }
    Grid<bool> grid;
    int NCOL = 0;
    int NROW = 0;
    getGrid(filename, grid, NROW, NCOL);
    printGrid(grid, NROW, NCOL);
    string nextstep;

    while (true) {
        cout << "a)nimate, t)ick, q)uit? ";
        getline(cin, nextstep);
        if (nextstep == "t") {
            grid = updateGeneration(grid, NROW, NCOL);
            printGrid(grid, NROW, NCOL);
        } else if (nextstep == "a") {
            animate(grid, NROW, NCOL);
        } else if (nextstep == "q") {
            cout << "Have a nice Life!" << endl;
            break;
        } else {
            cout << "Invalid input. Input 'a', 't', or 'q'." << endl;
        }
    }
    return 0;
}
