/* Connie Zeng
 * CS 106B, Winter 2014
 * ---------------------
 * Assignment 4: Boggle
 * This program lets the user play a game of Boggle. This file declares the
 * Boggle class and lists its members.
 *
 * (1 late day)
 */

#ifndef _boggle_h
#define _boggle_h

#include "lexicon.h"
#include "grid.h"
#include "stack.h"
#include "point.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    int humanScore();
    Set<string> humanWords();
    Set<string> computerWordSearch();
    int getScoreComputer();

    // TODO: add any other member functions/variables necessary

private:
    Grid<char> board;
    Lexicon boggleDictionary;
    Set<string> words;
    int score;
    int remScore;

    bool humanWordSearch(string word, int x, int y, Set<string>& chosen);
    void findWords(int x, int y, string wordSoFar, Set<string>& chosen, Set<string>& result);

    // TODO: add any other member functions/variables necessary
};

ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
