// Boggle.h
// This file contains the member functions and member variables of the Boggle class.

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "vector.h"
#include "stack.h"
#include "set.h"
#include "hashset.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = ""); //Boggle constructor
    char getLetter(int row, int col) const; //Returns the letter in the boggle board at the specified row and column, throws an int exception if out of bounds
    bool checkWord(string word); //Checks the validity of a word
    bool humanWordSearch(string word); //Returns true if word can be formed on the board, false otherwise
    int humanScore() const; //Returns the player's score
    Set<string> computerWordSearch(); //Returns a set of all the words that can be formed on the board (by the computer)
    int computerScore(); //Returns the computer's score
    Set<string> foundWordsSet(); //Returns the set of words that the user found

private:
    /*Variables*/
    Grid <char> board; //Stores a single letter in each index
    Lexicon gameDictionary; //Dictionary of words that is set in the constructor
    Set<string> foundWords; //Contains all words that the user has found
    int hScore; //Player score

    /*Functions*/
    bool humanWordSearchHelper(string word, Grid<bool>& isUsed, int row, int col, string chosen = "") const; //Helper function for the human recrusive function
    void computerWordSearchHelper(int r, int c, Grid<bool>& isUsed, Set<string>& result, string chosen); //Helper function for the computer recursive function
    void createBoard(string boardText); //Creates a grid
    void getBoardString(string boardText, string& boardStr) const; //Gets a valid string that is either given or randomly generated
    void fillBoard(string boardStr); //Fills in the grid object with letters from a string
    void updateHumanData(string word); //Updates variables associated with the human player (foundWordsSet and hScore)
};

ostream& operator<<(ostream& out, Boggle& boggle); //Overrides operator to print boggle board in grid format

#endif // _boggle_h
