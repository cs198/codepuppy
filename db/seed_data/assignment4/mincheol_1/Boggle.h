// Min Cheol Kim
// Section Leader Jimmy Lee
// Section Wed 5:15
/* This assignment simulated the word game Boggle, in which players try to form words on a 4x4 grid
 * using letters that are immediately adjacent to each other (diagonally, horizontally, and vertically).
 * This assignment involved a computer player who can find all the words recursively, and essentially wins
 * every single time.
 *
 * This specfic file is the Boggle class header file. */


#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "point.h"
#include "set.h"
using namespace std;

class Boggle {
public:
    /* The constructor takes in a dictionary and a string representing the letters that should be used to
     * form the game board. If an empty string is passed, a randomly generated board is used. */
    Boggle(Lexicon& dictionary, string boardText = "");

    /* This function returns the letter on the board at the given row and column, only if the row and col values
     * are valid numbers for a location on the grid */
    char getLetter(int row, int col) const;

    /* This function takes in a string and checks for:
     * 1. The word is longer than 3 letters.
     * 2. The word exists in a dictionary.
     * 3. The word has not been already added to the found list.
     * The function returns true if all three conditions are met, false in other cases. */
    bool checkWord(string word) const;

    /* This method checks if a valid word inputted by the human can be actually formed on the board.
     * The function utilizes recursive backtracking helper method and attempts to find the path leading
     * to the word. Returns true if the word can be formed, false if it cannot. */
    bool humanWordSearch(string word);

    /* Accessor method for the humanScore private variable, returns the score of the human. */
    int getHumanScore() const;

    /* Function returns a set of strings that contain all the words over four letters found in the dictionary that
     * can be formed on the board. This function calls the recursive helper function on every square of the board, while
     * resetting the marked/unmarked grid every time the helper function is called. */
    Set<string> computerWordSearch();

    /* returns the score of the computer. */
    int getScoreComputer() const;

    /* returns the game board character grid the game is played on. */
    Grid<char> getGameBoard() const;

    /* Accessor method for the private Set<string> containing the words already used by the human/computer. */
    Set<string> getAlreadyUsed() const;

    /* Adds a word to the private set of strings that contains the words that have already been found. */
    void addToAlreadyUsed(string word);

    /* Increases the score of the human appropriately, with the score being the number of letters longer or equal to 4. */
    void increaseHumanScore(string word);

private:
    Grid<char> gameBoard;
    int humanScore;
    int computerScore;
    Set<string> alreadyUsed;
    Lexicon dict;
    Grid<bool> markedCubes;

    /* Takes in a point and checks if the point is in bound of the gameBoard. Returns true if yes, false if no. */
    bool isOutside (Point p) const;

    /* returns the value of the Grid<bool> markedCubes at a given point. */
    bool isMarked(Point p) const;

    /* Changes the value of the Grid<bool> markedCubes to "true" at the given point. */
    void markSquare(Point p);

    /* Changes the value of the Grid<bool> markedCubes to "false" at the given point. */
    void unmarkSquare (Point p);

    /* This function is the recursive helpder function for the humanWordSearch function, taking in the current
     * string, the target word, and the current position point. The function returns false and stops recursing
     * if the location is outside the grid, the location is already marked, or the string formed by
     * adding the current character is not part of the target word. The function returns true and stops
     * recursing if the word formed by adding the current character to the string is the target word. Otherwise,
     * the function calls on itself at the eight squares surrounding the current square. */
    bool humanWordSearchHelper(string current, string word, Point pos);

    /* This is the helper function for finding all the valid words starting from a given point and saving the words to
     * a set of strings as you go. The function takes in a string current, current position point, and a reference to
     * a set of strings. It implements recursive backtracking; the function returns and stops recursing if the position
     * is outside the grid, is already marked, or the dictionary doesn't have any words that start with the current string.
     * The function then calls on itself eight times in all adjacent directions. */
    void computerWordSearchHelper(string current, Point pos, Set<string>& fromThisPos);

    /* Returns a new point adjacent to the inputted current point in the direction direction in which:
     * 1 = Top Left
     * 2 = Top
     * 3 = Top Right
     * 4 = Left
     * 5 = Right
     * 6 = Bottom Left
     * 7 = Bottom
     * 8 = Bottom Right */
    Point adjacentCube(Point current, int dir) const;

    /* Resets the boolean grid markedCubes to all false, since this represents the game state where no cubes
     * have been visited yet. */
    void resetGameBoard();

    /* Increases the score of the computer appropriately, with the score being the number of letters longer or equal to 4. */
    void increaseComputerScore(string word);

    /* Sets up the Boggle board with manual input, with the first character of the string representing the letter at
     * (0,0) and so forth. */
    void manualBoardSetup(string scheme);

    /* Sets up the Boggle board with random letters given from the 16 dies, with the dies in random positions and the
     * letter in each of the dice chosen randomly. */
    void randomBoardSetup();

    /* Sets up the scores, assign a value to the lexicon private variable dict, resizes the board char grid to the
     * appropriate size, and sets up the markedCubes boolean grid to all false (unvisited). */
    void initialSetup(Lexicon& dictionary);
};

/* Overloads the << operator, correctly prints out the game board of the Boggle class. */
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
