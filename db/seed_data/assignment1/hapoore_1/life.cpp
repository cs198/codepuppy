/* Name: Austin Poore
 *
 * This program simulates the life cycle of a colony of bacteria based on
 * a fixed set of rules. The user is prompted to enter a starter file which
 * represents the initial state of the colony. Then, once a valid file has been
 * imported, the user is prompted to advance the colony through one or more
 * generations before quitting. The life cycle of the colony is displayed on
 * an animated graphics window.
 *
 * For this project, I used the Stanford library documentation and slides from the class website.
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


void printRules();
void getFile(string& entry);
void storeData(ifstream& input, Grid<bool>& grid, LifeGUI& gui);
void printGrid(Grid<bool>& grid, LifeGUI& gui);
void advanceOneGeneration(Grid<bool>& grid, LifeGUI& gui);
int countNeighbors(Grid<bool>& grid, int row, int column);
void countAbove(Grid<bool>& grid, int& counter, int row, int column);
void countBelow(Grid<bool>& grid, int& counter, int row, int column);
void countSides(Grid<bool>& grid, int& counter, int row, int column);
void updateColony(Grid<bool>& grid, LifeGUI& gui);

/*
 * This method initializes the Grid used to store the data, the ifstream to
 * read in the starter file, and the LifeGUI graphical interface. It then runs
 * the program.
 */

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    Grid <bool> bacteria;
    ifstream input;
    LifeGUI animation;
    printRules();
    string fileName;
    getFile(fileName);
    openFile(input, fileName);
    storeData(input, bacteria, animation);
    input.close();
    printGrid(bacteria, animation);
    updateColony(bacteria, animation);
    cout << "Have a nice Life!" << endl;
    return 0;
}

/*
 * This method prints the introductory lines of text at the start of the program.
 */

void printRules() {
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies."<< endl;
    cout << "" << endl;
}

/*
 * This method takes in the name of a starter file from the user. If the file
 * does not exist, the user is prompted to enter another file.
 */

void getFile(string& entry) {
    while (true) {
        entry = getLine("Grid input file name? ");
        if (fileExists(entry)) {
            break;
        }
        cout << "File not found; please try again." << endl;
    }
}

/*
 * This method reads the file entered by the user and stores its data in the
 * grid.
 */

void storeData(ifstream& input, Grid<bool>& grid, LifeGUI& gui) {
    string line;
    getline(input, line); //Read the first two lines to get the size
    int rows = stringToInteger(line); //and determine how many more lines to read.
    getline(input, line);
    int cols = stringToInteger(line);
    grid.resize(rows, cols);
    gui.resize(rows, cols);
    for (int i = 0; i < rows; i++) { //Read and store the rest of the lines.
        getline(input, line);
        for (int j = 0; j < cols; j++) {
            if (line[j] == 'X') {
                grid.set(i, j, true);
            } else {
                grid.set(i, j, false);
            }
        }
    }
}

/*
 * This method displays the current grid on the console (row by row) and the GUI.
 */

void printGrid(Grid<bool>& grid, LifeGUI& gui) {
    for (int i = 0; i < grid.numRows(); i++) {
        string row;
        for (int j = 0; j < grid.numCols(); j++) { //Iterate through the grid and reproduce it on console/GUI.
            if (grid[i][j]) {
                row += 'X';
                gui.drawCell(i, j, true);
            } else {
                row += '-';
                gui.drawCell(i, j, false);
            }
        }
        cout << row << endl;
    }
}

/*
 * This method moves the grid forward one generation in the grid and GUI
 * according to the rules which govern the bacteria. It uses a temporary grid
 * to store the data while the original grid is updated.
 */

void advanceOneGeneration(Grid<bool>& grid, LifeGUI& gui) {
    Grid<bool> tempGrid = grid;
    for (int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            int neighbors = countNeighbors(grid, i, j);
            if (neighbors == 2) {
                tempGrid[i][j] = grid[i][j];
            } else if (neighbors == 3) {
                tempGrid[i][j] = true;
            } else {
                tempGrid [i][j] = false;
            }
        }
    }
    grid = tempGrid;
    printGrid(grid, gui);
}

/*
 * This method counts the number of neighboring bacteria surrounding each space
 * on the grid by breaking the surrounding cells into above, below, and to the side.
 */

int countNeighbors(Grid<bool>& grid, int row, int column) {
    int counter = 0;
    countAbove(grid, counter, row, column);
    countBelow(grid, counter, row, column);
    countSides(grid, counter, row, column);
    return counter;
}


/*
 * This method counts the number of neighboring bacteria in the row of cells
 * above the given space on the grid. It first makes sure the indices are in bounds
 * to deal with edge cases.
 */

void countAbove(Grid<bool>& grid, int &counter, int row, int column) {
    for (int i = 0; i < 3; i++) {
        if (grid.inBounds(row - 1, column - 1 + i)) {
            if (grid[row - 1][column - 1 + i] == true) {
                counter++;
            }
        }
    }
}

/*
 * This method counts the number of neighboring bacteria in the row of cells
 * below the given space on the grid. It first makes sure the indices are in bounds
 * to deal with edge cases.
 */

void countBelow(Grid<bool>& grid, int& counter, int row, int column) {
    for (int i = 0; i < 3; i++) {
        if (grid.inBounds(row + 1, column - 1 + i)) {
            if (grid[row + 1][column - 1 + i] == true) {
                counter++;
            }
        }
    }
}

/*
 * This method counts the number of neighboring bacteria in the row of cells
 * to the left and right of the given space on the grid. It first makes sure
 * the indices are in bounds to deal with edge cases.
 */

void countSides(Grid<bool>& grid, int& counter, int row, int column) {
    for (int i = 0; i < 2; i++) {
        if (grid.inBounds(row, column - 1 + 2*i)) {
            if (grid[row][column - 1 + 2*i] == true) {
                counter++;
            }
        }
    }
}

/*
 * This method prompts the user to enter a choice as to how the program should
 * proceed. If the input is invalid (not 'a', 't', or 'q'), the user is prompted
 * to enter another choice. If the user selects 't', the colony advances one
 * generation. If the user selects 'a', he/she is then prompted to enter an
 * integer for the number of frames he/she would like to animate. If the user
 * enters 'q', the program finishes and the method ends.
 */

void updateColony(Grid<bool>& grid, LifeGUI& gui) {
    while (true) {
        string userPrompt = getLine("a)nimate, t)ick, q)uit? ");
        if (userPrompt == "q") {
            break;
        } else if (userPrompt == "t") {
            advanceOneGeneration(grid, gui);
        } else if (userPrompt == "a") {
            int frames = getInteger("How many frames? ");
            for(int i = 0; i < frames; i++) {
                clearConsole();
                advanceOneGeneration(grid, gui);
                pause(100);
            }
        } else {
            cout << "Invalid choice; please try again." << endl;
        }
    }
}

