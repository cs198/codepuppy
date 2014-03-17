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
    /*
     * Constructs a Boggle board that recognizes the entries of the passed
     * dictionary as valid words. Fills in the board with the passed string,
     * or with random letters from the Boggle dice if the string is empty.
     * Throws an exception if the string length is neither 16 nor 0.
     */
    Boggle(const Lexicon& dictionary, string boardText = "");

    /*
     * Returns the letter at the given row and column. Throws an integer
     * exception if out of bounds.
     */
    char getLetter(int row, int col) const;

    /*
     * Returns whether the given word is valid. The word must be found in the
     * dictionary, not already found in the game, and at least 4 letters long.
     */
    bool checkWord(string word) const;

    /*
     * Returns whether the passed word can be found on the board. Examines each
     * letter on the board as a possible start to the word. If the word is
     * found, then the list of found words and human score are updated.
     */
    bool humanWordSearch(string word);

    /*
     * Returns the human's score. One point is earned for each letter after the
     * third letter, in each found word.
     */
    int humanScore() const;

    /*
     * Returns the words found by the human.
     */
    Set<string> humanWords() const;

    /*
     * Returns the words found by the computer. These are all the words that
     * could possibly be formed from the board, except the words already found
     * by the human.
     */
    Set<string> computerWordSearch();

    /*
     * Retuns the computer's score. As with the human's score, one point is
     * earned for each letter after the third letter, in each found word.
     */
    int getScoreComputer() const;


private:
    Grid<char> board;          // keeps track of current state of Boggle board
    const Lexicon boggleDictionary;  // stores dictionary entries
    Set<string> words;         // stores words found by the human
    int score;                 // stores the human's score
    int remScore;              // stores the computer's score

    /*
     * Returns whether the passed string can be formed on the Boggle board
     * starting at the given (x, y) letter. Examines each neighbor for the next
     * letter in the string, highlighting each letter as it is examined. A
     * letter position that has already been chosen cannot be reused in the
     * same word.
     */
    bool humanWordSearch(string word, int x, int y, Set<string>& chosen);

    /*
     * Finds all words that could be formed on the Boggle board starting at the
     * given (x, y) letter. A letter position that has already been chosen
     * cannot be reused in the same word.
     */
    void findWords(int x, int y, string wordSoFar, Set<string>& chosen, Set<string>& result);

};

/*
 *  Displays the Boggle board to the console
 */
ostream& operator<<(ostream& out, const Boggle& boggle);

#endif // _boggle_h
