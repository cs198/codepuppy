// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "set.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary);
    void setCheckedLetters();
    void setHighlightedLetters();
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    int humanScore();
    Set<string> computerWordSearch();
    int getScoreComputer();
    void createBoard(string);
    Set<string> getHumanWords();

    // TODO: add any other member functions/variables necessary

private:
    Lexicon Dictionary; //the lexicon
    Grid<char> boggleBoard; //the board grid
    Set<string> humanWords; //set of the human's words
    Grid<bool> checkedLetters; //a separate grid of bools to keep track within the loop of which letters have been used
    Set<string> result; //set of the computer's words
};

ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
