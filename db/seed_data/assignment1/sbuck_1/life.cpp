//// This is the CPP file I edited and turned in.
//// Also remove these comments here and add your own.
//// TODO: remove this comment header!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

void fileReader(Grid<char>& lifeGrid, int& rows, int& columns);
void animateCommand(int frames, Grid<char>& lifeGrid, int& rows, int& columns) ;
void openMessage();
void acctionCommand(Grid<char>& lifeGrid, int& rows, int& columns);

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);
    openMessage();
    Grid<char> lifeGrid;
    int rows;
    int columns;
    fileReader(lifeGrid, rows, columns);
    acctionCommand(lifeGrid, rows, columns);
    cout << "Have a nice Life!" << endl;
    return 0;
}

/**
 * @brief acctionCommand
 * acctionCommand takes in the user option to animate, tick
 * or quit and runs it through the program.
 */
void acctionCommand(Grid<char>& lifeGrid, int& rows, int& columns) {
    while (true) {
        string name;
        cout << "a)nimate, t)ick, q)uit? ";
        cin >> name;
        if (name == "a") {
            int frames;
            cout << "How many frames? ";
            string a;
            cin >> a;
            bool intTest = true;
            int b = a.length();
            for(int i = 0; i < b; i++) {
                if(!isdigit(a.at(i))) {
                    intTest = false;
                }
            }
            if(intTest) {
                frames = stringToInteger(a);
                animateCommand(frames, lifeGrid, rows, columns);
            } else {
                cout << "Illegal integer format. Try again." << endl;
            }
       } else if(name == "t") {
            animateCommand(1, lifeGrid, rows, columns);
       } else if(name == "q") {
            break;
       } else {
            cout << "Invalid choice; please try again." << endl;
       }
    }
}

/**
 * @brief openMessage
 * openMessage() displays the opening message for the game.
 */
void openMessage() {
    cout << "Welcome to the CS 106B Game of Life, " << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
}

/**
 * @brief oneTick
 * @param copyGrid
 * @param row
 * @param column
 * oneTick() is used in the animateCommand() to actually change the
 * grid one tick
 */
void oneTick(Grid<char>& copyGrid, Grid<char>& lifeGrid, int row, int column) {
    int counter = 0;
    for(int i = -1; i < 2; i++) {
        for(int y = -1; y < 2; y++) {
            if(copyGrid.inBounds(row + i, column + y)){
               if(copyGrid[row + i][column + y] == 'X') {
                  counter++;
                }
            }
        }
    }
    if(copyGrid[row][column] == 'X') {
        counter--;
    }
    if(counter <= 1) {
        lifeGrid[row][column] = '-';
    }else if(counter == 3){
        lifeGrid[row][column] = 'X';
    }else if(counter >= 4){
        lifeGrid[row][column] = '-';
    }
}

/**
 * @brief animateCommand
 * @param frames
 * animateCommand takes in how many frames that the user requests
 * and runs the number of ticks.
 */
void animateCommand(int frames, Grid<char>& lifeGrid, int& rows, int& columns) {
     for(int a = 0; a < frames; a++) {
        Grid<char> copyGrid = lifeGrid;
        for(int b = 0; b < rows; b++) {
            for(int i = 0; i < columns; i++) {
                oneTick(copyGrid, lifeGrid, b, i);
            }
        }
        pause(50);
        if(frames > 1) {
            clearConsole();
        }
        for(int i = 0; i < rows; i++){
            for(int a = 0; a < columns; a++) {
                cout << lifeGrid[i][a];
            }
            cout <<  endl;
        }
    }

}

/**
 * @brief fileReader
 * fileReader opens a file that's name inputed into
 * it and reads the lines into a grid.
 */
void fileReader(Grid<char>& lifeGrid, int& rows, int& columns) {
    string name;
    cout << "Enter file name:";
    cin >> name;
    while (true) {
        if(fileExists(name)) {
            int counter = 0;
            ifstream input;
            input.open(name.c_str());
            string line;
            getline(input, line);
            rows = stringToInteger(line);
            getline(input, line);
            columns = stringToInteger(line);
            lifeGrid.resize(rows , columns);
            for (int i = 0; i < rows; i++) {
                getline(input, line);
                for(int i = 0; i < columns; i++) {
                    lifeGrid[counter][i] = line[i];
                }
                counter++;
            }
//           while(getline(input, line)) { //for testing purposes
//                 cout << line << endl;
//            }
            input.close();
            for(int i = 0; i < rows; i++){
                for(int a = 0; a < columns; a++) {
                    cout << lifeGrid[i][a];
                }
                cout <<  endl;
            }
            break;
         }else {
            cout << "File not found; please try again." << endl;
            cout << "Enter file name:";
            cin >> name;
        }
    }
}
