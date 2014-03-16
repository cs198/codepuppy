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
#include "hashset.h"
#include "point.h"
#include "bogglegui.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char const getLetter(int row, int col);
    bool const checkWord(string word);
    bool humanWordSearch(string word);
    bool humanWordSearchHelper(string word, string wordCopy, string currentWordFound, int gridX, int gridY, HashSet<Point> pointsChecked);
    int const humanScore();
    Set<string> computerWordSearch();
    void computerWordSearchHelper(string currentWordFound, Set<string>& wordsFound, int gridX, int gridY, HashSet<Point> pointsChecked);
    int const getScoreComputer();
    Grid<char> const getGrid();


private:
    HashSet<string> wordsEntered;
    Grid<char> boggleGrid;
    Lexicon boggleDictionary;
    int playerScore;
    int computerScore;

};

ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
