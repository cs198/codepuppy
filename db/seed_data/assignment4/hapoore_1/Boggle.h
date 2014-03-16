// This is the header file for the boggle class. This class keeps track of all the internal
// data required to play a game of Boggle against the computer, which utilizes recursive
// backtracking. The boggle class keeps track of the board state, words found by each player,
// scores. If features an overloaded "<<" operator to allow for easier printing of the game
// state to the console. The boggle class also contains a pair of recursive searching function
// to find words on the board.

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "vector.h"
#include "strlib.h"
#include "random.h"
#include "shuffle.h"
#include "stack.h"

using namespace std;

class Boggle {
public:
    // The Boggle class constructor. It gets passed a Lexicon representing the valid dictionary
    // and has the option of also receiving a string of 16 letters to fill the board tiles. If
    // no string is passed, a random board is generated based on the cube templates provided.
    Boggle(Lexicon& dictionary, string boardText = "");

    // This method is a "getter" method from the boggle class. It gets passed a location and
    // returns the char found at that spot on the board.
    char getLetter(int row, int col);

    // This boolean method gets passed a string and makes sure it is of requisite length, a valid
    // english word, and that it has not already been found. If all of these conditions are met,
    // the method returns true. If it fails one of the tests, the function returns false.
    bool checkWord(string word);

    // This boolean checks to make sure the string parameter word can be formed from the cubes on
    // the board. It makes use of a recursive helper method to search the board for the word. If the
    // word is on the board, the function returns true. Otherwise, it returns false.
    bool humanWordSearch(string word);

    // This function is a "getter" method that returns the human player's score as an int.
    int humanScore();

    // This function calls a helper method which uses recursive backtracking to search the board for
    // all of the valid words. It returns a set of all the valid words that the human player did
    // not find.
    Set<string> computerWordSearch();

    // This function is a "getter" that returns the score of the computer player as an integer.
    int getScoreComputer();

    // This function is a "getter" that returns the number of words found by the human player.
    int getHumanWords();

    // This function is a "getter" that returns the number of words found by the computer player.
    int getComputerWords();

    // This function is a "getter" that returns a string which contains all of the words found by the
    // human player.
    string playerWords();

private:
    // This private variable stores the characters that make up the current board.
    Grid<char> board;

    // This private variable is used to keep track of the board spots that have already
    // been checked during the recursive search algorithms.
    Grid<bool> indicesChecked;

    // This Lexicon stores the valid English dictionary against which the words on the board
    // are compared.

    Lexicon validLexicon;

    // This private variable is a set which stores the words found by the human player.
    Set<string> foundHuman;

    // This private variable is a set which stores the words found by the computer player.
    Set<string> foundComputer;

    // This private variable is an integer which stores the player's score.
    int playerScore = 0;

    // This private variable is an integer which stores the computer's score.
    int computerScore = 0;

    // This function gets passed a string (can be empty) and uses it to initialize the boggle board.
    // If necessary (when a random board gets generated), sides of the cubes are chosen and then the
    // board gets shuffled. Otherwise, it fills the board in order from the characters in the string.
    void setUpBoard(string boardText);

    // This function is the recursive helper method used by human word search. It takes a starting
    // position (int row and int col) and a string, which represents the desired word. The function
    // checks the first letter against the current board space and then calls itself for all of the
    // neighboring spaces to search for the substring of word starting at index 1.
    bool checkForWord(int row, int col, string word);

    // This function is the recursive helper method used by the computer word search. It takes a board
    // location and a string representing the word currently being built. It also takes in a set as a
    // reference parameter to store the valid words it has found (which weren't found by the human player).
    // When the current string being built cannot possibly lead to any english words, it backtracks and
    // continues the search.
    void findWords(int row, int col, string& curr);

    // This private helper function takes in a word and increments the human player's score appropriately
    // based on the length of the word.
    void incrementPlayerScore(string word);

    // This private helper function takes in a word and increments the computer player's score appropriately
    // based on the length of the word.
    void incrementComputerScore(string word);

    // This function removes the highlights from all of the currently highlighted cubes by iterating over the
    // board and setting the highlight parameter to false on each space.
    void clearGUI();
};

// The boggle class overloads the << operator to facilitate the printing of the current state of the boggle game.
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
