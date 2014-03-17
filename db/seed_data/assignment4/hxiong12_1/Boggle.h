/*
 * CS 106B Boggle
 * This file declares the Boggle class and its methods and instance variables.
 * Allows the user to create a Boggle board, and play a game using these methods.
 *
 * Author: Helen Xiong
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "vector.h"
#include "set.h"
#include "shuffle.h"
#include "bogglegui.h"
using namespace std;

class Boggle {
public:

/*Declares the edge size of a normal, square Boggle board.
 * Can be modified to fit bigger boards.*/
    const int BOGGLE_SIZE = 4;

/*Declares the number of faces on a die.*/
    const int DICE_SIZE = 6;

/*Creates a Boggle board and game interface.
 * Inputs: dictionary - lexicon of valid words,
 *         boardText - an optional 16-letter string corresponding to letters on a custom board.*/
    Boggle(Lexicon& dictionary, string boardText);

/*Returns the character found at an input row and column of the Boggle board.*/
    char getLetter(int row, int col);

/*Checks to see if the word
 * 1) is in the dictionary,
 * 2) is greater than four characters
 * 3) has not been found before.
 * Returns true if all three conditions have been met.*/
    bool checkWord(string word);

/*Checks if the word can be formed from adjacent (including diagonal) tiles on the Boggle board.
 * Returns true and adds the word to the set of humanWordsFound if the word can be formed.*/
    bool humanWordSearch(string word);

/*Returns the total points accumulated by the human player.
 * One point is given for each four-letter long word,
 * and one additional point is given for each additional letter in the word.
 *Uses the private instance set of human words found.*/
    int humanScore();

/*Returns a set of all words that can be formed on the Boggle board, that have not been found
 * by the human player already.*/
    Set<string> computerWordSearch();

/*Returns the set of all words found by the human player so far. The data itself
 * cannot be modified through this method because it is stored in the private instance
 * Set humanWordsFound.*/
    Set<string> getHumanWords();

/*Returns the score of the computer "player". For each 4-letter word that was found by the computer
 * but was not found by the human player, the computer gets one point. For each
 * additional letter in the word, the computer receives one extra point.*/
    int getScoreComputer();

private:
/*Sets all characters on the "visited" grid and the GUI to false/un-highlighted.
 * (See description for Grid<bool> visited.) */
    void resetVisited();

/*A private grid of characters on the Boggle board. Initialized by the createBoard method.*/
    Grid<char> board;

/*A private grid of booleans that correspond to which character have been "used". This grid
 * is used to keep track of:
 * 1) filling in random cubes for the random createBoard method.
 * 2) which letters have been "visited" (i.e. already checked) in the wordSearch methods.
 *For #2 in general, if a cube is "visited" it is also highlighted in the GUI interface.*/
    Grid<bool> visited;

/*Generates a random Boggle board if there is no input string of characters by the user.
 * Stored in the private instance Grid Boggle::board.*/
    void createBoard();

/*Generates a Boggle board according to a 16-letter string input by the user.
 * Stored in the private instance Grid Boggle::board.*/
    void createBoard(string boardText);

/*A private copy of the input Lexicon; used to check if a certain word is part of the English language.*/
    Lexicon b_dictionary;

/*A private set of the words that have been found by the human player.
 * Words are added if the humanWordSearch() method returns true.*/
    Set<string> humanWordsFound;

/*A helper method for the computerWordSearch function that adds words to a set WordsFound if
 * the word can be formed on the board. The capsule function computerWordSearch "filters"
 * the words that have not been found by the human player into a public set.*/
    void computerWordSearchHelper(Set<string>& wordsFound, string word, int i, int j);

/*A helper method for the humanWordSearch method. Returns true if the word
 * can be formed on the board from adjacent tiles.*/
    bool humanWordSearchHelper(string word, int i, int j, int index);
};

/*Overloads the operator<< to print out the Boggle board in four lines of four characters.*/
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
