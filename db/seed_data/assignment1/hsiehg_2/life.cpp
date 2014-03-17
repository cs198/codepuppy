//// This is a simulation of the "Game of Life" where cells live and die according to a few simple rules.
//// A grid represents the world and within this world, "X"'s represent cells and "-"'s represent empty cells.
//// This simulation allows the user to view the changes in the world (grid) 1 generation at a time, or pick
//// the number of generations they want the program to cycle through at once.
//// Extensions: Random world generator (Lines 41-43, 60-62, 79-97)

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

/*Function prototypes*/
void printIntro();
bool isNum(string s);
void printGrid(Grid<string>& grid);
void getDimensions(string file, int& row, int& col);
void readToGrid(string file, Grid<string>& grid);
void update (Grid<string>& grid);
void updateCopy (int r, int c, int xCount, Grid<string>& temp, Grid<string>& grid);
bool validInput(string ans);
void animate(Grid<string>& grid);
void promptForChoice(string& userInput);
int getXCount(int r, int c, Grid<string>& grid);
void runLife(Grid<string>& lifeGrid);
Grid<string> createFileGrid(string file);
void promptForValidFile(string& fileName);

/*Extension Function Prototypes*/
Grid<string> createRandomGrid();

/*Extension Constants*/
const int maxRows = 50;
const int maxCols = 50;

/*
 * Program main that sets the console characteristics.
 * Prompts the user for a valid text file until the user enters a valid string.
 * Then either sets up a randomly generated grid or reads in a grid from a pre-existing file and
 * prints it to the console. AFterwards, it runs the life simulation.
 */
int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    printIntro();
    string fileName = getLine("Grid input file name? ");
    promptForValidFile(fileName);
    if(fileExists(fileName) || toLowerCase(fileName) == "random") {
        Grid<string> lifeGrid;
        if(toLowerCase(fileName) == "random") {
            lifeGrid = createRandomGrid();
        }
        else {
            lifeGrid = createFileGrid(fileName);
        }        
        printGrid(lifeGrid);
        runLife(lifeGrid);
    }
    return 0;
}

/*
 * Creates a grid with random dimensions and random placement of "X" and "-".
 * The two dimensions (randRow and randCol) is a random number generated between 1 and 50.
 * After the random grid is resized to the dimensions, each cell is either filled with "X" or "-"
 * based on a randomly generated boolean variable that is true 50% of the time.
 *
 */
Grid<string> createRandomGrid() {
    int randRow = randomInteger(1, maxRows);
    int randCol = randomInteger(1, maxCols);
    Grid<string> randGrid;
    randGrid.resize(randRow, randCol);
    bool isEmpty = randomChance(0.5);
    for(int i = 0; i < randRow; i++) {
        for(int j = 0; j < randCol; j++) {
            if(isEmpty) {
                randGrid[i][j] = "-";
            }
            else{
                randGrid[i][j] = "X";
            }
            isEmpty = randomChance(0.5);
        }
    }
    return randGrid;
}

/*
 * Prompts user repeatedly for a file name entered in the format "filename.txt" until
 * the user enters a valid file name.
 */
void promptForValidFile(string& fileName) {
    while(!fileExists(fileName) && toLowerCase(fileName) != "random") {
        cout << "File not found; please try again." << endl;
        fileName = getLine("Grid input file name? ");

    }
}

/*
 * Sets up the initial grid displayed on the console after the user enters a valid file name.
 * This function converts the file into a grid object and then prints the result to the console.
 */
Grid<string> createFileGrid(string file) {
    Grid<string> grid;
    int row;
    int col;
    getDimensions(file, row, col);
    grid.resize(row, col);
    readToGrid(file, grid);
    return grid;
}

/*
 * Runs the user interaction part of the Game of Life program.
 * Prompts the user for either a)nimate, t)ick or q)uit until the user enters a valid letter.
 * Animates if the user enters "a", updates the current grid by 1 frame if the user enters "t"
 * and exits the program if the user enters "q".
 */
void runLife(Grid<string>& lifeGrid) {
    string ans;
    promptForChoice(ans);
    while(ans != "q") {
        if(ans == "a") {
            animate(lifeGrid);
        }
        else {
            update(lifeGrid);
        }
        promptForChoice(ans);
    }
    cout << "Have a nice Life!" << endl;
}

/* Prompts user repeatedly for either a)nimate, t)ick or q)uit until the user enters a valid letter.
 * If the user enters an invalid letter, a message will be displayed to notify them.
 */
void promptForChoice(string& ans) {
    cout << "a)nimate, t)ick, q)uit? ";
    getline(cin, ans);

    while(ans != "a" && ans != "t" && ans != "q") {
        cout << "Invalid choice; please try again." << endl;
        cout << "a)nimate, t)ick, q)uit? ";
        getline(cin, ans);
    }
}

/* Prompts the user for the number of frames they would like to animate the grid.
 * Clears the console and updates repeatedly for the number of frames specified.
 */
void animate(Grid<string>& grid) {
    int frames = getInteger("How many frames? ");
    for(int i = 0; i < frames; i++) {
        clearConsole();
        update(grid);
        pause(50);
    }
}

/*
 * Creates a temporary grid that is an updated version of the original grid passed in.
 * The temporary grid has the same dimensions as the original grid, but each cell changes
 * based on the number of X's that is around that cell. Afterwards, the original grid is set
 * to the temporary grid to complete the update.
 */
void update (Grid<string>& grid) {
    Grid<string> temp;
    temp.resize(grid.numRows(), grid.numCols());
    temp = grid;
    for (int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            int numX = getXCount(i, j, grid);
            updateCopy(i, j, numX, temp, grid);
        }
    }
    grid = temp;
    printGrid(grid);
}

/*
 * Returns the number of "X's" around a cell with location at row r and column c. This function
 * iterates through a 3x3 grid around the specified cell and increments xCount only if the locations
 * are in the bounds of the grid and if the location is not the location being checked.
 */
int getXCount(int r, int c, Grid<string>& grid) {
    int xCount = 0;
    for(int x = r-1; x < r+2; x++) {
        for(int y = c-1; y < c+2; y++) {
            if(grid.inBounds(x,y) && grid[x][y] == "X" && !(x == r && y == c)) {
                xCount++;
            }
        }
    }
    return xCount;
}

/*
 * Updates the temporary grid (temp) given the location to be updated (r, c) and the number of "X's" that are
 * around that location. If the number of "X's" is less than 2 or greater than 3, the location (r, c) will be
 * replaced by a "-". If the number of "X's" is exactly 2, the location's content will remain the same. Otherwise,
 * the location (r, c) will be replaced by a "X".
 */
void updateCopy (int r, int c, int xCount,
                 Grid<string>& temp, Grid<string>& grid) {
    if(xCount < 2 || xCount > 3) {
        temp[r][c] = "-";
    }

    else if (xCount == 2) {
        temp[r][c] = grid[r][c];
    }

    else {
        temp[r][c] = "X";
    }
}

/*
 * Reads a text file and stores each individual character into a Vector object.
 * Then transfers the characters in the Vector to a Grid object so that every cell in the grid
 * corresponds to a single character from the Vector object.
 */
void readToGrid(string file, Grid<string>& grid) {
    ifstream input;
    input.open(file);
    char c;
    Vector<char> charVector;
    while(input.get(c)) {
        if(c == '-' || c == 'X')
            charVector.add(c);
    }
    input.close();
    int index = 0;
    for(int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            grid[i][j] = charVector[index];
            index++;
        }
    }
}

/*
 * Reads only the first two lines of the text file and adds those two strings into a Vector object.
 * Sets the row and column equal to the integer equivalent of the strings.
 */
void getDimensions(string file, int& row, int& col){
    Vector<string> numVector;
    ifstream input;
    input.open(file);
    string line;
    while(getline(input, line)) {
        if(isNum(line)) {
            numVector.add(line);
            if(numVector.size() == 2) {
                break;
            }
        }
    }
    input.close();
    row = stringToInteger(numVector[0]);
    col = stringToInteger(numVector[1]);
}

/*
 * Prints the grid in a grid format to the console (instead of a linear format).
 */
void printGrid(Grid<string>& grid) {
    for(int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            cout << grid[i][j];
            if(j == grid.numCols()-1) {
                cout << endl;
            }
        }
    }
}

/*
 * Assumes that the only strings that can be entered are characters from the file.
 * Returns true if a number is entered, meaning not a "X", "-", "#" or space.
 */
bool isNum(string s) {
    return !startsWith(s, "X") && !startsWith(s, "-") && !startsWith(s,"#") && !startsWith(s," ");
}

/*
 * Prints the introductory message at the very start of the program.
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
