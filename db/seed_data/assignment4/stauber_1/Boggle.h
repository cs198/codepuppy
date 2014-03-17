//Filename: Boggle.h
//Author: Mark Stauber
//Date: 2/13/14
//Description: Contains the interface for the Boggle class
//that represents a single game of boggle.
#ifndef _boggle_h
#define _boggle_h

#include <iostream>

#include "strlib.h"
#include "lexicon.h"
#include "set.h"

#include "bogglegui.h"

using namespace std;

class Boggle {
public:
    //Constructor sets up game and GUI.
    Boggle(Lexicon& dictionary, string boardText = "");

    //Checks if row, col is on board and returns that letter. Throws
    //integer exception if row, col is not on board. Assumes a 4x4 board.
    char getLetter(int row, int col);

    //Checks if user's word is 4 letters, conatined in dictionary, and not found yet.
    bool checkWord(string word);

    //Searches for user's word on the board by calling
    //humanSearchHelper for every spot on board.
    bool humanWordSearch(string word);

    //Returns user score.
    int humanScore();

    //Searches board for all valid words and returns them in a Set
    //by using the computerSearchHelper function on every spot on board
    Set<string> computerWordSearch();

    //Returns computer's score
    int getScoreComputer();

    //Returns the game state. True = human turn, False = computer turn (game end).
    bool gameOn();

    //Set the gameState to game end.
    void endGame();

    //Print out the letters on the board.
    void printBoard();

    //Returns the set of humanWords.
    Set<string> getHumanWords();

    //Returns a statement declaring the result of the game.
    string gameResult();


private:
    //Contains letters on Boggle board
    string board;

    //Dictionary to check for words
    Lexicon dictionary;

    //If gameState = true then human turn, if gameState = false then computer turn.
    bool gameState;

    //Set of words found by user
    Set<string> humanWords;

    //Human's points
    int humanPoints;

    //Computer's points
    int computerPoints;

    //Recursive function to check if user word is contained on board.
    bool humanSearchHelper(string word, int row, int col, Set<int>& markedSpots);

    //Recursive function to find all words on board and add them to result.
    void computerSearchHelper(string word, int row, int col,
                              Set<int>& markedSpots, Set<string>& result);

};

//Operator that prints out the Boggle game status.
ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
