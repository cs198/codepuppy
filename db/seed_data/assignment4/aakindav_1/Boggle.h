// Boogle.h creates the Boggle class and all its private,
// public functions as well with memeber variables

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "set.h"

using namespace std;

class Boggle {
public:

    /* Constructs a new Boggle class and fills the grid with the
     * passed in text or random text created by shuffling the CUBES
     * array, also initializes a dictionary Lexicon
     */
    Boggle(Lexicon& dictionary, string boardText = "");

    /* returns the letter associated with the given row and col
     */
    char getLetter(int row, int col) const;

    /* checks to see if the given work is at least four characters
     * long, if it is in the dictionary, and if not already in the set
     */
    bool checkWord(string word);

    /* checks to see if the word entered by the human is on the
     * Boggle gameBoard by using exhaustive recursion
     */
    bool humanWordSearch(string word);

    /* returns the score of the human
     */
    int getHumanScore() const;

    /* searched through the whole Boggle gameBoard using recurisve
     * backtrackng to find every possible word that can be formed
     * and adds every new word to a set.
     */
    Set<string> computerWordSearch();

    /* returns the computer score
     */
    int getScoreComputer() const;

    /* returns the human's word set
     */
    Set<string> getSet() const;

    /* resets the text contained in the Boggle grid to the entered string
     * if it is 16 chars long.
     */
    void resetWithWords(string boardText);

private:
    // member variables inside each list object

    Grid<char> gameBoard;    // grid to store gameboard chars
    Lexicon dict;            // dict to store dictionary
    Set<string> uniqueWords; // set to store the human found words
    int humSum = 0;          // int to store the human score
    int compSum = 0;         // int to store the computer score

    // private "helper" methods

    /* helper recursive function that checks the neighbors for each
     * character on the board
     */
    bool humanWordSearchHelper(string word, Grid<char> gameBoard, int row, int col);

    /* checks the whole board recursive checking all the neighbors and every
     * word that the created word can form
     */
    void computerWordSearchHelper(Grid<char> gameBoard, Set<string>& result, string word, int row, int col);
};

/* Makes the Boggle grid printable to the console
 */
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
