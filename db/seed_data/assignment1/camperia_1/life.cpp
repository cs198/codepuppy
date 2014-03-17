/*
 * This file simulates Conway's Game of Life
 * by Andrés Camperi
 * section leader: Jimmy Lee
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

void printWelcomeMessage();
string getFileName();
Grid<string> makeGrid(ifstream& file);
void printGrid(Grid<string>&);
void update(Grid<string>&);
int countNeighbors(int i , int j , Grid<string>&);
void playGame(Grid<string>&);

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    //prints the welcome message
    printWelcomeMessage();

    //gets the file name from the user and opens it in an ifstream
    string filename = getFileName();
    ifstream file;
    openFile(file, filename);

    //creates the grid from the file
    Grid<string> lifeGrid = makeGrid(file);

    //starts the main loop of the program that animates it/ticks it forwards
    playGame(lifeGrid);

    return 0;
}

void printWelcomeMessage(){
    cout << "Welcome to the CS 106B Game of Life, \n"
            "a simulation of the lifecycle of a bacteria colony. \n"
            "Cells (X) live and die by the following rules: \n"
            "- A cell with 1 or fewer neighbors dies. \n"
            "- Locations with 2 neighbors remain stable. \n"
            "- Locations with 3 neighbors will create life. \n"
            "- A cell with 4 or more neighbors dies.\n" << endl;
}

string getFileName(){
    //asks for a filename and checks for correct input
    string filename;
    while(true){
        filename = getLine("Grid input file name? ");
        if (fileExists(filename)){
            //if it exists
            break;
        } else{
            //if the file doesn't exist
            cout << "File not found; please try again." << endl;
        }
    }
    return filename;
}

Grid<string> makeGrid(ifstream& file){
    //creates the frid with all the vales

    Grid<string> grid; //initializes it
    string line;
    getline(file, line); //gets the first line (integer with the number of rows)
    int height = stringToInteger(line);
    getline(file, line); //gets the second line (integer with the number of columns)
    int width = stringToInteger(line);
    grid.resize(height, width); //resizes the grid with the correct number of rows and columns

    //adds all the values to the grid
    for (int i = 0 ; i < height ; i++){
        //line by line
        getline(file, line);
        for (int j = 0 ; j < width ; j++){
            string cell = string("") + line[j];
            grid.set(i, j, cell);
        }
    }

    return grid;
}

void printGrid(Grid<string>& grid){
    //prints the grid to the console
    for (int i = 0 ; i < grid.numRows() ; i++){
        for (int j = 0 ; j < grid.numCols() ; j++){
            if (j+1 == grid.numCols()){
                //need to go to the next line (end line) at the end of a row
                cout << grid.get(i , j) << endl;
            } else{
                cout << grid.get(i , j);
            }
        }
    }
}

void update (Grid<string>& grid){
    //updates the grid to the next generation
    Grid<string> temp = grid; //creates a temporary grid and sets it equal to the original
    int neighbors; //initializes a variable to count the amount of neighbors
    for (int i = 0 ; i < grid.numRows() ; i++){
        for (int j = 0 ; j < grid.numCols() ; j++){
            neighbors = countNeighbors(i , j , grid); //gets the amount of neighbors

            //we don't need to worry about the case with 2 neighbors because that's a stable position (doesn't change)
            if (neighbors <= 1 || neighbors >= 4){
                //if the amount of neighbors is less than 2 or more than 3 the cell will be dead
                temp.set(i , j , string("-"));
            } else if (neighbors == 3){
                //if the amount of neighbors is 3 the cell will be alive
                temp.set(i , j , string("X"));
            }
        }
    }
    grid = temp; //sets the grid equals to the temp
}

int countNeighbors(int r, int c, Grid<string>& grid){
    //counts the amount of neighbors a cell has

    int count = 0;

    for (int i = r-1 ; i <= r+1 ; i++){
        //for int i that starts at one before the current row and ends one after the current row
        for (int j = c-1 ; j  <= c+1 ; j++){
            //for int j that starts one before the current column and ends one after the current column
            if (grid.inBounds(i , j)){
                //if the grid square is in the bounds (if it exists in the grid)
                if (i != r || j != c){
                    //if it isn't the current square for whom we're counting neghbors
                    if (grid.get(i , j) == string("")+'X'){
                        //if there is a neighbor
                        count++;
                    }
                }
            }
        }
    }

    return count;
}

void playGame(Grid<string>& lifeGrid){
    //plays the game (animate tick or quit)
    printGrid(lifeGrid);
    while(true){
        //first asks the user what to do (animate, tick, or quit)
        string whatDo = getLine("a)nimate, t)ick, q)uit? ");
        if (whatDo == string("")+'t'){
            //if the user hit tick updates the grid and reprints it
            update(lifeGrid);
            printGrid(lifeGrid);
        } else if (whatDo == string("")+'a'){
            //if the user hit animate
            int reps = getInteger("How many frames? ");
            for (int i = 0 ; i < reps ; i++){ //iterates as many times as the user wants
                //animates the simulation by clearing the screen, updating the grid, printing it and then pausing
                clearConsole();
                update(lifeGrid);
                printGrid(lifeGrid);
                pause(50);
            }
        } else if (whatDo == string("")+'q'){
            //quits if the user presses q
            cout << "Have a nice Life!" << endl;
            break;
        } else{
            //if the user inputted anything but a t or q
            cout << "Invalid choice; please try again." << endl;
        }
    }
}
