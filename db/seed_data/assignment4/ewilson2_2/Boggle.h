//This file defines the Boggle class, containing all of the variables and the prototypes for all of the functions

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    int humanScore();
    Set<string> computerWordSearch();
    int getScoreComputer();

    Grid<char> board;
    Lexicon dictionary=Lexicon("dictionary.txt");
    Set<string> usedWords;

private:
    string getBoardChoice();
    Grid<char> randomBoard();
    Grid<char> userBoard(string boardText);
    string userString();
    Vector< Vector<int> > startPositions(char ch);
    bool findWord(Vector<int> position, string word, int counter);
    bool checkInBounds(int row, int col);
    Vector< Vector<int> > getNeighbors(Vector<int> position);
    void addNeighbors(Vector< Vector<int> >& neighbors, int row, int col);
    Set<string> computerWords(Vector<int> cube, string prefix, Set<string>& foundWords, Grid<bool>& checks);
    Grid<bool> makeBlankCheck();
    int humanPoints;
    int computerPoints;
    char setLetter(string cube);

};
ostream& operator<<(ostream& out, Boggle& boggle);


#endif // _boggle_h
