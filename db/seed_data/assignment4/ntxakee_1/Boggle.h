/* Name: Lindsey Txakeeyang
 * Class: 106B, Wednesday 4:15 section
 * Assignment: Boggle
 * This program replicates the game of Boggle with one human player and one computer player.
 */


#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"

using namespace std;

class Boggle {
public:
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;
    Grid<char> gameboard;

    /*
     * Constructs a new Boggle board using the given dictionary lexicon to look up words, and
     * uses the given 16-letter string to initialize the 16 board cubes from top left to bottom right.
     * Generates a random shuffled board if string passed is empty.
     */
    Boggle(Lexicon& dictionary, string boardText = "");

    /*
     * Returns the character stored at the given row/column on the board.
     * Throws an int exception if row/col are out of bounds.
     */
    char getLetter(int row, int col);

    /*
     * Intakes a string.
     * Returns true only if the function isRightLength, inDictionary, and humanWordSearch return true, and
     * hasBeenUsed returns false.
     */
    bool checkWord(string word, Lexicon& dictionary);

    /*
     * Intakes a string and returns true if the word can be found on the gameboard.
     */
    bool humanWordSearch(string word);

    /*
     * Computes the score for the human by iterating through each word in the set and
     * applying the scoring rules of Boggle.
     */
    int humanScore();

    /*
     * Finds all the words that can be formed on the board that follow the rules of Boggle.
     */
    Set<string> computerWordSearch(Lexicon& dictionary);

    /*
     * Computes the score for the computer by iterating through each word in the set and
     * applying the scoring rules of Boggle.
     */
    int computerScore();

    /*
     * Returns a randomly configured 16-letter string.
     */
    string shuffleCubes();

    /*
     * Intakes a 16-letter string and assigns each letter to a square on the gameboard Grid.
     */
    void setBoard(string boardText);

    /*
     * Intakes a string and adds it to the set of words that have been guessed by the human player.
     */
    void addHumanUsedWord(string word);

    /*
     * Prints a string version of the gameboard.
     */
    void printBoard();

    /*
     * Prints a string version of the set of words that have been guessed by the human player.
     */
    string printHumanUsedWords();

    /*
     * Intakes a string and returns true only if the string's length is equal to or greater than 4.
     */
    bool isRightLength(string word);

    /*
     * Intakes a string and returns true only if the string can be found in the set of words that have
     * been guessed by the human player.
     */
    bool hasBeenUsed(string word);

    /*
     * Intakes a string and returns true only if the string can be found in the provided dictionary file
     * initialized in the Lexicon.
     */
    bool inDictionary(string word, Lexicon& dictionary);

    /*
     * Returns how many words can be found in the set of words guessed by the human player.
     */
    int numberOfHumanWords();

    /*
     * Returns the number of words that can be found in the set of words found by the computer.
     */
    int numberOfComputerWords();

    /*
     * Returns a string of words that can be found in the set of words found by the computer.
     */
    string printComputerWords();

    /*
     * Clears the sets of used words for both the human and computer so that a new game can be played.
     */
    void resetSets();

private:
    // member variables inside each Boggle object
    Grid<bool> visitedTiles;
    Grid<bool> compVisitedTiles;
    Set<string> humanWords;
    Set<string> computerWords;

    // private member functions

    /*
     * Intakes the word guessed by the user, and the row and column ints in order to
     * check if the neighbors of a tile contain letters that could spell out the word.
     */
    bool neighborsCanSpellWord(string word, int row, int column);

    /*
     * Checks if the word passed through the parameters is a valid, unused word by
     * checking if it is in bounds, unvisited, and matches the given letter.
     */
    bool isValidUnusedWord(string word, int x, int y);

    /*
     * Intakes a string and checks the neighbors of the current tile to find if any words
     * can be formed by adding the letters on those tiles to the string.
     * Adds a word to the set of words found by the computer if the dictionary contains the
     * word, the human player didn't guess it, and the word is the right length.
     */
    void computerWordSearchHelper(string word, int r, int c, Lexicon& dictionary);

};

/*
 * Prints a Boggle object to the console.
 */
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
