/* This .h file declares all Boggle class members, to be implemented in the cpp file.*/

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
    void displayGrid();
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    void printFoundWords(string word);
    int humanScore();
    Set<string> computerWordSearch();
    void printCompFoundWords(Set<string> compFoundWords);
    int getScoreComputer(Set<string> compFoundWords);
    int getBoardSize();
    Set<string> foundWords;

private:
    Grid<char> board;
    const int BOARD_SIZE = 4;
    const int NUM_SQUARES = BOARD_SIZE*BOARD_SIZE;
    const int PAUSE_INTERVAL = 100;
    Lexicon dict;
    bool humanWordSearchHelper(string& word, int row, int col, Grid<bool>& used, int currentCharIndex);
    Set<string> computerWordSearchHelper(int row, int col, Grid<bool> used, string soFar, Set<string> result);
    Set<string> compFoundWords;
};

ostream& operator <<(ostream& out, Boggle& boggle);

#endif // _boggle_h
