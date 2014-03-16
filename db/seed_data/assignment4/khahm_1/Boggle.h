/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * Boggle.h file
 * defines the functions and variables defined in a "Boggle"
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "hashset.h"
#include "grid.h"
using namespace std;

class Boggle {
public:
    // builds a Boggle board and object
    Boggle(Lexicon& dictionary, string boardText = "");
    // returns the character at that location
    char getLetter(int row, int col) const;
    // checks if given word is valid
    bool checkWord(string word) const;
    // searches if the human player is asking for a valid word
    bool humanWordSearch(string word, string& boardLetters);
    // returns the human player's score
    int humanScore() const;
    // returns the set of words remaining that the computer finds
    Set<string> computerWordSearch() ;
    // returns the computer's score
    int getScoreComputer() const;
    // returns the set of already found words
    Set<string> getFoundWords() const;

private:
    // dictionary
    Lexicon dict;
    // grid of characters representing the board
    Grid<char> board;
    // set of already found words
    Set<string> foundWords;
    // computer's score
    int computerScoreNum = 0;
    // human player's score
    int humanScoreNum = 0;
    // helper recursive backtracking for humanWordSearch
    bool isValidOnBoard(Grid<char>& temp, string word, int r, int c) const;
    // checks if given indeces are in bounds
    bool isInBounds(int i, int j) const;
    // helper recursive backtracking for computerWordSearch
    bool findAllWordsStartingHere
                (string& word, Grid<char>& temp, Set<string>& result, int r, int c);
};

// overloading << operator in Boggle
ostream& operator<<(ostream& out, const Boggle& boggle);

#endif // _boggle_h
