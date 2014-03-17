/* Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * File: Boggle.h
 * ----------------------------------
 * The interface for the Boggle class. This class creates a
 * Boggle object that can be used to play the game Boggle. It
 * provides functionality for one human player to play the game
 * against a computer player that will find all of the words that
 * the human cannot find. In addition, it includes a GUI to represent
 * the board, the players' scores, and the words that each player has
 * found.
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "shuffle.h"
#include "random.h"
#include "strlib.h"
#include "hashset.h"
#include "bogglegui.h"
using namespace std;

class Boggle {
public:
    //Returns the number of rows or number of columns on the Boggle board.
    int numRows();
    int numCols();

    /* The constructor for a Boggle object. This takes in the dictionary as a parameter
     * and, if included, a string of what the user wishes to set the board to.
     */
    Boggle(Lexicon& dictionary, string boardText = "");

    //Returns the letter at that row and column in the Boggle board.
    char getLetter(int row, int col);

    //Checks if the word entered by the player is in the dictionary, is long enough, and hasn't been discovered already.
    bool checkWord(string word);

    /* Searches for the word that the user entered on the board and returns true
     * if it is able to find the word. If it finds the word, it also gives the human points
     * for the word and marks down that the word was discovered.
     */
    bool humanWordSearch(string word);

    //Returns the human player's score
    int humanScore();

    /* This function searches through the Boggle board using recursive backtracking and finds
     * every word that was not found by the human player. It stores all of these words
     * in a Set which is returned by the function. It uses a private helper function.
     */
    Set<string> computerWordSearch();

    //Returns the computer player's score
    int getScoreComputer();



    
private:
    //The pause time between highlighting letters in the GUI.
    const int PAUSE_TIME = 50;

    //The number of rows and number of columns on the board.
    const static int NROWS = 4;
    const static int NCOL = 4;

    //The grid that is used to store the values of the board.
    Grid<char> board;

    //Private function that is used to create a random Boggle board using an array of cubes
    //stored in the Boggle.cpp file.
    void generateRandomBoard();

    //Integer values for the number of points scored by both players
    int humanPoints = 0;
    int computerPoints = 0;

    //The set of words that have already been discovered.
    Set<string> discoveredWords;

    //The lexicon that contains the English dictionary.
    Lexicon dictionary;

    /* The helper function to humanWordSearch that includes the additional parameter of a HashSet of
     * Vectors that keeps track of which dice have been used already. This ensures that
     * no die can be used twice to create a word. This method uses recursive backtracking to search
     * through the board and find the desired. If it cannot find the word, it returns false.
     */
    bool humanWordSearch(string word, int row, int col, Grid<bool>& usedDice);

    /* This helper function for computerWordSearch includes more parameters. These parameters include:
     * the row and column numbers that are currently being looked at, the Set that contains the different found
     * words, a HashSet of Vectors that stores which dice have already been used to form that word, and the
     * current word being formed. The function is designed to immediately stop checking a branch off of
     * a certain combination of letters if there is no word in English with that prefix.
     */
    bool computerWordSearch(int row, int col, Set<string>& result, Grid<bool> &usedDice, string word);
};
        
//Overloads the << operator so that it can be used in boggleplay.cpp to print out
//board to the console.
        ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
