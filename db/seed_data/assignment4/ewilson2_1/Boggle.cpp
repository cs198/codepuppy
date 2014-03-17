// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

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

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    if(boardText!="") {
        board=userBoard(boardText);
    } else {
        board=randomBoard();
    }
}

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
            //cout << letter;
        }
        //cout << endl;
    }
    return board;
}

char Boggle::setLetter(string cube) {
    int random=randomInteger(0, cube.length()-1);
    char letter=cube[random];
    return letter;
}

char Boggle::getLetter(int row, int col) {
    char letter=board[row][col];
    return letter;
}

bool Boggle::checkWord(string word) {
    // TODO: implement
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


bool Boggle::humanWordSearch(string word) {
    // TODO: implement
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
    return false;   // remove this
}

bool Boggle::findWord(Vector<int> position, string word, int counter) {
    char ch=word.at(counter);
    cout << "ch=" << ch << endl;
    cout << "substr=" << word.substr(0, counter) << endl;
    Vector< Vector<int> > neighbors=getNeighbors(position);
    //cout << neighbors << endl;
    for(int i=0; i<neighbors.size(); i++) {
        Vector<int> neighbor=neighbors[i];
        int row=neighbor[0];
        int col=neighbor[1];
        cout << "checking row "<< row << endl;
        cout << "checking col "<< col << endl;
        if(ch==board[row][col]) {
            cout << "found character" << endl;
            if(counter==word.length()-1) {
                BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                usedWords.add(word);
                humanPoints=humanPoints+(1+(word.length()-4));
                BoggleGUI::setScore(humanPoints, BoggleGUI::HUMAN);
                return true;
            }
            counter++;
            if(findWord(neighbor, word, counter)) {
                //cout << "true";
                return true;
            } else {
                counter--;
            }
        }
    }
    cout << "false";
    return false;

}

Vector< Vector<int> > Boggle::getNeighbors(Vector<int> position) {
    cout << "neighbors:" << endl;
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

void Boggle::addNeighbors(Vector< Vector<int> >& neighbors, int row, int col) {
    if(checkInBounds(row, col)) {
        Vector<int> neighbor;
        neighbor.add(row);
        //cout << "neighborx=" << row << endl;
        neighbor.add(col);
        //cout << "neighbory=" << col << endl;
        neighbors.add(neighbor);
        cout << board[row][col] << endl;
        //cout << endl;
    }
}

Vector< Vector<int> > Boggle::startPositions(char ch) {
    Vector< Vector<int> > startPositions;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            Vector<int> position;
            if(board[i][j]==ch) {
                position.add(i);
                //cout << "i=" << i << endl;
                position.add(j);
                //cout << "j=" << j << endl;
                startPositions.add(position);
            }
        }
    }
    return startPositions;
}

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

Set<string> Boggle::computerWordSearch() {
    // TODO: implement
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
            cout << "completed board[" << i << "][" << j << "]" << endl;
        }

    }
    //Set<string> result;   // remove this
    BoggleGUI::setScore(computerPoints, BoggleGUI::COMPUTER);
    return foundWords;        // remove this
}

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
            cout << "prefix=" << prefix << endl;
            if(dictionary.contains(prefix)) {
                if(prefix.length()>3) {
                    if(usedWords.contains(prefix)==false) {
                        cout << "user did not find " << prefix << endl;
                        foundWords.add(prefix);
                        computerPoints+=(1+(prefix.length()-4));
                        cout << "found word: " << prefix << endl;
                        BoggleGUI::recordWord(prefix, BoggleGUI::COMPUTER);
                    }
                }
            }
            if(dictionary.containsPrefix(prefix)) {
                cout << "found prefix: " << prefix << ", continuing search" << endl;
                computerWords(neighbor, prefix, foundWords, checks);
            }
            if(dictionary.containsPrefix(prefix)==false) {
                cout << " does not contain prefix: " << prefix << endl;
                prefix=prefix.substr(0, prefix.length()-1);
                cout << "backtracked prefix: " << prefix << endl;
            }
        }

    }
    checks[row][col]=false;
    BoggleGUI::labelCube(row, col, board[row][col], false);
    //prefix=prefix.substr(0, prefix.length()-1);
    //cout << "unchecked cube, new prefix: "<< prefix << endl;

    cout << "unchecking cube" << endl;
    return foundWords;
}

Grid<bool> Boggle::makeBlankCheck() {
    Grid<bool> blankCheck=Grid<bool>(4, 4);
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            blankCheck[i][j]=false;
        }
    }
    return blankCheck;
}

int Boggle::getScoreComputer() {
    // TODO: implement
    return computerPoints;   // remove this
}

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
