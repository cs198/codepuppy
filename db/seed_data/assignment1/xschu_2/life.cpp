//// Xinsheng Chu
//// This program models the life cycle of bacteria using a two-dimensional grid of cell.

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

// This method is called to display all the messages in the console as shown in the demo.
void displayMessage() {
    cout << "Welcome to the CS 106B Game of Life" << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable" << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << "" << endl;
}

// This medthod converts the initial patten in the txt file into a two-dimensional grid.
// It accepts an ifstream input as reference, and return the grid built within the method.
Grid<string> buildGrid(ifstream& input) {
    string line;
    getline(input, line);
    int rowNum = stringToInteger(line);
    getline(input, line);
    int colNum = stringToInteger(line);
    Grid<string> lifeData;
    lifeData.resize(rowNum, colNum);
    int currentRow = 0;
    while (getline(input, line) && currentRow < rowNum) {
        for (int currentCol = 0; currentCol < colNum; currentCol++) {
            lifeData[currentRow][currentCol] = line.at(currentCol);
        }
        currentRow++;
    }
    return lifeData;
}

// This method draws the grid to console.
// It adds each character in one row of the grid to a string, and draws the grid by row by row.
void displayGrid(Grid<string>& grid) {
    for (int i = 0; i < grid.numRows(); i++) {
        string singleLine = "";
        for (int j = 0; j < grid.numCols(); j++) {
            singleLine += grid[i][j];
        }
        cout << singleLine << endl;
    }
}

// This method checks the status of the bacteria of the current generation and decides the status of the bacteria of the next generation, based on the number of neighbors.
// It accepts the status of the bacteria at the current generation, and the number of neighbors of that location.
// It returns the status the bacteria at the next generation.
string updateLocation (string currentGen, int count) {
    string nextGen;
    if (count == 0 || count == 1) {
        nextGen = "-";
    } else if (count == 2) {
        nextGen = currentGen;
    } else if (count == 3) {
        nextGen = "X";
    } else {
        nextGen = "-";
    }
    return nextGen;
}

// This method counts the number of neighbors of one location.
// It accepts the grid by reference, and row number x, column number y of the location, and total number of rows and columns of the grid by value.
// It returns the number of neighbors as integer.
// It first checks the first row of the grid, and if the location is the first or last location in the row.
// If yes, it only check three surrounding locations, if not, it checks five surrounding locations within the boundry.
// Similarily, it checks the last row the grid.
// Then it checks if the location is in the first or the last column, if yes, it checks five surrounding locations.
// If the location does not lie on the boundry, the method checks all the eight surrounding locations.
int checkNeighbor(Grid<string>& grid, int x, int y, int nRows, int nCols) {
    int count = 0;
    if (x == 0) {
        if (y == 0) {
            if (grid[x][y+1] == "X") count++;
            if (grid[x+1][y+1] == "X") count++;
            if (grid[x+1][y] == "X") count++;
        } else if (y == nCols-1) {
            if (grid[x+1][y] == "X") count++;
            if (grid[x+1][y-1] == "X") count++;
            if (grid[x][y-1] == "X") count++;
        } else {
            if (grid[x][y+1] == "X") count++;
            if (grid[x+1][y+1] == "X") count++;
            if (grid[x+1][y] == "X") count++;
            if (grid[x+1][y-1] == "X") count++;
            if (grid[x][y-1] == "X")  count++;
        }
    } else if (x == nRows-1) {
        if (y == 0) {
            if (grid[x-1][y] == "X") count++;
            if (grid[x-1][y+1] == "X") count++;
            if (grid[x][y+1] == "X") count++;
        } else if (y == nCols-1) {
            if (grid[x][y-1] == "X") count++;
            if (grid[x-1][y-1] == "X") count++;
            if (grid[x-1][y] == "X") count++;
        } else {
            if (grid[x][y-1] == "X") count++;
            if (grid[x-1][y-1] == "X") count++;
            if (grid[x-1][y] == "X") count++;
            if (grid[x-1][y+1] == "X") count++;
            if (grid[x][y+1] == "X") count++;
        }
    } else if (y == 0) {
        if (grid[x-1][y] == "X") count++;
        if (grid[x-1][y+1] == "X") count++;
        if (grid[x][y+1] == "X") count++;
        if (grid[x+1][y+1] == "X") count++;
        if (grid[x+1][y] == "X") count++;
    } else if (y == nCols-1) {
        if (grid[x+1][y] == "X") count++;
        if (grid[x+1][y-1] == "X") count++;
        if (grid[x][y-1] == "X") count++;
        if (grid[x-1][y-1] == "X") count++;
        if (grid[x-1][y] == "X") count++;
    } else {
        if (grid[x+1][y] == "X") count++;
        if (grid[x+1][y+1] == "X") count++;
        if (grid[x][y+1] == "X") count++;
        if (grid[x-1][y+1] == "X") count++;
        if (grid[x-1][y] == "X") count++;
        if (grid[x-1][y-1] == "X") count++;
        if (grid[x][y-1] == "X") count++;
        if (grid[x+1][y-1] == "X") count++;
    }
    return count;
}

// This method generates the new grid of the next generation.
// It accepts the grid of current genration as reference, and returns that of next generation.
// This method calls the checkNeighbor and updateLocation methods by iterating through rows and columns of the current grid.
Grid<string> nextGen(Grid<string>& grid) {
    Grid<string> newGrid = grid;
    int nRows = grid.numRows();
    int nCols = grid.numCols();
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            int neighborCount = checkNeighbor(grid, i, j, nRows, nCols);
            newGrid[i][j] = updateLocation(grid[i][j], neighborCount);
        }
    }
    return newGrid;
}

// This method asked the user if the next step is animate, tick or quit.
// If user selects action, it further asks user number of frames he wants to animate and perform the animation in the for loop.
// If user selects tick, it perform one step and check with user about next step again.
// It user selects quit, it prints out a message.
// It accepts the grid passing by reference.
void nextAction(Grid<string>& grid) {
    string action = getLine("a)nimate, t)ick, q)uit? ");
    while (true) {
        if (action == "a") {
            int frame = getInteger("How many frames? ");
            for (int i = 0; i < frame; i++) {
                clearConsole();
                grid = nextGen(grid);
                displayGrid(grid);
                pause(50);
            }
        action = getLine("a)nimate, t)ick, q)uit? ");
        } else if (action == "t") {
            grid = nextGen(grid);
            displayGrid(grid);
            action = getLine("a)nimate, t)ick, q)uit? ");
        } else if (action == "q") {
            break;
        } else {
            cout << "Invalid choice: please try again." << endl;
            action = getLine("a)nimate, t)ick, q)uit? ");
        }
    }
}

// This is the main method calling different functions.
// It also asks user to input the name of the txt file to open.
// If the name is invalid, it re-prompts user to input the name.
int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    displayMessage();

    ifstream input;
    string fileName = getLine("Grid input file name? ");
    while (!fileExists(fileName)) {
        cout << "File not found; please try again." << endl;
        fileName = getLine("Grid input file name?");
    }
    openFile(input, fileName);

    Grid<string> lifeGrid;
    lifeGrid = buildGrid(input);
    displayGrid(lifeGrid);
    nextAction(lifeGrid);

    input.close();
    cout << "Have a nice Life!" << endl;
    return 0;
}




