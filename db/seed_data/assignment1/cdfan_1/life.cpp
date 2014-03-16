//// This is a program that simulates the game of life.
//// The game models the life cycle of bacteria using a two-dimensional grid of cells. Given an initial pattern,
//// the game simulates the birth and death of future generations of cells using a set of simple rules.

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

const int DIE = 1;
const int SURVIVE = 2;
const int BIRTH = 3;
const int OVER_CROWD = 4;

//Prototypes
void welcomeMsg();
void readFileToGrid(Grid<string> & generation);
void printGrid(Grid<string> & generation);
void updateActions(Grid<string> & generation);
void simulate( Grid<string> & generation);
int countNeighbors(Grid<string> & generation, int i, int j);

//Main program
int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    welcomeMsg();
    Grid<string> generation;
    readFileToGrid(generation);
    printGrid(generation);
    updateActions(generation);

    cout << "Have a nice Life!" << endl;
    return 0;
}

void welcomeMsg() {
    //Welcome message and rules
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with "<< DIE <<" or fewer neighbors dies." << endl;
    cout << "- Locations with " << SURVIVE <<" neighbors remain stable." << endl;
    cout << "- Locations with " << BIRTH << " neighbors will create life." << endl;
    cout << "- A cell with " << OVER_CROWD << " or more neighbors dies." << endl;
    cout << " " << endl;
}

void readFileToGrid(Grid<string> & generation) {
    //Prompt correct file name from the user
    ifstream input;
    promptUserForFile(input, "Grid input file name? ");

    //read the numbers of rows and cols for grid and resize grid
    string line;
    getline(input,line);
    int r = stringToInteger(line);
    getline(input,line);
    int c = stringToInteger(line);
    generation.resize(r,c);

    // Assign cell values to grid by reading eaching line from the file
    for(int i=0; i<generation.numRows(); i++) {
        getline(input,line);
        for(int j=0; j<generation.numCols(); j++){
            generation[i][j]=line[j];
        }
    }
    input.close();
}

// Print out the current state of the grid: one generation of the cells.
void printGrid(Grid<string> & generation) {
    string line;
    for(int i=0; i<generation.numRows(); i++) {
        for(int j=0; j<generation.numCols(); j++){
           line += generation[i][j];
        }
        cout << line << endl;
        line ="";
    }
}

//Seek action commands from the user:animate, tick or quit. The requests continue until the user hits quit. If the
//command is tick, the program simulate the cells by one generation. In animation, the number of frames is also
//requested from the user, then it goes on to simute that many generations of the cells with the effect of animation.
void updateActions(Grid<string> & generation) {
    while (true){
        string action = getLine("a)nimate, t)ick, q)uit? ");
        if(action == "q") {
            break;
        } else if(action == "t") {
            simulate(generation);
            printGrid(generation);
        } else if(action == "a") {
            int times = getInteger("How many frames? ");
            for(int i=0; i<times; i++) {
                clearConsole();
                simulate(generation);
                printGrid(generation);
                pause(50);

            }
        } else {
            cout << "Invalid choice; please try again." << endl;
        }
    }
}

//It implements the rule of the game for one generation. It goes through every cell of the grid. Based on the
//number of their neighbors, it determine whether they die, live, or reproduce in next generation.
// The next generation was temporarily stored in a temp grid, and after every cell is determined, it is copied
// into the original grid.
void simulate( Grid<string> & generation) {
    Grid<string> temp;
    temp.resize(generation.numRows(),generation.numCols());
    for(int i=0; i<generation.numRows(); i++) {
        for(int j=0; j<generation.numCols(); j++){
           int neighbors = countNeighbors(generation,i,j);
           if(neighbors <= DIE) temp[i][j] = "-";
           if(neighbors == SURVIVE) temp[i][j] = generation[i][j];
           if(neighbors == BIRTH) temp[i][j] = "X";
           if(neighbors >= OVER_CROWD) temp[i][j] = "-";
        }
    }
    generation = temp;
}

// Go through all the neighboring positions to [i][j], excluding the cell itself. If there is a 'X', it counts as one.
// It accrues to the total number of cells living abjacent to [i][j] in the grid, and return that count to the calling method
int countNeighbors(Grid<string> & generation, int i, int j) {
    int count = 0;
    for(int n=i-1; n<=i+1;n++) {
        for(int k=j-1; k<=j+1; k++){
            if(generation.inBounds(n,k) && generation[n][k] == "X") count++;
        }
    }
    if(generation[i][j] == "X") count--;
    return count;
}






