/* Program: The Game of Life
 * Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * ------------------------------
 * This program allows the user to interact with the Game Of Life.
 * This game models the life cycle of bacteria using 2D cells. Given
 * a starting formation, as determined by a text file that the user enters,
 * the game simulates the profression of the bacteria over generations using
 * a specific set of rules that can be found at the top of the console window when
 * the program is run.
*/

#include <cctype>
#include <cstring>
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

//Constant definitions
const int PAUSE_TIME = 250;
const int CELL_PRESENT = 1;
const int NO_CELL_PRESENT = 0;

//Function prototypes
void displayInitialMessages();
void createMatrix(int &nRows, int &nColumns, Grid<int> &initialMatrix);
void displayGrid(Grid<int> &matrix);
void playGame (Grid<int> &initialMatrix);
Grid<int> updateGrid (Grid<int> &initialMatrix);
void shiftLeft (Grid<int> &initialMatrix, Grid<int> &newMatrix);
void shiftRight (Grid<int> &initialMatrix, Grid<int> &newMatrix);
void shiftUp (Grid<int> &initialMatrix, Grid<int> &newMatrix);
void shiftDown (Grid<int> &initialMatrix, Grid<int> &newMatrix);
void sumMatrices (Grid<int> &rightShiftedMatrix,Grid<int> &leftShiftedMatrix,Grid<int> &upShiftedMatrix,
                  Grid<int> &downShiftedMatrix,Grid<int> &upRightShiftedMatrix, Grid<int> &upLeftShiftedMatrix,
                  Grid<int> &downLeftShiftedMatrix, Grid<int> &downRightShiftedMatrix, Grid<int> &result);

/* Runs the Game of Life program */
int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    displayInitialMessages();
    int nRows, nColumns;
    Grid<int> initialMatrix;
    createMatrix(nRows,nColumns,initialMatrix);
    displayGrid(initialMatrix);
    playGame(initialMatrix);
    cout << "Have a nice Life!" << endl;
    return 0;
}

/* Displays the messages that should run at the start of the program. */
void displayInitialMessages() {
    cout << "Welcome to the CS106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules: " << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl << endl;
}

/* Reads the string inputted by the user and attempts to find a file with
 * that name. If none exists, then it tells the user that it is not valid
 * and asks for a different file name. */
void createMatrix(int &nRows, int &nColumns, Grid<int> &initialMatrix) {
    while (true) {
        try {
            ifstream input;
            string file = getLine ("Grid input file name? ");
            input.open(file.c_str());
            string nRowString;
            getline(input,nRowString);
            nRows = stringToInteger(nRowString);
            string nColString;
            getline(input,nColString);
            nColumns = stringToInteger(nColString);
            initialMatrix.resize(nRows,nColumns);
            for (int i = 0; i < nRows ; i++) {
                string line;
                getline(input,line);
                for (int j = 0 ; j < nColumns ; j++) {
                    int value;
                    if (line[j] == '-') {
                        value = NO_CELL_PRESENT;
                    }
                    if (line[j] == 'X'){
                        value = CELL_PRESENT;
                    }
                    initialMatrix.set(i,j,value);
                }
            }
            break;
        } catch (exception e) {
            cerr << e.what() << endl;
            cout << "That is not a valid file. Try again." << endl;
        }
    }
}

/* Interprets the grid that contains the information about which locations
 * have living cells and prints it out on the display using '-' and 'X'. */
void displayGrid(Grid<int> &matrix) {
    for (int r = 0 ; r < matrix.numRows() ; r++) {
        string line = "";
        for (int c = 0 ; c < matrix.numCols() ; c++) {
            int value = matrix.get(r,c);
            if (value == NO_CELL_PRESENT) {
                line += '-';
            } else if ( value == CELL_PRESENT) {
                line += 'X';
            }
        }
        cout << line << endl;
    }
}

/* Allows the user to tell the program to either quit, tick, or animate
 * through different generations of the Game of Life. If the user enters
 * someting other than "a", "t", or "q" then the program prompts them to
 * enter a different response. Quitting causes this method to end, ticking causes
 * the display to update one generation, and animate causes the display to clear
 * and update for a set amount of generations depending on what the user inputs. */
void playGame (Grid<int> &initialMatrix) {
    while (true) {
        string response;
        while (true) {
            response = getLine("a)nimate, t)ick, q)uit? ");
            if (response == "a" || response == "t" || response == "q") {
                break;
            } else {
                cout << "That is not a valid response. Try again." << endl;
            }
        }
        if (response == "q") {
            return;
        }
        if (response == "t") {
            initialMatrix = updateGrid(initialMatrix);
            displayGrid(initialMatrix);
        }
        if (response == "a") {
            int frames = getInteger("How many frames? ");
            for (int i = 0 ; i < frames ; i++) {
                clearConsole();
                initialMatrix = updateGrid(initialMatrix);
                displayGrid(initialMatrix);
                pause(PAUSE_TIME);
            }
        }
    }
}

/* Updates the Game Of Life grid by shifting the grid up, down, right, left, up-right,
 * up-left, down-right, and down-left and summing the number of cells that are alive
 * at each of those locations. This gives a grid that contains an integer value for the
 * number of neighbors that location has. This grid of neighbors is used to update the grid. */
Grid<int> updateGrid (Grid<int> &initialMatrix) {
    Grid<int> result;
    result.resize(initialMatrix.numRows(), initialMatrix.numCols());
    Grid<int> rightShiftedMatrix;
    shiftRight(initialMatrix,rightShiftedMatrix);
    Grid<int> leftShiftedMatrix;
    shiftLeft(initialMatrix,leftShiftedMatrix);
    Grid<int> upShiftedMatrix;
    shiftUp(initialMatrix, upShiftedMatrix);
    Grid<int> downShiftedMatrix;
    shiftDown(initialMatrix, downShiftedMatrix);
    Grid<int> upRightShiftedMatrix;
    shiftRight(upShiftedMatrix, upRightShiftedMatrix);
    Grid<int> upLeftShiftedMatrix;
    shiftLeft(upShiftedMatrix, upLeftShiftedMatrix);
    Grid<int> downLeftShiftedMatrix;
    shiftLeft(downShiftedMatrix, downLeftShiftedMatrix);
    Grid<int> downRightShiftedMatrix;
    shiftRight(downShiftedMatrix, downRightShiftedMatrix);
    Grid<int> neighborCount;
    sumMatrices(rightShiftedMatrix, leftShiftedMatrix, upShiftedMatrix, downShiftedMatrix, upRightShiftedMatrix,
                upLeftShiftedMatrix, downLeftShiftedMatrix, downRightShiftedMatrix, neighborCount);
    for (int r = 0 ; r < initialMatrix.numRows() ; r++) {
        for (int c = 0 ; c < initialMatrix.numCols() ; c++) {
            int nNeighbors = neighborCount.get(r,c);
            int value = initialMatrix.get(r,c);
            if (value == CELL_PRESENT) {
               if (nNeighbors <= 1 || nNeighbors >= 4) {
                   value = NO_CELL_PRESENT;
               }
            } else {
                if (nNeighbors == 3) {
                    value = CELL_PRESENT;
                }
            }
            result.set(r,c,value);
        }
    }
    return result;
}

/* Shifts the current grid to the right and saves it in the value newMatrix. */
void shiftRight (Grid<int> &initialMatrix, Grid<int> &newMatrix) {
    newMatrix.resize(initialMatrix.numRows(), initialMatrix.numCols());
    for (int r = 0 ; r < initialMatrix.numRows() ; r++) {
        for (int c = 0 ; c < initialMatrix.numCols() - 1 ; c++) {
            newMatrix.set(r,c+1, initialMatrix.get(r,c));
        }
    }
}

/* Shifts the current grid to the left and saves it in the value newMatrix. */
void shiftLeft (Grid<int> &initialMatrix, Grid<int> &newMatrix) {
    newMatrix.resize(initialMatrix.numRows(), initialMatrix.numCols());
    for (int r = 0 ; r < initialMatrix.numRows() ; r++) {
        for (int c = 1 ; c < initialMatrix.numCols() ; c++) {
            newMatrix.set(r,c-1, initialMatrix.get(r,c));
        }
    }
}

/* Shifts the current grid up and saves it in the value newMatrix. */
void shiftUp (Grid<int> &initialMatrix, Grid<int> &newMatrix) {
    newMatrix.resize(initialMatrix.numRows(), initialMatrix.numCols());
    for (int r = 1 ; r < initialMatrix.numRows() ; r++) {
        for (int c = 0 ; c < initialMatrix.numCols() ; c++) {
            newMatrix.set(r-1,c, initialMatrix.get(r,c));
        }
    }
}

/* Shifts the current grid down and saves it int he value newMatrix */
void shiftDown (Grid<int> &initialMatrix, Grid<int> &newMatrix) {
    newMatrix.resize(initialMatrix.numRows(), initialMatrix.numCols());
    for (int r = 0 ; r < initialMatrix.numRows() - 1 ; r++) {
        for (int c = 0 ; c < initialMatrix.numCols() - 1 ; c++) {
            newMatrix.set(r+1,c, initialMatrix.get(r,c));
        }
    }
}

/* Adds together all of the different shifted grids to create a final grid that has an integer value
 * for the number of neighbors at each location. */
void sumMatrices (Grid<int> &rightShiftedMatrix,Grid<int> &leftShiftedMatrix,Grid<int> &upShiftedMatrix,
                  Grid<int> &downShiftedMatrix,Grid<int> &upRightShiftedMatrix, Grid<int> &upLeftShiftedMatrix,
                  Grid<int> &downLeftShiftedMatrix, Grid<int> &downRightShiftedMatrix, Grid<int> &result) {
    int nRows = rightShiftedMatrix.numRows();
    int nCols = rightShiftedMatrix.numCols();
    result.resize(nRows, nCols);
    for (int r = 0 ; r < nRows ; r++) {
        for (int c = 0 ; c < nCols ; c++) {
            int value = rightShiftedMatrix.get(r,c) + leftShiftedMatrix.get(r,c) + upShiftedMatrix.get(r,c) +
                    downShiftedMatrix.get(r,c) + upRightShiftedMatrix.get(r,c) + upLeftShiftedMatrix.get(r,c) +
                    downLeftShiftedMatrix.get(r,c) + downRightShiftedMatrix.get(r,c);
            result.set(r,c,value);
        }
    }
}
