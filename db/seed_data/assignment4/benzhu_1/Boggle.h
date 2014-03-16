//Name: Benzheng Zhu
//CS 106B Assignment 4
//Citation: lecture slides
//This is the Boggle class .h file, where the member variables and memeber functions are listed

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText);
    //constructor, takes a dictionary by reference, and a string
    //if the string is empty, it will generate 16 random letters
    //it initializes the member variables

    char getLetter(int row, int col) const;
    //it takes two integers, row and col, as inputs
    //and returns the corresponding letter on the boggle board

    bool checkWord(string &word) const;
    //takes a string as input
    //returns true if the string is a word that is at least 4-letter long and has not been found
    //returns false otherwise

    bool humanWordSearch(string word);
    //takes a string as input
    //returns true if the word can be formed on the board
    //returns false otherwise
    //updates human's list and score if true

    void computerWordSearch();
    //search for words that are at least 4-letter long and have not been found

    void printBoard() const;
    //prints the board

    int humanScore() const;
    //returns human's score

    int computerScore() const;
    //returns computer's score

    Set<string> computerList()const;
    //returns computer's word list

    Set<string> humanList() const;
    //returns human's word list

    string getText() const;
    //stores the letters on the board into a string
    //returns the string
    //used with labelAllCubes(str) to un-highlight letters

private:
    Grid<char> board; //represents the boggle board
    Lexicon D; //the dictionary. Initlizaing this a private member variables slows the program.

    Set<string> yourList; //a set of words found by the human
    Set<string> myList; //a set of words found by the computer
    int yourScore; //human's score
    int myScore; //computer's score

    bool humanHelper(Grid<char>& temp, string word, int row, int col);
    //takes a grid, a string and two integers as input
    //the grid is a copy of the original board, except that the elected letters are replaced with '-'
    //returns true if the string can be formed on the grid starting from the cell represented by the two integers
    //returns false otherwise

    void computerHelper(Grid<char>& temp, string s, int row, int col);
    //takes a grid, a string and two integers as input
    //the grid is a copy of the original board, except that the elected letters are replaced with '-'
    //returns true if the string can be formed on the grid starting from the cell represented by the two integers
    //returns false otherwise
    //updates computer's list and score whenever a new word is found
    //prunes dead-end searches by using Lexicon's .containsPrefix() function

};

ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
