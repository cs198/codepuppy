/// Min Cheol Kim
/// CS106B
/// Section Leader Jimmy Lee
/// Section time Wed 5:15
/// Assignment + Extension!
/* The assignment was to create a game that models the life cycle of a simple lifeform in
   in a two dimensional grid of cells. The cells are born or killed on the grid based on a
   set of rules, displayed once the game starts. The user has three types of commands: animate
   (running the simulation for a certain number of frames, tick (advancing the generation by one
   frame), and quit (exiting the program). */

/* The "wrap around" extension code was written in the function getNumberOfNeighbors, refer to
   the comment describing the function. */

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
#include "vector.h"
using namespace std;

void introMessage();
void setUpGrid(Grid<string> &g);
void displayGrid(Grid<string> &g);
int getNumberOfNeighbors(Grid<string> &g, int row, int col);
void advanceGeneration(Grid<string> &g);
void animation(Grid<string> &g);
void tick(Grid<string> &g);

/* The main sets up the console and runs a while loop that exits on the "quit" command.
   Within the loop, the user is prompted for a command and the appropriate function is
   called based on the command. */
int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    Grid<string> lifeBoard;
    introMessage();
    setUpGrid(lifeBoard);
    displayGrid(lifeBoard);
    string command;
    /* The loop runs until the "quit" command is given. */
    while (command != "q") {
        cout << "a)nimate, t)ick, q)uit? ";
        cin >> command;
        if (command == "a")
            animation(lifeBoard);
        else if (command == "t")
            tick(lifeBoard);
        else if (command != "q")
            cout << "Invalid choice; please try again. " << endl;
    }
    cout << "Have a nice Life! " << endl;
    return 0;
}

/* This function prints the introductory message to the program,
 * describing the rules of the Game of Life.*/
void introMessage() {
    cout << "Welcome to the CS 106B Game of Life," << endl
            << "a simulation of the lifecycle of a bacteria colony." << endl
            << "Cells (X) live and die by the following rules:" << endl
            << "- A cell with 1 or fewer neighbors dies. " << endl
            << "- Locations with 2 neighbors remain stable." << endl
            << "- Locations with 3 neighbors will create life."  << endl
            << "- A cell with 4 or more neighbors dies." << endl << endl;
}

/*This function takes the reference to a Grid<sting> and fills the grid up with
  the pattern found in textfile the user specifies. The function prompts the user
  for the filename. */
void setUpGrid(Grid<string>& g) {
    ifstream input;
    string line;
    cout << "Grid input file name? ";
    string filename;
    cin >> filename;
    if(fileExists(filename))
        openFile(input, filename);
    else {
        while (!fileExists(filename)) {
            cout << "File not found; please try again." << endl;
            cout << "Grid input file name? ";
            cin >> filename;
        }
        openFile(input, filename);
    }
    getline(input, line);
    int numRows = stringToInteger(line);
    getline(input, line);
    int numCols = stringToInteger(line);
    g.resize(numRows, numCols);
    for (int r = 0; r < numRows; r++) {
        getline(input, line);
        for (int c = 0; c < numCols; c++)
            g[r][c] = line[c];
    }
}

/* This function takes a reference to a Grid<string> and prints the grid in a
   neat way, printing each row and moving down a line. */
void displayGrid (Grid<string>& g) {
    for (int r = 0; r < g.numRows(); r++) {
        for (int c = 0; c < g.numCols(); c++)
            cout << g[r][c];
        cout << endl;
    }
}

/* This function takes a reference to a Grid<string> and two int parameters (row, column)
   referring to the location on the grid. It calculates and returns the number of neighbors in the
   immediately adjacent eight cells surrounding the given location.

    For the extension wrap around, the cells on the far left considered the cells on the far right
    their neighbors, and the bottom most cells consider the top most cells as their neighbors.
    The grid becomes infinite in size. The number of neighbors is increased apropriately. */
int getNumberOfNeighbors(Grid<string>& g, int row, int col) {
   int neighbors = 0;
   /* Adding neighbors within the grid */
   for (int r = row-1; r <= row+1; r++)
       for (int c = col-1;c <= col+1; c++)
           if (!(r == row && c == col))
                if (g.inBounds(r,c)&& g[r][c] == "X")
                     neighbors++;
   /* Extension code start, adding neighbors wrapping around the grid */
   if (row == 0) {
       for (int c = col-1; c <= col+1; c++)
           if (g.inBounds(g.numRows()-1,c) && g[g.numRows()-1][c] == "X")
                neighbors++;
   }
   if (row == g.numRows()-1) {
       for (int c = col-1; c <= col+1; c++)
            if (g.inBounds(0,c) && g[0][c] == "X")
                neighbors++;
   }
   if (col == 0)
       for (int r = row-1; r <= row+1; r++)
            if (g.inBounds(r, g.numCols()-1) && g[r][g.numCols()-1] == "X")
                neighbors++;
   if (col == g.numCols()-1)
       for (int r = row-1; r <= row+1; r++)
            if (g.inBounds(r, 0) && g[r][0] == "X")
                neighbors++;
   if (col == 0 && row == 0)
       if (g[g.numRows()-1][g.numCols()-1] == "X")
           neighbors++;
   if (col == g.numCols()-1 && row == 0)
       if (g[g.numRows()-1][0] == "X")
               neighbors++;
   if (col == 0 && row == g.numRows()-1)
       if (g[0][g.numCols()-1] == "X")
           neighbors++;
   if (col == g.numCols()-1 && row == g.numRows()-1)
       if (g[0][0] == "X")
               neighbors++;
   /* Extension code end */
   return neighbors;
}

/* This function takes in a reference to a Grid<string> and modify the grid into the next
   generation of cells. This is done by setting a temporary grid equal to the original
   grid, and making modifications on the original grid using the information from the saved
   temporary grid. */
void advanceGeneration (Grid<string>& g)
{
    Grid<string> temp = g;
    /* The following code implements the rules of the Game based on the number of the neighbors
       a specific location has. */
    for (int r = 0; r < temp.numRows(); r++)
        for (int c = 0; c < temp.numCols(); c++) {
            if (getNumberOfNeighbors(temp, r, c) <= 1)
                    g[r][c] = "-";
            if (getNumberOfNeighbors(temp, r, c) == 2 && temp[r][c] == "-")
                    g[r][c] = "-";
            if (getNumberOfNeighbors(temp, r, c) == 3 && temp[r][c] == "-")
                    g[r][c] = "X";
            if (getNumberOfNeighbors(temp, r, c) >= 4 && temp[r][c] == "X")
                    g[r][c] = "-";
        }
}

/* This method takes in a reference to a Grid<string> and prompts the user for the number
   of frames for animation. It then advances generation for that specific frames with 50 ms
    pause in between. It clears the console after each frame. */
void animation (Grid<string>& g) {
    cin.ignore(100000, '\n');
    int generations = getInteger("How many frames? ");
    for (int n = 0; n < generations; n++) {
        advanceGeneration(g);
        displayGrid(g);
        pause(50);
        clearConsole();
    }
}

/* This method takes in a reference to a Grid<string> and advances the generation once
   and displays the grid */
void tick (Grid<string>& g) {
    advanceGeneration(g);
    displayGrid(g);
}

