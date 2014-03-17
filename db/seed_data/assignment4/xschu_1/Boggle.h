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
#include "bogglegui.h"
using namespace std;

class Boggle {
public:
    Boggle(const Lexicon& dictionary, const string boardText = "");
    char getLetter (const int row, const int col) const;
    bool checkWord(const string word) const;
    bool humanWordSearch(const string word);
    int humanScore() const;
    Set<string> computerWordSearch();
    int getScoreComputer() const;

    Vector<string> humanWords() const;

private:
    void buildRandomBoard();
    void buildInputBoard (const string text);
    bool humanWordFindable(const string word);
    bool humanWordFindableHelper(Grid<string>& tempBoard, const string word, int index, int row, int col);
    void computerWordSearchHelper(string word, int row, int col, Grid<string>& tempBoard);
    Grid<string> board;
    Vector<string> humanSearchResult;
    Set<string> computerSearchResult;
    string cubeLetters;
    int hScore = 0;
    int cScore = 0;
    Lexicon dict;
};

ostream& operator<<(ostream& out, const Boggle& boggle);

#endif // _boggle_h



