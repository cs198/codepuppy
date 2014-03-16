/* Name: Lindsey Txakeeyang
 * Course: CS106B, Friday 1:15 Section
 * Assignment: The Game of Life.
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
using namespace std;

void intro();
void newGeneration(Grid<bool>&);
int checkNeighbors(Grid<bool>&, int, int);
void printGrid(Grid<bool>&);
void animate(Grid<bool>&);
void tick(Grid<bool>&);
void quit();
void choices(Grid<bool>&);
//void initialGrid(Grid<bool>& lifeGrid, ifstream input);

int main() {
    setConsoleSize(700, 400);
    intro();
    cout << endl;
    Grid<bool> lifeGrid; // constructs the basic grid being used
    string filename;
    cout << "Grid input file name? ";
    cin >> filename;
    ifstream input;
    input.open(filename.c_str());
    int num1;
    int num2;
    string line;
    for (int n = 2; n > 0; n--) {
        getline(input, line);
        istringstream tokens(line);
        if (n==2) tokens >> num1;
        if (n==1) tokens >> num2;
    }
    lifeGrid.resize(num1,num2);
    for (int r = 0; r < lifeGrid.numRows(); r++) {
        getline(input,line);
        for (int c = 0; c < lifeGrid.numCols(); c++) {
            if (line[c] == '-') {
                lifeGrid.set(r, c, false);
            }
            if (line[c] != '-') {
                lifeGrid.set(r, c, true);
            }
        }
    }
    printGrid(lifeGrid);
    choices(lifeGrid);
    cout << "Have a nice Life!";
    return 0;
}

/*
 * Prints a welcome message explaining the program
 */
void intro() {
    cout << "Welcome to the CS106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
}

/* Tried creating a function to make main look simpler but it wouldn't work
 * for some reason as a function. It kept saying that I was attemping to use
 * a deleted function??
 void initialGrid(Grid<bool>& lifeGrid, ifstream input) {
    int num1;
    int num2;
    string line;
    for (int n = 2; n > 0; n--) {
        getline(input, line);
        istringstream tokens(line);
        if (n==2) tokens >> num1;
        if (n==1) tokens >> num2;
    }
    lifeGrid.resize(num1,num2);
    for (int r = 0; r < lifeGrid.numRows(); r++) {
        getline(input,line);
        for (int c = 0; c < lifeGrid.numCols(); c++) {
            if (line[c] == '-') {
                lifeGrid.set(r, c, false);
            }
            if (line[c] != '-') {
                lifeGrid.set(r, c, true);
            }
        }
    }
}*/

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
            if (lifeGrid.inBounds(x, y) && lifeGrid[x][y] == true && (x != r || y != c)) counter++;
        }
    }
    return counter;
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
void choices(Grid<bool>& lifeGrid) {
    string answer;
    while (true) {
        cout << "a)nimate, t)ick, q)uit? ";
        cin >> answer;
        if (answer == "q") {
            break;
        }
        if (answer == "a") animate(lifeGrid);
        if (answer == "t") tick(lifeGrid);
    }
    cout << "Have a nice Life!" << endl;
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

