/**
 * Name: Milan Doshi
 * Assignment: Boggle.h
 * TA: Jimmy Lee
 *
 * This file is the header file which displays all the public and prive member functions and variables necessary in the Boggle class.
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "hashmap.h"
#include "grid.h"
using namespace std;

class Boggle {
public:
    /**
     * method: (Lexicon& dictionary, string boardText = "")
     * return: constructor
     * This method creates a new Boggle object.
     */
    Boggle(Lexicon& dictionary, string boardText = "");

    /**
     * method: getLetter(int row, int col)
     * return: char
     * This method returns the character at a certain row and column in the grid (board).
     */
    char getLetter(int row, int col);

    /**
     * method: checkWord(string word)
     * return: bool
     * This method checks to see whether the required length is met, whether it's in the dictionary, and whether or not its already been used.
     */
    bool checkWord(string word);

    /**
     * method: humanWordSearch(string word)
     * return: bool
     * This method checks to see if a user-inputed word can be found on the board by calling a helper function.
     */
    bool humanWordSearch(string word);

    /**
     * method: humanScore()
     * return: int
     * This method returns the human's score.
     */
    int humanScore();

    /**
     * method: computerWordSearch()
     * return: Set<string>
     * This method checks the entire board for all words that can be created by calling a helper function and then returns a Set of all the words found (by the computer).
     */
    Set<string> computerWordSearch();

    /**
     * method: getScoreComputer()
     * return: int
     * This method returns the computer's score.
     */
    int getScoreComputer();

    /**
     * method: getBoardText()
     * return: string
     * This method returns the string boardText which was set initially in the game, either by random or through the user input.
     */
    string getBoardText();

    /**
     * method: removeWord()
     * return: void
     * This method removes word passed in a parameter from the lexicon which houses all the remaining words so that the next time that lexicon is checked this word
     * won't be in there because it has already been found.
     */
    void removeWord(string word);
    Vector <string> humanWords;
    Vector <string> computerWords;
    string allLetters; //boardText

private:
    Grid<char> boggleboard;
    Lexicon allWords; //entire dictionary
    Lexicon remainingWords; //words that have not been found yet
    string usableBoardText;

    /**
     * method: humanWordSearchHelp(string word, int row, int col, Grid<bool>& booleanGrid)
     * return: bool
     * This method is the recursive helper method called by the humanWordSearch() which goes through all the letters past the first letter in a word and
     * recursively goes through the board to determine whether or not the word is in the board.
     */
    bool humanWordSearchHelp(string word, int row, int col, Grid<bool>& booleanGrid);

    /**
     * method: computerWordSearchHelp(string word, int row, int col, Set<string>& result, Grid<bool>& boolGrid)
     * return: void
     * This method is the recursive helper method called by the computerWordSearch() which goes through all the letters in the board and finds all potential
     * words on the board.
     */
    void computerWordSearchHelp(string word, int row, int col, Set<string>& result, Grid<bool>& boolGrid);

    /**
     * method: shuffleBoard(string& boardText)
     * return: void
     * This method takes in a string and shuffles the string.
     */
    void shuffleBoard(string& boardText);
};

/**
 * method: operator<<(ostream& out, Boggle& boggle)
 * return: ostream&
 * This method overloads <<.
 */
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
