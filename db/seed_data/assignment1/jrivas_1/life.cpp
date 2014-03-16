/*
 * This program simulates the life cycle of bacteria using a
 * two-dimensional grid of cells. The game simulates the birth
 * and death of future generations of cells using a set of
 * rules.
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

void Welcome();
void PromptForFile(ifstream & infile, Grid<int> & grid1);
void ReadFile(ifstream & infile, Grid<int> & grid1);
void FillGrid(ifstream& infile, Grid<int>& grid1);
void PromptForMovement(Grid<int> & grid1);
void AnimateLife(Grid<int> & grid1);
void TickScreen(Grid<int> & grid1);
int CountNeighbors(Grid<int> grid1, int row, int col);
void PrintGrid(Grid<int> grid);

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    ifstream infile;
    Grid<int> grid1;
    Welcome();
    PromptForFile(infile,grid1);
    PromptForMovement(grid1);
    cout << "Have a nice Life!" << endl;
    return 0;
}

/*
 * Welcome introduces the program, and shows the rules to the user.
 */

void Welcome() {
    cout << "Welcome to the CS106B Game of Life," << endl;
    cout << "A simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neigbhors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
}
/*
 * This asks the user for a filename. If the file is able to open,
 * the file will be read by ReadFile; if not, it will continue asking
 * the user for a valid file until one is inputted.
*/

void PromptForFile(ifstream & infile, Grid<int> & grid1) {
    string prompt = "Grid input file name? ";
    while (true) {
        string filename = getLine(prompt);
        infile.open(filename.c_str());
        if (!infile.fail()) break;
        cout << "File not found; please try again." << endl;
        infile.clear();
    } ReadFile(infile, grid1);
}

/*
 * ReadFile sizes the grid according to the dimensions
 * provided by the file; it then sends it to FillGrid to
 * fill in the newly resized grid.
*/
void ReadFile(ifstream & infile, Grid<int> & grid1) {
    int rows = 0;
    int cols = 0;
    string line;
    if (getline(infile,line)) {
        rows = stringToInteger(line);
    } if (getline(infile,line)) {
        cols = stringToInteger(line);
    } grid1.resize(rows, cols);
    FillGrid(infile, grid1);
}

/*
 * FillGrid reads in the file and inserts a life whenever
 * it sees that there is an X.
 */

void FillGrid(ifstream & infile, Grid<int> & grid1) {
    int rows = grid1.numRows();
    int cols = grid1.numCols();
    string line;
    for (int i = 0; i < rows; i++) {
        if (getline(infile, line)) {
            for (int j = 0; j < cols; j++) {
                if (line.at(j) == 'X') {
                    grid1[i][j] = 1;
                } cout << line.at(j);
            }
        } if (infile.fail()) break;
        cout << endl;
    }
}
/*
 * PromptForMovement asks the user if it wants to
 * animate, tick, or quit the program. If the user selects
 * quit, the cycle ends; if they tick or animate, it
 * sends them to the appropriate function. The cycle continues
 * until quit is selected.
*/

void PromptForMovement(Grid<int> & grid1) {
    string prompt = "a)nimate, t)ick, q)uit? ";
    while (true) {
        string instruction = getLine(prompt);
        if (instruction == "q") break;
        else if (instruction == "a") {
            AnimateLife(grid1);
        } else if (instruction == "t") {
            TickScreen(grid1);
        } else {
            cout << "Invalid choice; please try again." << endl;
        }
    }
}

/*
 * AnimateLife asks the user for the number of
 * frames, and then uses the TickScreen function for
 * the designated number of frames, inserting a pause
 * and clearing the screen when appropriate.
 */

void AnimateLife(Grid<int> & grid1) {
    string prompt = "How many frames? ";
    int frames = getInteger(prompt);
    for (int i = 0; i < frames; i++) {
        clearConsole();
        TickScreen(grid1);
        pause(50);
    }
}
/*
 * TickScreen creates a new grid and then goes through
 * each cell to count the neighbors; then, depending on
 * the neighbors, fills the new grid with dead and living
 * cells. Once this is complete, the grid prints out and the
 * old grid is replaced with the new grid.
 */

void TickScreen(Grid<int> & grid1) {
    Grid<int> grid2 = grid1;
    int rows = grid1.numRows();
    int cols = grid1.numCols();
    grid2.resize(rows, cols);
    int neighbors = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            neighbors = CountNeighbors(grid1, i, j);
            if (neighbors == 2) {
                grid2[i][j] = grid1[i][j];
            }else if (neighbors == 3) {
                grid2[i][j] = 1;
            } else grid2[i][j] = 0;
        }
    } PrintGrid(grid2);
    grid1 = grid2;
}

/*
 * CountNeighbors counts the number of neighbors, going through
 * all nine cells but excluding the cell itself. It returns the int
 * of neighbors of a specific cell.
 */

int CountNeighbors(Grid<int> grid1, int row, int col) {
   int neighbors = 0;
   for (int i = row - 1; i <= row + 1; i++) {
       for (int j = col - 1; j <= col + 1; j++) {
           if (grid1.inBounds(i,j)) {
               if ((!(i == row && j == col)) && grid1[i][j] == 1) neighbors++;
           }
       }
   } return neighbors;
}

/*
 * PrintGrid goes through the grid and prints an X if the cell is
 * alive, and a dash otherwise.
 */
void PrintGrid(Grid<int> grid) {
    int rows = grid.numRows();
    int cols = grid.numCols();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 1) {
                cout << 'X';
            } else cout << '-';
        } cout << endl;
    }
}
