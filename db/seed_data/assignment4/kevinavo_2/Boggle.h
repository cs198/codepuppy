/* Name: Kevin Vo
 * TA: Jimmy Lee
   This is the h file for the Boggle program. The member functions
   are located in the public section. The member variables and
   any helper function is placed in the private section.
*/
#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "random.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText);
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    int getHumanScore();
    void setHumanScore(int score);
    Set<string> computerWordSearch();
    int getScoreComputer();
    void setScoreComputer(int computerScore);
    Grid <char> getBoard();
    void setUsedWords(string word);
    string displayBoard(Grid<char>& grid, int index);

private:
    Lexicon dictionary;
    string boardText;
    Grid <char> board;
    Lexicon usedWords;
    int score = 0;
    int computerScore = 0;
    bool humanWordSearchHelper(string word, Grid<bool> &chosen, int row, int col);
    void computerWordSearchHelper(Grid<bool> &chosen, int row, int col, string word, Set<string>& allWords);

};

ostream& operator<<(ostream& out, Boggle& boggle);

#endif
