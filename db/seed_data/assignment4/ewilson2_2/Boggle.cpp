// This file defines implements the Boggle class. It contains all of the methods necessary to create a single Boggle object and play a single round.

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "Boggle.h"
#include "grid.h"
#include "iostream"
#include "tokenscanner.h"
#include "shuffle.h"
#include "string"
#include "istream"
#include "simpio.h"
#include "random.h"
#include "lexicon.h"
#include "bogglegui.h"


string getBoardChoice();
Grid<char> board(string choice);
char setLetter(string cube);
Grid<char> randomBoard();
bool checkAlpha(string userString);
string userString();
Grid<char> userBoard();
string toString(Grid<char> board);
Vector< Vector<int> > startPositions(char ch);
bool findWord(Vector<int> position, string word, int counter);
bool checkInBounds(int row, int col);
Vector< Vector<int> > getNeighbors(Vector<int> position);
void addNeighbors(Vector< Vector<int> >& neighbors, int row, int col);
Vector< Vector<int> > startPositions(char ch);
Set<string> computerWords(Vector<int> cube, string prefix, Set<string>& foundWords, Grid<bool>& checks);
Grid<bool> makeBlankCheck();
Set<string> getUsedWords();

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

//This is the constructor for a single Boggle object
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    if(boardText!="") {
        board=userBoard(boardText);
    } else {
        board=randomBoard();
    }
    humanPoints=0;
    computerPoints=0;
}

//This method generates a random playing board
Grid<char> Boggle::randomBoard() {
    Vector<string> cubes;
    for(int i=0; i<16; i++) {
        cubes.add(CUBES[i]);
    }
    shuffle(cubes);
    Grid<char> board=Grid<char>(4, 4);
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            int random=randomInteger(0, cubes.size()-1);
            string cube=cubes[random];
            char letter=setLetter(cube);
            BoggleGUI::labelCube(i, j, letter);
            cubes.remove(random);
            board.set(i, j, letter);
        }
    }
    return board;
}

//This method generates a board from the user's inputted text
Grid<char> Boggle::userBoard(string boardText) {
    Vector<char> userVec;
    for(int i=0; i<boardText.length(); i++) {
        userVec.add(boardText[i]);
    }
    Grid<char> board=Grid<char>(4, 4);
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            char letter=userVec[0];
            userVec.remove(0);
            board.set(i, j, letter);
        }
    }
    return board;
}

//This method sets a single random letter on the board
char Boggle::setLetter(string cube) {
    int random=randomInteger(0, cube.length()-1);
    char letter=cube[random];
    return letter;
}

//This method returns the value of a letter on the board given the row and column location
char Boggle::getLetter(int row, int col) {
    char letter=board[row][col];
    return letter;
}

//This method checks to see if a word inputted by the user is valid
bool Boggle::checkWord(string word) {
    word=toUpperCase(word);
    if(word.length()<4) {
        return false;
    } else if (usedWords.contains(word)) {
        return false;
    } else if(dictionary.contains(word)==false) {
        return false;
    } else {
        return true;
    }
}


//This method attempts to find a word inputted by the user on the given board. It will return true if the word is found.
//This method also acts as a helper method for the below method, find word, which solves the problem recursively.
bool Boggle::humanWordSearch(string word) {
    word=toUpperCase(word);
    char ch=word.at(0);
    Vector< Vector<int> > starts=startPositions(ch);
    if(starts.size()==0) {
        return false;
    }
    for(int i=0; i<starts.size(); i++) {
       Vector<int> position=starts[i];
       int counter=1;
       bool foundWord=findWord(position, word, counter);
       if(foundWord) {
           return true;
       }
    }
    return false;
}

//This method uses recursive backtracking to find a word guessed by the user on the given board. It returns true if the word is found.
bool Boggle::findWord(Vector<int> position, string word, int counter) {
    char ch=word.at(counter);
    Vector< Vector<int> > neighbors=getNeighbors(position);
    for(int i=0; i<neighbors.size(); i++) {
        Vector<int> neighbor=neighbors[i];
        int row=neighbor[0];
        int col=neighbor[1];
        if(ch==board[row][col]) {
            if(counter==word.length()-1) {
                BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                usedWords.add(word);
                humanPoints=humanPoints+(1+(word.length()-4));
                BoggleGUI::setScore(humanPoints, BoggleGUI::HUMAN);
                return true;
            }
            counter++;
            if(findWord(neighbor, word, counter)) {
                return true;
            } else {
                counter--;
            }
        }
    }
    return false;

}

//This method returns the coordinates of all of the legal neighbors of a given cube
Vector< Vector<int> > Boggle::getNeighbors(Vector<int> position) {
    Vector< Vector<int> > neighbors;
    int row=position[0];
    int col=position[1];
    int row1=row-1;
    int col1=col-1;
    int row3=row+1;
    int col3=col+1;
    addNeighbors(neighbors, row1, col1);
    addNeighbors(neighbors, row1, col);
    addNeighbors(neighbors, row1, col3);
    addNeighbors(neighbors, row, col1);
    addNeighbors(neighbors, row, col3);
    addNeighbors(neighbors, row3, col1);
    addNeighbors(neighbors, row3, col);
    addNeighbors(neighbors, row3, col3);
    return neighbors;
}

//This method will add a vector containing the coordinates of a legal neighbor to a vector of vectors
void Boggle::addNeighbors(Vector< Vector<int> >& neighbors, int row, int col) {
    if(checkInBounds(row, col)) {
        Vector<int> neighbor;
        neighbor.add(row);
        neighbor.add(col);
        neighbors.add(neighbor);
    }
}

//This method is used to help find a word that the user has guessed. It searches for all of the possible locations of the starting letter of the word, and returns a vector of vectors containing the coordinates.
//Each vector containing coordinates within the larger vector represents a point to begin the recursive search for the word.
Vector< Vector<int> > Boggle::startPositions(char ch) {
    Vector< Vector<int> > startPositions;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            Vector<int> position;
            if(board[i][j]==ch) {
                position.add(i);
                position.add(j);
                startPositions.add(position);
            }
        }
    }
    return startPositions;
}

//This method checks to see if the coordinates of a given cube are in bounds
bool Boggle::checkInBounds(int row, int col) {
    if(row<0 || row>=4) {
        return false;
    }
    if(col<0 || col>=4) {
        return false;
    }
    return true;
}

int Boggle::humanScore() {
    // TODO: implement
    return humanPoints;   // remove this
}

//This method performs the computer's search for words on the board. It will return a set of strings of all of the found words.
//This method also acts as a wrapper function for the below function computerWords, which uses recursion to solve the problem.
Set<string> Boggle::computerWordSearch() {
    Set<string> foundWords;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            Grid<bool> checks=makeBlankCheck();
            string word="";
            word+=board[i][j];
            Vector<int> cube;
            cube.add(i);
            cube.add(j);
            computerWords(cube, word, foundWords, checks);
        }

    }
    BoggleGUI::setScore(computerPoints, BoggleGUI::COMPUTER);
    return foundWords;
}

//This method uses recursive backtracking to find all of the possible words on the board, and returns a set of strings containing all of the words.
Set<string> Boggle::computerWords(Vector<int> cube, string word, Set<string>& foundWords, Grid<bool>& checks) {
    int row=cube[0];
    int col=cube[1];
    checks[row][col]=true;
    BoggleGUI::labelCube(row, col, board[row][col], true);
    Vector< Vector<int> > neighbors=getNeighbors(cube);
    for(int i=0; i<neighbors.size(); i++) {
        string prefix=word;
        Vector<int> neighbor=neighbors[i];
        int row1=neighbor[0];
        int col1=neighbor[1];
        if(checks[row1][col1]==false) {
            prefix+=board[row1][col1];
            if(dictionary.contains(prefix)) {
                if(prefix.length()>3) {
                    if(usedWords.contains(prefix)==false) {
                        if(foundWords.contains(prefix)==false) {
                            foundWords.add(prefix);
                            computerPoints=computerPoints+(1+(prefix.length()-4));
                            BoggleGUI::recordWord(prefix, BoggleGUI::COMPUTER);
                            BoggleGUI::setScore(computerPoints, BoggleGUI::COMPUTER);
                        }
                    }
                }
            }
            if(dictionary.containsPrefix(prefix)) {
                computerWords(neighbor, prefix, foundWords, checks);
            }
            if(dictionary.containsPrefix(prefix)==false) {
                prefix=prefix.substr(0, prefix.length()-1);
            }
        }

    }
    checks[row][col]=false;
    BoggleGUI::labelCube(row, col, board[row][col], false);
    return foundWords;
}

//This method creates an empty grid of booleans, initialized to false.
//The grid will be used to mark which squares have been visited or not; if a square has been visited, other methods will change the corresponding location to "true"
Grid<bool> Boggle::makeBlankCheck() {
    Grid<bool> blankCheck=Grid<bool>(4, 4);
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            blankCheck[i][j]=false;
        }
    }
    return blankCheck;
}

//This method returns the score of the computer
int Boggle::getScoreComputer() {
    return computerPoints;
}

//This method overloads the << operator to print out a boggle object's contents
ostream& operator<<(ostream& out, Boggle& boggle) {
    Grid<char> board=boggle.board;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            out << board[i][j];
        }
        out << endl;
    }
    out << "" << endl;
    return out;
}
