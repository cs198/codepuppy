/*
 * CS106B Assignment 4: Boggle
 * Emma Daugherty
 * Section Leader: Jimmy Lee
 *
 * This is the header file for the Boggle class. It explains all the member functions and variables.
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "stack.h"
#include "bogglegui.h"
using namespace std;

class Boggle {
public:
    /* This is the constructor. It creates a new Boggle board using the Lexicon passed in as a parameter. If the
     * boardText is empty, it creates a random board configuration, otherwise it creates the board configuration
     * based off the 16-character string passed in by the user.
     */
    Boggle(Lexicon& dictionary, string boardText = "");
    /*
     * This function returns the letter at the given space on the board that is passed in as a parameter. It throws
     * an exception if the parameters are out of bounds.
     */
    char getLetter(int row, int col);
    /*
     * This function returns true if the word passed in is at least 4 letters long, in the dictionary and
     * has not already been chosen by the user. Otherwise, it returns false.
     */
    bool checkWord(string word);
    /*
     * This function returns true if the word passed in is a valid word on the board and false if it is not.
     */
    bool humanWordSearch(string word);
    /*
     * This method returns the score of the human player. It is one point for every four letter word, two points
     * for every five letter word, etc...
     */
    int humanScore();
    /*
     * This function returns a set of words that the computer found on the board. It includes all possible words
     * except the words already found by the human.
     */
    Set<string> computerWordSearch();
    /*
     * This function returns the score of the computer. It is one point for every four letter word, two points
     * for every five letter word, etc...
     */
    int getScoreComputer();
    /*
     * This function returns a set of all the words the human found. It is updated everytime the human finds a new
     * word.
     */
    Set<string> getWords();

private:
    // The 16-character string of all the letters on the board.
    string boardText;
    // A grid layout of the board.
    Grid<char> layout;
    // The lexicon storing all the words that are allowed.
    Lexicon dictionary;
    // A set containing all the words the user has entered that are at least 4 letters and in the dictionary.
    // They are not necessarily on the board.
    Set<string> correctEntries;
    // A set containing all the words the computer found on the board.
    Set<string> computerWords;
    // A set containing all the words the human found on the board.
    Set<string> chosenWords;
    /*
     * This is a helper function for humanWordSearch. It keeps track of the position on the board and recursively
     * searches all the possible pathways to check if the word is on the board. Its parameters are the current position
     * and the remaining letters to be found.
     */
    bool humanWordSearchHelper(int row, int col, string remaining);
    /*
     * This is a helper function for computerWordSearch. It keeps track of the current position on the board and
     * recusively searches to see if the surrounding letters can potentially form words. Found words are added to
     * computerWords.
     */
    void computerWordSearchHelper(int row, int col, string word);
    /*
     * This function unhighlights all the cubes on the GUI board.
     */
    void unhighlightBoard();

};
/*
 * This overloads the << operator so that the Boggle board can easily be printed out. It prints out the grid layout
 * of the board.
 */
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
