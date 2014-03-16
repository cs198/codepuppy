/* Charles Akin-David
* CS106B
* SL: Jimmy Lee
* Sources used: Lecture Slides
* I couldn't axcess the Java Applet on my machine so there
* may be slight differences in my error messages.
* This program plays the Game of Life.
* Extension: Can play the game multiple times based on the
* user's answer to the question after one game has been played.
* Extension can be found in the main function.
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
#include "strlib.h"

using namespace std;

void startMessage();
void readFile(Grid<string> &gameBoard);
void makeGameBoard (Grid<string> &gameBoard, ifstream& input, int row, int col);
void printGrid(Grid<string> &gameBoard);
void playGame(Grid<string> &gameBoard);
void animate (Grid<string> &gameBoard);
void tick(Grid<string> &gameBoard);
int checkNeighbors(Grid<string> &gameBoard, int r, int c);
void cellLife(Grid<string> &temp, int r, int c, int neighbors);

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    // Initializes string "again" to store the user response to
    // determine if the game will run again or end
    string again = "";
    while (true) {
        startMessage();
        Grid<string> gameBoard;
        readFile(gameBoard);
        playGame(gameBoard);
        again = getLine("Want to play again? ");
        while (again != "yes" && again != "no") {
            cout << "Please enter \"yes\", or \"no\"!" << endl;
            again = getLine("Want to play again? ");
        }
        if (again == "no") break;
    }
    cout << "Have a nice Life!" << endl;
    return 0;
}

/* Displays the beginning message of the game which
 * describes the rules of the game.
*/
void startMessage(){
    cout << "Welcome to the CS106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies" << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << "" << endl;
    cout << "" << endl;
}

/* Reads the file inputted by the user then stores each
 * line of the in the string "line" takes the first two
 * lines as the row and column vales for the Grid. Then
 * resizes the gris to those values.
 * Takes in the parameter Grid to set the size of and assumes
 * the first two lines in the file will be integers first row
 * then column. Fianlly calls the makeGameBoard function.
 */
void readFile(Grid<string> &gameBoard) {
    ifstream input;
    promptUserForFile(input, "Grid input file name? ");
    string line;
    getline(input, line);
    int row = stringToInteger(line);
    getline(input,line);
    int col = stringToInteger(line);
    gameBoard.resize(row,col);
    makeGameBoard(gameBoard, input, row, col);
}

/* Makes the gameboard for the game by filling in the Grid
 * passed in with the contents of the file stored in the passed
 * in ifstream. Iterates through the ifstream and places all the lines
 * in the grid and a counter is used to make sure that the grid placement
 * does not go out of bounds. Assumes that the grid as been resized and
 * that the row and column values are the ones passed in. Fianlly prints
 * the grid using the printGrid function.
 */
void makeGameBoard (Grid<string> &gameBoard, ifstream& input, int row, int col) {
    int count = 0;
    string line;
    while (getline(input,line)) {
        if (count > row-1) break;
        for(int i=0; i<col; i++) {
            gameBoard[count][i] = line.at(i);
        }
        count++;
    }
    printGrid(gameBoard);
}

/* Prints out the passed in grid by interating through the
 * contents of the grid one by one going through each row
 * and column then printing out each element. Proceeds to
 * ending each line after each row is finshed printing.
 * Assumes the gameboard has been resized.
 */
void printGrid(Grid<string> &gameBoard) {
    for (int r=0; r<gameBoard.numRows(); r++) {
        for (int c=0; c<gameBoard.numCols(); c++) {
            cout << gameBoard[r][c];
        }
        cout << endl;
    }
}

/* Actually plays the game of Life by asking the user to animate
 * tick or quick by entering a, t, or q and displays an error message
 * if none of the three are displayed. Breaks the loop if loop if quit
 * is selected runs the tick function if tick is selected and runs the
 * animate function if animate is selected. Passes in a Grid which is
 * used to pass into the tick and animate functions.
 */
void playGame(Grid<string> &gameBoard) {
    string userChoice;
    while (true) {
        userChoice = getLine("a)nimate, t)ick, q)uit? ");
        if (userChoice != "a" && userChoice != "t" && userChoice != "q") {
            cout << "Please enter \"a\", \"t\", or \"q\"!" << endl;
        }
        if (userChoice == "q") break;
        if (userChoice == "t") tick(gameBoard);
        if (userChoice == "a") animate(gameBoard);
    }
}

/* After animate is selected asks the user the number of
 * frames they wish to animate through and the function
 * uses the tick function that many times and clears the
 * console and pauses in the middle of each tick. Passes in
 * a Grid to pass into the tick function.
 */
void animate (Grid<string> &gameBoard) {
    int times = getInteger("How many frames? ");
    for (int a=0; a<times; a++) {
        clearConsole();
        pause(50);
        tick(gameBoard);
    }
}

/* Makes a temporary copy of the passed in grid and then runs
 * the grid through a double loop pulling out each unit in the
 * grid and finds out the number of neighbors each cell has using
 * the checkNeighbors function and then uses the cellLife function
 * to see if that cell will be alive or not in the next generation.
 * Fianlly replaces the gameboard's Grid with the temporary one then
 * prints out the grid using the printGrid function.
 * Assumes the gameboard has been resized.
 */
void tick(Grid<string> &gameBoard) {
    Grid<string> temp = gameBoard;
    for (int r=0; r<gameBoard.nRows; r++) {
        for (int c=0; c<gameBoard.nCols; c++) {
            int neighbors = checkNeighbors(gameBoard, r, c);
            cellLife(temp, r, c, neighbors);
        }
    }
    gameBoard = temp;
    printGrid(gameBoard);
}

/* Uses the Grid, row and column integers, and the number of neighbors
 * to determine if the cell in that position will remain alive, come
 * to life, die, or remain dead by the number of neighbors that cell
 * has. Runs through a series of checks to accomplish this.
 * Assumes the gameboard has been resized.
 */
void cellLife (Grid<string> &temp, int r, int c, int neighbors) {
    if (neighbors <= 1) {
        temp[r][c] = "-";
    } else if (neighbors == 3) {
        temp[r][c] = "X";
    } else if (neighbors >= 4) {
        temp[r][c] = "-";
    }
}

/* Initializes a count integer to hold the number of neighbors
 * that this function will return. Uses row and column integers
 * passed in and runs through a douple loop going from one row
 * and column in front and in the back to span a 3x3 check. Then
 * goes though a series of checks to make sure that the function
 * is still in bounds of the grid, to make sure to skip of the
 * actually cell and to see if that cell is alive. If all three
 * checks are passed then the count is incremented by one.
 * Assumes the gameboard has been resized.
 */
int checkNeighbors(Grid<string> &gameBoard, int r, int c) {
    int count = 0;
    for (int row=r-1; row<=r+1; row++) {
        for (int col=c-1; col<=c+1; col++) {
            if (gameBoard.inBounds(row,col) && (row != r || col != c) && gameBoard[row][col] == "X") {
                // checks to see if inbounds, if a cell neighbor, and if an alive cell
                count++;
            }
        }
    }
    return count;
}
