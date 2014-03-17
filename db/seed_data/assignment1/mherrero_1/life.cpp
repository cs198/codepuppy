//// Life.cpp - Matthew Herrero - 1/15/14
/// --------------------------------------------
/// This program runs Conway's Game of Life. It begins by welcoming the user to the program, describing
/// the rules of the "game" to the user (which can be found in the method "printIntro()"), and asking
/// the user to input a filename. Once the user inputs a valid filename, the program finds this file and
/// uses it to generate a char grid of appropriate size, filled with the "char environment" contents of
/// the file. Then, the program asks the user if it would like to "tick" the environment one frame forward,
/// "animate" the environment several frames forward, or "quit" the program. After the user has input a valid
/// command, the program executes the command, progressing the environment based on the logic rules laid
/// out in the assignment packet. If the tick command is called, the program prints the next frame of the
/// environment, then reprompts the user for the next command. If the animate command is triggered, then the
/// program asks the user how many frames they would like to see animated. Once the user has input a valid
/// number of frames, the programs animates these many frames in rapid succession (displaying the frame,
/// pausing for 50 ms, then clearing the console and printing the next frame) before asking the user for the
/// next command.
///

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

/*
 * These are the prototypes for the methods used in this program. They are indented based on the
 * methods in which they first appear.
 */
void printIntro();
void generateGrid(Grid<char>& grid);
    string retrieveFilename();
    void formatGrid(Grid<char>& grid, ifstream& input);
    void fillGrid(Grid<char>& grid, ifstream& input);
void printGrid(Grid<char>& grid);
char getNextOperation();
void printNextFrame(Grid<char>& grid);
    void progressLife(Grid<char>& grid);
        int getNeighborCount(Grid<char>& grid, int rowInd, int colInd);
            int cornerNeighbors(Grid<char>& grid, int rowInd, int colInd);
            int sideNeighbors(Grid<char>& grid, int rowInd, int colInd);
                bool cellAt(Grid<char>& grid, int row, int col);
void animateGrid(Grid<char>& grid);
    int getFrameCount();
    bool isPositiveNumber(string frames);

/*
 * This is the main function. It has the functionality described in the program's header comment.
 */
int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    char operation = ' ';
    Grid<char> grid;

    printIntro();
    generateGrid(grid);
    printGrid(grid);
    while(operation!='q') {
        operation = getNextOperation();
        if(operation == 't') {
            printNextFrame(grid);
        } else if (operation == 'a') {
            animateGrid(grid);
        }
    }
    cout << "Have a nice Life!" << endl;
    return 0;
}

/*
 * This method prints the header/welcoming text for the program.
 */
void printIntro() {
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
 * This method formats the "environment" grid based on the text file that the user wants to
 * use for the program. The program is passed a Grid of chars, and two int variables, one which
 * contains the number of rows in the grid, one for the number of columns. This method prompts the
 * user for a filename to read, opens this file, uses it to format and fill the grid, then closes
 * the file.
 */
void generateGrid(Grid<char>& grid) {
    ifstream input;

    string filename = retrieveFilename();
    input.open(filename.c_str());
    formatGrid(grid, input);
    fillGrid(grid, input);
    input.close();
}

/*
 * This method prompts the user for a filename to read in, then returns the inputed string once
 * a valid filename is entered.
 */
string retrieveFilename() {
    string inputFilename = "";
    while (!fileExists(inputFilename)) {
        cout << "Grid input file name? ";
        getline(cin,inputFilename);
        if (fileExists(inputFilename)) {
            break;
        }
        cout << "File not found; please try again." << endl;
    }
    return inputFilename;
}

/*
 * This method formats the char grid. It is passed the grid and ifstream that is being used to
 * read the file. This method reads the first two lines of the text file uses these values, which
 * are the number of rows and columns, to resize the grid.
 */
void formatGrid(Grid<char>& grid, ifstream& input) {
    string nRowsStr;
    string nColStr;

    getline(input, nRowsStr);
    getline(input, nColStr);
    grid.resize(stringToInteger(nRowsStr), stringToInteger(nColStr));
}

/*
 * This method fills the grid with the chars that are in the file. It iterates through each row,
 * gets the line of chars, then individually places each char in the row in its respective place
 * in the grid. It is passed the char grid and the ifstream reading the text file.
 */
void fillGrid(Grid<char>& grid, ifstream& input) {
    for(int rowInd = 0; rowInd < grid.numRows(); rowInd++) {
        string rowString;
        getline(input, rowString);
        for(int colInd = 0; colInd < grid.numCols(); colInd++) {
            grid[rowInd][colInd] = rowString[colInd];
        }
    }
}

/*
 * This method prints out the grid that it is passed. It iterates through each row and prints each
 * char in each row. Once it reaches the end of a row, it goes to the next line and begins printing
 * that row.
 */
void printGrid(Grid<char>& grid) {
    for(int rowInd = 0; rowInd < grid.numRows(); rowInd++) {
        for(int colInd = 0; colInd < grid.numCols(); colInd++) {
            cout << grid[rowInd][colInd];
        }
        cout << endl;
    }
}

/*
 * This method retrieves the action command that the user inputs. This methods prompts the user to enter
 * either a 'a' char, a 't' char, or a 'q' char, and returns the value once one of these have been inputed.
 */
char getNextOperation() {
    string userInput = "";
    while (true) {
        cout << "a)nimate, t)ick, q)uit? ";
        getline(cin,userInput);
        if(userInput.size() == 1 &&
                (userInput[0] == 'a' ||
                 userInput[0] == 't' ||
                 userInput[0] == 'q')) {
            break;
        }
        cout << "Invalid choice; please try again." << endl;
    }
    return userInput[0];
}

/*
 * This method prints the next frame (or evolution) of the grid. It is passed the char grid, and then calls
 * a method to make the appropriate changes to the grid. After this happens, the method prints the grid.
 */
void printNextFrame(Grid<char>& grid){
    progressLife(grid);
    printGrid(grid);
}

/*
 *This method analyzes each place in the grid, decides if it should contain a cell, and makes the changes to
 * the grid. It is passed a grid, which it duplicates. Then, it iterates though each value in the grid and
 * uses the duplicate of the grid to find the number of neighbors that place has. Once it recieves this value,
 * it uses it to determine if it should hold a cell or not. It makes the appropriate change to the original
 * grid.
 */
void progressLife(Grid<char>& grid) {
    Grid<char> checkGrid = grid;
    int neighborCount;
    for(int rowInd = 0; rowInd < grid.numRows(); rowInd++) {
        for(int colInd = 0; colInd < grid.numCols(); colInd++) {
            neighborCount = getNeighborCount(checkGrid, rowInd, colInd);
            if (neighborCount <= 1 || neighborCount >=4) {
                grid[rowInd][colInd] = '-';
            } else if (neighborCount == 3) {
                grid[rowInd][colInd] = 'X';
            }
        }
    }
}

/*
 * This method returns the number of live cells that neighbor a certain place in the grid. It is passed the
 * grid and the row and column index of the position in the grid being analyzed. It first collects the number
 * of neighbors that are in the corner spots relative to the place being analyzed, then add this to the number
 * of cells that are at the side places relative to the place being analyzed. It then returns this value as
 * the total number of neighbors.
 */
int getNeighborCount(Grid<char>& grid, int rowInd, int colInd) {
    int totalNeighbors = 0;
    totalNeighbors += cornerNeighbors(grid, rowInd, colInd);
    totalNeighbors += sideNeighbors(grid, rowInd, colInd);
    return totalNeighbors;
}

/*
 * This method is passed the grid and the row and column index of a place in the grid and returns the number
 * of corner cells that are neighbors to the place being analyzed.
 */
int cornerNeighbors(Grid<char>& grid, int rowInd, int colInd) {
    int cornerCellCount = 0;
    if (cellAt(grid, rowInd-1,colInd-1)) cornerCellCount++;
    if (cellAt(grid, rowInd+1,colInd-1)) cornerCellCount++;
    if (cellAt(grid, rowInd-1,colInd+1)) cornerCellCount++;
    if (cellAt(grid, rowInd+1,colInd+1)) cornerCellCount++;
    return cornerCellCount;
}

/*
 * This method is passed the grid and the row and column index of a place in the grid and returns the number
 * of side cells that are neighbors to the place being analyzed.
 */
int sideNeighbors(Grid<char>& grid, int rowInd, int colInd) {
    int sideCellCount = 0;
    if (cellAt(grid, rowInd-1, colInd)) sideCellCount++;
    if (cellAt(grid, rowInd, colInd+1)) sideCellCount++;
    if (cellAt(grid, rowInd+1, colInd)) sideCellCount++;
    if (cellAt(grid, rowInd, colInd-1)) sideCellCount++;
    return sideCellCount;
}

/*
 * This method recieves the grid and the row and column index of a place in the grid and returns the boolean
 * true if the place is within the bounds of the grid and the place contains an X. If either or both of these
 * clauses are false, the method returns false.
 */
bool cellAt(Grid<char>& grid, int row, int col) {
    if (grid.inBounds(row,col) && grid[row][col] == 'X') {
        return true;
    } else {
        return false;
    }
}

/*
 * This method animates the progression of the environment. It recieves the char grid, then prompts the user
 * to input the number of frames they would like the program to progress. It then clears the console, prints
 * the next frame of the grid, pauses for 50 milliseconds, then repeats the process. It executes this loop
 * the number of times that the user prompts.
 */
void animateGrid(Grid<char>& grid) {
    int frameCount = getFrameCount();
    for(int i = 0; i < frameCount; i++) {
        clearConsole();
        printNextFrame(grid);
        pause(50);
    }
}

/*
 * This method prompts the user to enter the number of frames they would like the grid to progress, then returns
 * the value when the user has entered a valid number.
 */
int getFrameCount() {
   string frames = "";
   while(true) {
       cout << "How many frames? " << endl;
       getline(cin,frames);
       if(isPositiveNumber(frames)) break;
       cout << "Illegal integer format. Try again" << endl;
   }
   return stringToInteger(frames);
}

/*
 * This method is passed a string. It returns the boolean true if the string contains only number from 0 - 9.
 */
bool isPositiveNumber(string frames) {
    bool isValid = true;
    for(int i = 0; i < (int)frames.length(); i++) {
        if(frames[i] < '0' || frames[i] > '9') {
            isValid = false;
        }
    }
    return isValid;
}
