//// This document simulates Conway's Game of Life.
/// When prompted, enter the starting file name and watch your Life colony evolve!
/// Coded by: Helen Xiong, CS106B Winter 2014
/// Sources cited: lecture programs by Dr. Stepp
/// Extension 1: wraparound
/// Extension 2: GUI
/// Extension 3: random world

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
#include "random.h"
using namespace std;

/*Reads and stores the lines of the text file into the Life Grid.*/
void lineToGrid(int row, string & line, Grid<char> & Life) {
    for (int col = 0; col < Life.numCols(); col++) {
        char ch = line[col];
        if (ch == '-') Life.set(row, col, '-');
        if (ch == 'X') Life.set(row, col, 'X');
    }
}

/*Generates a random world.
 * Uses a random number generator (from 3 - 100) to generate world dimensions.
 * Uses a random boolean generator (true/false) to generate '-' and 'X'. */
void random(Grid<char> & Life) {
    int nRows = randomInteger(3, 200);
    int nCols = randomInteger(3, 200);
    Life.resize(nRows, nCols);
    for (int row = 0; row < Life.numRows(); row++) {
        for (int col = 0; col < Life.numCols(); col++) {
            bool isAlive = randomChance(0.5);
            if (isAlive) {
                Life.set(row, col, 'X');
            } else {
                Life.set(row, col, '-');
            }
        }
    }
}

/*Displays an input grid, the Life Grid, and displays it on the GUI*/
void displayGrid(Grid<char> & Life, LifeGUI & gui) {
    clearConsole();
    gui.resize(Life.numRows(), Life.numCols());
    for (int row = 0; row < Life.numRows(); row++) {
        string rowchars = "";
        for (int col = 0; col < Life.numCols(); col++) {
            if (Life.get(row, col) == 'X') {
                gui.drawCell(row, col, true);
            } else {
                gui.drawCell(row, col, false);
            }
            rowchars += Life.get(row, col);
        }
        cout << rowchars << endl;
    }
}

/* Reads the text file into a grid that stores the cell values.
 * Resizes, and "fills in" the grid using "X" for alive cells,
 * and "-" for dead cells.
 * The function assumes that the first two lines of the text file
 * have integers that represent the number of rows and columns in
 * the Life gameboard respectively. The next lines consist of the
 * contents of the board. Any comments at the end of the file are
 * prefaced by "#" or another non-"X" or "-" character.*/
void initializeGrid(Grid<char> & Life, ifstream & input) {
    int nRows;
    int nCols;
    string line;
    int count = 0;
    while (getline(input, line)) { //starts text extraction from file
        if (count > 1 && line[0] != 'X' && line[0] != '-') break; //error checking for non-grid material here
        switch (count) {
        case (0):
            nRows = stringToInteger(line);
            break;
        case (1) :
            nCols = stringToInteger(line);
            Life.resize(nRows, nCols);
            break;
        default:
            lineToGrid(count - 2, line, Life);
        }
        count++;
    }
}

/*Calculates the new grid positions and
 * updates the Life grid based on the rules of the game:
 *If 4 surrounding cells are alive (i.e. value of grid denoted with 'X', the cell dies of overcrowding.
 * If 3 surrounding cells are alive, the cell generates life.
 * If 2 cells are alive, the cell is stable.
 * If 1 or fewer cells are alive, the cell dies of loneliness.
 * The program tallies all the "alive" cells that surround the cell in question.
 *Special checking is used in the case of corners and edges.
 * For corners, the surrounding three cells are checked.
 * With the "wraparound extension", the program checks the two cells at the opposite end of the board
 * that would be adjacent to the corner cell if the board wrapped around all teh way.
 * For edges, the program checks adjacent cells on both sides of the cell in question.*/
void checkGrid(Grid<char> & Life) {
    int nRows = Life.numRows();
    int nCols = Life.numCols();
    Grid<char> temp (nRows, nCols);
    temp = Life;
    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
            int count = 0;
            //corners
            if (row == 0 && col == 0) { //upper left
                if (temp.get(row, col + 1) == 'X') count++;
                if (temp.get(row + 1, col + 1) == 'X') count++;
                if (temp.get(row + 1, col) == 'X') count++;
                if (temp.get(nRows - 1, nCols - 2) == 'X') count++;
                if (temp.get(nRows - 1, nCols - 1) == 'X') count++;
            }
            else if (row == 0 && col == nCols - 1) { //upper right
                if (temp.get(row, col - 1) == 'X') count++;
                if (temp.get(row + 1, col - 1) == 'X') count++;
                if (temp.get(row + 1, col) == 'X') count++;
                if (temp.get(nRows - 1, 0) == 'X') count++;
                if (temp.get(nRows - 1, 1) == 'X') count++;
            }
            else if (row == nRows - 1 && col == 0) { //lower left
                if (temp.get(row, col + 1) == 'X') count++;
                if (temp.get(row - 1, col + 1) == 'X') count++;
                if (temp.get(row - 1, col) == 'X') count++;
                if (temp.get(0, nCols - 2) == 'X') count++;
                if (temp.get(0, nCols - 1) == 'X') count++;
            }
            else if (row == nRows - 1 && col == nCols - 1) { //lower right
                if (temp.get(row, col - 1) == 'X') count++;
                if (temp.get(row - 1, col - 1) == 'X') count++;
                if (temp.get(row - 1, col) == 'X') count++;
                if (temp.get(0, 0) == 'X') count++;
                if (temp.get(0, 1) == 'X') count++;
            }
            //edges
            else if (row == 0) { //upper edge
                if (temp.get(nRows - 1, col - 1) == 'X') count++;
                if (temp.get(row, col - 1) == 'X') count++;
                if (temp.get(row + 1, col - 1) == 'X') count++;
                if (temp.get(nRows - 1, col) == 'X') count++;
                if (temp.get(row + 1, col) == 'X') count++;
                if (temp.get(nRows - 1, col + 1) == 'X') count++;
                if (temp.get(row, col + 1) == 'X') count++;
                if (temp.get(row + 1, col + 1) == 'X') count++;

            }
            else if (row == nRows - 1) { //lower edge
                if (temp.get(row - 1, col - 1) == 'X') count++;
                if (temp.get(row, col - 1) == 'X') count++;
                if (temp.get(0, col - 1) == 'X') count++;
                if (temp.get(row - 1, col) == 'X') count++;
                if (temp.get(0, col) == 'X') count++;
                if (temp.get(row - 1, col + 1) == 'X') count++;
                if (temp.get(row, col + 1) == 'X') count++;
                if (temp.get(0, col + 1) == 'X') count++;
            }
            else if (col == 0) { //left edge
                if (temp.get(row - 1, nCols - 1) == 'X') count++;
                if (temp.get(row, nCols - 1) == 'X') count++;
                if (temp.get(row + 1, nCols - 1) == 'X') count++;
                if (temp.get(row - 1, col) == 'X') count++;
                // row, col
                if (temp.get(row + 1, col) == 'X') count++;
                if (temp.get(row - 1, col + 1) == 'X') count++;
                if (temp.get(row, col + 1) == 'X') count++;
                if (temp.get(row + 1, col + 1) == 'X') count++;
            }
            else if (col == nCols - 1) { // right edge
                if (temp.get(row - 1, col - 1) == 'X') count++;
                if (temp.get(row, col - 1) == 'X') count++;
                if (temp.get(row + 1, col - 1) == 'X') count++;
                if (temp.get(row - 1, col) == 'X') count++;
                if (temp.get(row + 1, col) == 'X') count++;
                if (temp.get(row - 1, 0) == 'X') count++;
                if (temp.get(row, 0) == 'X') count++;
                if (temp.get(row + 1, 0) == 'X') count++;
            } else { //all other cells
                if (temp.get(row - 1, col - 1) == 'X') count++;
                if (temp.get(row, col - 1) == 'X') count++;
                if (temp.get(row + 1, col - 1) == 'X') count++;
                if (temp.get(row - 1, col) == 'X') count++;
                if (temp.get(row + 1, col) == 'X') count++;
                if (temp.get(row - 1, col + 1) == 'X') count++;
                if (temp.get(row, col + 1) == 'X') count++;
                if (temp.get(row + 1, col + 1) == 'X') count++;
            }
            switch (count) { //Check the count:
            case (3): //generates life
                Life.set(row, col, 'X');
                break;
            case (2): break; //stable
            case (1): case (0): //cell dies
                Life.set(row, col, '-');
                break;
            default: //count >= 4 : cell dies of overcrowding
                Life.set(row, col, '-');
                break;
            }
        }
    }
}

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
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << "Enter \"random\" to generate a random world." << endl;
    cout << "" << endl;

    LifeGUI gui;
    ifstream input;
    Grid<char> Life;
    string textfile = getLine("Grid input file name? ");
    if (textfile == "random") {
        random(Life);
    } else {
        input.open(textfile.c_str());
        while (input.fail()) {
            cout << "File not found, please try again." << endl;
            textfile = getLine("Grid input file name? ");
            input.open(textfile.c_str());
        }
        initializeGrid(Life, input);
    }
    displayGrid(Life, gui);
    input.close();

    while (true) { //user enters the next step
        string answer = getLine("a)nimate, t)ick, q)uit? ");
        while (answer != "a" && answer != "t" && answer != "q") {
            answer = getLine("Invalid answer. a)nimate, t)ick, q)uit? ");
        }
        if (answer == "a") {
            int nTicks = getInteger("How many frames? ");
            for (int i = 0; i < nTicks; i++) {
                checkGrid(Life);
                displayGrid(Life, gui);
                pause(50);
            }
        } else if (answer == "t") {
            checkGrid(Life);
            displayGrid(Life, gui);
        } else if (answer == "q") break;
    }
    cout << "Have a nice Life!" << endl;
    return 0;
}
