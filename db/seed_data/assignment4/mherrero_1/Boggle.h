// This is a .h file for the Boggle class. It has the functionality described in the assignment.
#ifndef _boggle_h
#define _boggle_h

#include "lexicon.h"
#include "grid.h"
using namespace std;
//header declaration for the
class Boggle {
public:
    /*
     * This is the header declaration for the constructor method for the Boggle class. First, it initializes the GUI board and sets the
     * GUI animation delay to 25 ms. Then, the method initializes the string that will store a string version
     * of the board's letters. Next, the method sets the human's and computer's scores to 0. The method stores
     * the passed dictionary lexicon into an internal dictionary lexicon object, and then it determines if
     * it needs to generate a board of random letters or use the letters passed in the string by the user.
     * It takes the appropriate action based on this determination.
     */
    Boggle(const Lexicon& dictionary, string boardText = "");
    /*
     * This is the header declaration for a method that returns the character in a specific row and column in the dice board. The values passed to
     * the method must be the index values in the grid.
     */
    char getLetter(const int row, const int col) const;
    /*
     * This is the header declaration for a method that checks if a word is a valid, scorable word, but does not check if the word is contained
     * in the board. This is determined based on if the word is 4 letters or more, if it has not been played
     * by a human player, and if it is a valid english word.
     */
    bool checkWord(string word) const;
    /*
     * This is the header declaration for a method that searches the board for a word inputed by the user. First, it declares a vector of bools
     * in which each index place refers to a particular place on the board and the bool value in that index
     * determines if that place in the board is selected or not (true for selected). Then, the GUI game board
     * is redeclared so that any highlighted places on the board are eliminated. Next, the method iterates
     * through each place on the Vector. Each place in the vector is "selected," which involves setting the
     * vector's value to true and highlighting the corresponding spot on the GUI game board. Next, the
     * recursive helper function is called to determine if that specific place on the board could be used as
     * the start of the word. If any call to the helper function results in finding the word (which we know
     * by the helper fucntion returning true), then the method returns true. Each call that does not result
     * in finding the word will "unselect" that cube and move to the next. If the word is never found, the
     * method returns false.
     */
    bool humanWordSearch(const string word);
    /*
     * This is the header declaration for a method that updates the human score on the GUI and returns the score as well.
     */
    int humanScore() const;
    /*
     * This is the header declaration for a method that recursively searches the game board to find all the valid english words it contains that
     * the user has not found. It resets the GUI board so that no cubes are highlighted and declares a vector
     * of bools that tracks the selected statuses of each place in the grid. It then iterates through the
     * vector and selects the corresponding place in the game board, calls the helper function that finds all
     * the words that starts from that place on the board, and then deselects the cube before moving to the
     * next. After this, the method returns the set containing the words the recursive method found.
     */
    Set<string> computerWordSearch();
    /*
     * This is the header declaration for a method that sets the computer player's score to the internally stored computer score. Then, the method
     * returns the compScore.
     */
    int getScoreComputer() const;
    /*
     * This is the header declaration for a method that returns a set of all the valid, scored words the human player has inputted.
     */
    Set<string> getHumanFoundWords() const;
    /*
     * This is the header declaration for a method that adds a word that the user has found. It does this by adding it to the GUI display, adding
     * it to the set of human words that have been found, and increases the human's score based on the length
     * of the word.
     */
    void addFoundWord(const string word);

private:
    //This is a Grid of char that contains the letters of the dice board
    Grid<char> diceBoard;
    //This is a lexicon object that is initialized to the english dictionary
    Lexicon dictionary;
    //This is a set of strings that stores every word the human player finds.
    Set<string> humanWords;
    //This is the int that stores the human player's score
    int humScore;
    //This is the int that stores the computer player's score
    int compScore;
    //this is a string that stores a string version of the letters on the game board
    string boardString;
    /*
     * This is the header declaration for a method that first shuffles all the string values in the CUBES array and then iterates through all the
     * values of the array. At each place in the array, a random character is chosen from the string contained
     * in the array. Then, this random character is added to the string that holds the board's characters and
     * the character is added to the corresponding place in the diceBoard grid. After this, the GUI is updated
     * using the full string of board characters.
     */
    void randomFillBoard();
    /*
     * This is a header declaration for a method that fills the diceBoard grid and GUI board based on the string inputted by the user.
     */
    void userFillBoard(string boardText);
    /*
     * This is the header declaration for a recursive helper function for the humanWordSearch method. This method is passed a row and
     * column index for a place on the board to check from, a word to search for, the Vector<bool> that stores
     * the "selected" statuses of grid's places, and the prefix of the word that has been built so far. If the
     * builtWord plus the letter at the row and column that has just been passed to the is the search word, the
     * method returns true. If this is not true, then the method checks if the builtWord plus the next letter
     * is a starting portion of the word. If this is not true, the method returns false. If neither of these
     * base cases are true, then the method iterates through all the cubes immediately surrounding the current
     * cube and, if it is in bounds, not equal to the current cube, and is not currently selected, then that
     * cube is "selected." Then, the method is recursively called to see if the built word plus the current
     * cube could lead to the word. If this is found to be true, then the method returns true. If not, the
     * cube is "deselected" and the next cube is looked at. If no cubes are found to work, the method returns
     * false.
     */
    bool humanWordSearchHelper(const int row, const int col, const string word, Vector<bool>& selectedSpots, string builtWord = "");
   /*
    * This is the header declaration for a method that recursively searches for every word on the game board that the user has not yet found. The
    * method first calls the checkWord method to see if the builtWord plus the current letter (based on the
    * row and column passed) is a playable letter. If so, the method increases the computer's score, records
    * the string in the GUI, and adds the word to the "result" set. If this is not true, the method
    * checks if the built word plus the current letter is the start of a valid english. If this is not true,
    * the method returns without searching any further (prunes the tree). If neither of these
    * base cases are true, then the method iterates through all the cubes immediately surrounding the current
    * cube and, if it is in bounds, not equal to the current cube, and is not currently selected, then that
    * cube is "selected." Then, the method is recursively called to find the words that start with the built
    * word plus the current letter. After these words are found, the current letter is "deselected" and the
    * method looks to the next place.
    */
    void computerWordSearchHelper(Set<string>& result, Vector<bool>& selectedSpots, const int row, const int col, string builtWord = "");
};

/*
 * This is the header declaration for the method that overrides the '<<' operator to print out a properly formatted version of the boggle board.
 * It iterates through each row and column and prints out each character in the grid. After all the
 * characters of a single row are printed, the print command transfers to the next line and prints the
 * next row.
 */
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
