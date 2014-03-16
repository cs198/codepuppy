/*
Section Leader: Jimmy Lee
Name: Kevin Vo
Functionality: This program simulates Conway's Game of Life, which simulates a colony of bacteria.
The rules that the game abides by is listed as follows.

1)Any live cell with fewer than two live neighbours dies, as if caused by under-population.
2)Any live cell with two or three live neighbours lives on to the next generation.
3)Any live cell with more than three live neighbours dies, as if by overcrowding.
4)Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
note: these conditions were taken from Wikipedia

The user has the ability to animate a sequence of frames or to view it one by one or they can just
quit the program.
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

/*These are my function prototypes.*/
void fileReader(Grid<char> &grid);
int addIntoGrid(string line, int columns, Grid<char>& grid, int linenumber);
void makeGrid(Grid<char>& grid, string &line, ifstream &input);
void gridIteration (Grid <char>& grid, ifstream& input, string& line, int col);
void printGrid(Grid<char> &grid);
void animateGrid(int iterations, Grid<char>& grid);
void checkLoop(int initialPointX, int initialPointY, int& occupied, int&empty, Grid<char> &grid);
void copyGridContents(Grid<char>& grid, Grid<char>& temp);
void promptLoop(Grid<char>& grid);

int main() {
    setConsoleSize(700, 400);
    cout << "Wecome to the CS106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << " - A cell with 1 or fewer neighbors dies." << endl;
    cout << " - Locations with 2 neighbors remain stable." << endl;
    cout << " - Locations with 3 neighbors will create life" << endl;
    cout << " - A cell with 4 or more neighbors dies." << endl;
    Grid <char> grid;
    fileReader(grid);
    printGrid(grid);
    promptLoop(grid);
    return 0;
}
/* This is the prompt loop which allows the user to animate, tick or quit.
*/
void promptLoop(Grid<char>& grid){
    while (true){
        string uInput = getLine("a)nimate t)ick q)uit: ");
        if (uInput == "a"){
            int n = getInteger("How many times?: ");
            animateGrid(n, grid);
        }else if (uInput == "t"){
            animateGrid(1, grid);
        }else if (uInput == "q"){
            break;
        }else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    cout << "Have a nice Life!" << endl;
}

/* This function reads in the file.
 */
void fileReader(Grid <char>& grid){
    string filename;
    string line;
    ifstream input;
    while (!fileExists(filename)){
       string filename = getLine("Grid input file name? ");
        if (!fileExists(filename)){
            cout<< "File not found; please try again." << endl;
        }else {
           input.open(filename.c_str());
           break;
        }
    }
    makeGrid(grid, line, input);
}
/*Overall this function reads the text file and puts the data into a grid. It reads
 * the first two lines to determine the dimension of the grid.
*/
void makeGrid(Grid<char>& grid, string& line, ifstream& input){
    int row = 0;
    int col = 0;
    for (int i = 0; i < 2; i++){
        getline(input,line);
        if (i == 0){
            row = stringToInteger(line);
        }
        if (i == 1){
            col = stringToInteger(line);
        }
    }
    grid.resize(row,col);
    gridIteration(grid, input, line, col);
}
/* This function adds the data in the text file into the grid.
*/
int addIntoGrid(string line, int col, Grid<char>& grid, int linenumber){
    for (int j = 0; j < col; j++){
         char ch = line[j];
         grid.set(linenumber,j, ch);
    }
    return 0;
}
/* The function goes over the text file and calls the function addIntoGrid, which
 * then adds in data.
*/
void gridIteration (Grid <char>& grid, ifstream& input, string& line, int col){
    int numberoflines = 0;
    while (getline(input,line) && line != ""){
        addIntoGrid(line, col, grid, numberoflines);
        numberoflines++;
    }
}
/* This function is used to display the grid in the console window.
*/
void printGrid(Grid<char>& grid){
    string str = "";
    for (int r = 0; r < grid.numRows(); r++){
        for (int c = 0; c < grid.numCols(); c++){
            cout << grid[r][c];
        }
    cout<<endl;
    }
}
/*This function is in charge of animating the grid. The strategy implemented was to
 * iterate over the grid to change the contents of the grid. The most outer for loop
 * is used to create the illusion of an animation. I then created a check to see if
 * I should place a '-' in the grid or a 'X'
*/
void animateGrid(int iterations, Grid<char>& grid){
    Grid <char> temp;
    temp.resize(grid.numRows(), grid.numCols());
    for (int x = 0; x < iterations; x++){
        for (int r = 0; r < grid.numRows(); r++) {
             for (int c = 0; c < grid.numCols(); c++) {
                       int empty = 0;
                       int occupied = 0;
                       checkLoop(r,c, occupied, empty, grid);
                       if (occupied < 1){
                           temp.set(r,c, '-');
                       } else if (occupied == 2){
                           temp.set(r,c, grid.get(r,c));
                       } else if (occupied == 3){
                           temp.set(r,c, 'X');
                       } else {
                           temp.set(r,c, '-');
                       }
             }
        }
        copyGridContents(grid, temp);
        clearConsole();
        printGrid(grid);
        pause(50);
    }
}
/* This function copies the content from the grid to the temporary grid, which was an advised
 * strategy in the assignment specifications.
*/
void copyGridContents(Grid<char>& grid, Grid<char>& temp){
    for (int r = 0; r < grid.numRows(); r++) {
         for (int c = 0; c < grid.numCols(); c++) {
            grid[r][c] = temp[r][c];
         }
    }
}
/*What this function does is that it checks the surrounding eight cells of a given cell and counts
 * the number of empty cells and occupied cells and returns it. The reason why I subtracted 1 and added
 * one to the inital points was for the loop to start in the upper left of the of the grid space
 * that we wanted to check. I also passed "occupy" and "empty" by reference because I wanted to return
 * two things.
 */
void checkLoop(int initialPointX, int initialPointY, int& occupied, int&empty, Grid<char>& grid){
    for (int r = initialPointX - 1; r <= initialPointX + 1; r++){
        for (int c = initialPointY - 1; c <= initialPointY + 1; c++){
            if ((r != initialPointX || c != initialPointY) && grid.inBounds(r,c)){
                if (grid.get(r,c) == '-'){
                    empty++;
                }else{
                    occupied++;
                }
            }
        }
    }
}
