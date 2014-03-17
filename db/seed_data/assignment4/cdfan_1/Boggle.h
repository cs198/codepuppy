// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "hashset.h"
using namespace std;

class Boggle {
public:
    /*
     *Constructor: Boggle:
     * usage: creates a Boggle object
     * A Boggle object encapsulate:
     *Lexicon lexicon;
     *Grid<char> board;
     *Set<string> used;
     *int score;
     *Set<string> computerResult;
     */
    Boggle(Lexicon& dictionary, string boardText = "");

    /*
     *Member function:
     * Usage:It takes in int row and int col as the location on the grid, and returns the char letter
     * in that location
     */
    char getLetter(int row, int col) const;

    /*
     *Member function:
     * Usage: It returns a string that is the text on the Grid<char> board. This function is constant, will not
     * be able to change the object when it's called.
     */
    string getBoardText() const;

    /*
     *Member function:
     *Usage: It takes a string word as a parameter, returns a boolean value indication whether this word
     * has at least the length of WORD_MIN_LENGTH, whether this word is used already by the user, and whether
     * this word is a legit English word
     */
    bool checkWord(string word);

    /*
     *Member function:
     * Usage: It takes in a string word as a parameter, conducts recursive serach on teh board to see whether
     * this word can be found on the board, and returns a boolean value. If it can be found, return true;
     * if not, return false.
     */
    bool humanWordSearch(string word);

    /*
     *Memebr function:
     * Usage: It returns the human score. This is a constant function, and can not modify the object.
     */
    int humanScore() const;


    /*
     *Member function:
     * Usage: It conduct computer search for valid English words on the board in a exhansting manner.
     * ONly returns the Set<string> of the all found words minus the human found words. It returns the remainder
     * of all words that are not found by human.
     */
    Set<string> computerWordSearch();

    /*
     *Member function:
     * Usage: It returns calculates the score and return it. It cannot modify the object.
     */
    int getScoreComputer() const;

    /*
     *Member function:
     * Usage: It returns the Grid<char> board. This is a constant function and cannot change the object
     */
    Grid<char> getBoard() const;

    /*
     * Member function:
     *Usage: It returns a Set<string> that contains all the words found and scored by human. It cannot modify
     * the object.
     */
    Set<string> getScoredWords() const;

    /*
     *Member function:
     * Usage: it has a string word as parameter. Add this word into the Set<string> of human Score words.
     */
    void addScoredWord(string word);

    /*
     *Member function:
     * Usage: it takes a int gain parameter. Gain is the score that is earned by the human entering a correct
     * word. It adds gain onto the total score of human by far.
     */
    void addHumanScore(int gain);

private:
    /*
     *Instance variables/ memeber fields:
     */
    Lexicon lexicon;
    Grid<char> board;
    Set<string> used;
    int score;
    Set<string> computerResult;

    //private helper functions

    /*
     *Private member function:
     * it generages a random text from the cubes given in the constant array. returns the text randomly generated
     */
    string randomBoard();

    /*
     *Private member function:
     * It is a helper function for recursion in human search. It has parameters:
     * Grid<char>& copyBoard: a grid with a char  in each cell
     * string word: the word that is about be searched.
     * int x: the row coordinate in the grid on which the searching current position is
     * int y: the col coordinate in the grid on which the searching current position is
     * It returns a boolean to inform whether thie word is found.
     */
    bool humanWordSearchHelper(Grid<char>& copyBoard, string word, int x, int y);

    /*
     *Private member function:
     * It is a helper funciton for recursion in computer search. It has parameters:
     * Grid<char> & copyBoard:  a grid with a char  in each cell
     * int row: the row coordinate in the grid on which the searching current position is
     * int col: the col coordinate in the grid on which the searching current position is
     * string word: The half finished word string the recursion is searching to fill up into a real word
     * Set<string> & result: the set that once a word is found is added to.
     */
    void computerWordSearchHelper(Grid<char> & copyBoard, int row, int col, string word, Set<string> & result);
};

/*
 *Operator overload:
 * It enables the operator "<<" in order to be able to print a boggle object, essentially the board of the
 * object.
 * It takes two parrameters, on its left is a ostream, on its right is a boggle object.
 * It returns the ostream
 */
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
