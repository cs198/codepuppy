/* This program plays the Game of Life. It allows the user to read
 * in a file containing information on the bacteria colony, and see
 * how the generations of bacteria evolve.
 * For this assignment, I used notes from lecture, sample programs
 * from the Programming Abstractions textbook, and help from Professor
 * Stepp and the Lair.
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

void readFile(Vector<string>& lines);
void createData(Grid<char>& data, Vector<string>& lines, int nrows, int ncolumns);
void printGrid(Grid<char>& data, int nrows, int ncolumns);
void greeting();
void updatingData(Grid<char>& data, int nrows, int ncolumns);
string userInput();
int frameNumber();

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    greeting();

    Vector<string> lines;

    readFile(lines);

    int nrows = stringToInteger(lines.get(0));
    int ncolumns = stringToInteger(lines.get(1));

    Grid<char> data;
    data.resize(nrows, ncolumns);

    createData(data, lines, nrows, ncolumns);

    printGrid(data, nrows, ncolumns);

    string choice = userInput();

    while(true){
        if(choice == "a"){
            int framenum = frameNumber();

            while(framenum > 0){
                updatingData(data, nrows, ncolumns);
                clearConsole();
                pause(50);
                printGrid(data, nrows, ncolumns);
                framenum--;
            }

            choice = userInput();

        }else if(choice == "t"){
            updatingData(data, nrows, ncolumns);
            printGrid(data, nrows, ncolumns);
            choice = userInput();

        }else if(choice == "q"){
            cout << "Have a nice Life!";
            break;

        }else{
            cout << "Invalid choice; please try again." << endl;
            choice = userInput();
        }
    }

    return 0;
}

/* The greeting function prints out an introduction to the Game of Life
 * for the user
 */
void greeting(){
    cout << "Welcome to the CS106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
}

/* The readFile function prompts the user for a file and reads the
 * lines of the file. It puts these lines in a vector of strings.
 * Once all the file has been read, it closes the file.
 */
void readFile(Vector<string>& lines){

    ifstream input;
    promptUserForFile(input, "Grid input file name? ");
    string line;
    while(true){
        bool success = getline(input, line);
        if(!success){
            break;
        }
        lines.add(line);
    }
    input.close();
}


/* The createData function takes the empty character data grid (which will hold the
 * bacteria colony information) and fills it with the information from
 * the vector of strings (created from reading the file).
 */
void createData(Grid<char>& data, Vector<string>& lines, int nrows, int ncolumns){
    int gridrow = 0;

    for(int i=2; i<=(1+nrows); i++){
        string temp = lines.get(i);

        for(int j=0; j<ncolumns; j++){
            char character = temp[j];
            data.set(gridrow, j, character);
        }
        gridrow++;
    }
}


/* The printGrid function takes the grid of data of the bacteria
 * colony's state and prints this data to the console for the
 * user to view.
 */
void printGrid(Grid<char>& data, int nrows, int ncolumns){
    for (int r = 0; r < nrows; r++) {
        if(r > 0){
            cout << endl;
        }

        for (int c = 0; c < ncolumns; c++) {
            cout << data[r][c];
        }
    }

    cout << endl;
}

/* The updatingData function takes the current grid of data of
 * the bacteria colony and updates this data for the next generation
 * depending on the number of neighbors each bacteria has. It stores
 * the next generation in a temporary grid as it iterates through the
 * data grid. The temporary grid is then stored into the data grid.
 */
void updatingData(Grid<char>& data, int nrows, int ncolumns){

    Grid<char> temporary;
    temporary.resize(nrows, ncolumns);

    for(int r=0; r<nrows; r++){

        for(int c=0; c<ncolumns; c++){

            int neighborcount = 0;

            int rowstart = r-1;
            int rowend = r+1;
            int colstart = c-1;
            int colend = c+1;

            if((r-1) < 0){
                rowstart = r;
            }
            if((r+1) > (nrows - 1)){
                rowend = r;
            }
            if((c-1) < 0){
                colstart = c;
            }
            if((c+1) > (ncolumns-1)){
                colend = c;
            }

            for(int a = rowstart; a <= rowend; a++){
                for(int b = colstart; b <= colend; b++){
                    if(data[a][b] == 'X'){
                        neighborcount++;
                    }
                }
            }

            if(data[r][c] == 'X'){
                neighborcount--;
            }

            if (neighborcount == 0 || neighborcount == 1 || neighborcount >= 4){
                temporary[r][c] = '-';
            }else if (neighborcount == 2){
                temporary[r][c] = data[r][c];
            }else if (neighborcount == 3){
                temporary[r][c] = 'X';
            }
        }
    }

    data = temporary;
}

/* The userInput function accepts the user's choice to animate
 * the Game of Life, to see the next generation, or to quit
 * the Game of Life. It returns this choice.
 */
string userInput(){
    string choice = getLine("a)nimate, t)ick, q)uit? ");
    return choice;
}

/* The frameNumber function asks the user how many frames
 * (or generations) they would like to advance the bacteria colony.
 * It returns the number of frames the user inputs.
 */
int frameNumber(){
    int frame = getInteger("How many frames? ");
    return frame;
}
