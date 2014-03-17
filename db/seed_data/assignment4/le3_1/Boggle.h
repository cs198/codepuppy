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
#include <vector.h>
#include "hashset.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    string getString();
    Grid<char> getGrid();
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    int getHumanWordsNum(); // return how many words human found
    HashSet<string> getHumanWords();
    int humanScore();
    Set<string> computerWordSearch();
    int getComputerWordsNum();
    int computerScore();


    // TODO: add any other member functions/variables necessary

private:
    // TODO: add any other member functions/variables necessary
    Lexicon dic;
    string boardString;
    Grid<char> boardGrid;
    string word;
    HashSet<string> humanWords;
    int totalHumanWords; // num of human words
    int humanWordScore;
    bool humanWordSearchHelper(string word, int row, int col, int num);
    Set<string> computerWords;
    int computerWordScore;
    int totalComputerWords; // num of computer words
    void computerWordSearchHelper(string cword,int row, int col);
};

ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
