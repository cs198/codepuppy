// This is a .h file for the game of Boggle.
// This .h file creates the methods necessary
// to play the game.

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
    bool checkWord(string word, Lexicon& dictionary);
    bool humanWordSearch(string word);
    int humanScore();
    Set<string> computerWordSearch(Lexicon &dictionary);
    int getScoreComputer(Set<string> foundByComputer);
    Grid<string> board;
    Set<string> foundByPlayer;

private:
    bool humanWordSearchHelper(string word, int row, int column, Grid<bool>& usedCheck);
    void createBoard();
    void computerWordSearchHelper(Set<string>& result, string word, int row, int column, Lexicon& dictionary, Grid<bool>& usedCheck);
};

ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
