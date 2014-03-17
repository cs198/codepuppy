/*
 * This is the file that controls the game
 * of Boggle. This controls the gameplay
 * and the main functions it needs to
 * do all the commands.
*/

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "bogglegui.h"
#include "grid.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    int humanScore();
    Set<string> computerWordSearch();
    int getScoreComputer();
    Set<string> usedWords;
    Set<string> result;


//private functions
private:
    Grid<char> board; //the game board
    Lexicon dict;    //the dictionary
    //these two are helper functions that help determine what
    bool isOnBoard(string word, int row, int col, Grid<char>& board, Grid<bool> grid);
    bool isOnBoardForComputer(string word, int row, int col, Grid<char>& board, Grid<bool> grid, Lexicon & dict, Set<string> & result);
};

ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
