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

    // TODO: add any other member functions/variables necessary
    //Grid<char> board(string choice);
    //bool checkAlpha(string userString);
    string toString();
    Grid<char> board;
    Lexicon dictionary=Lexicon("dictionary.txt");
    //Set<string> getUsedWords();
    Set<string> usedWords;
    //Set<string> getUsedWords(Boggle boggle);

private:
    // TODO: add any other member functions/variables necessary
    string getBoardChoice();
    Grid<char> randomBoard();
    Grid<char> userBoard(string boardText);
    string userString();
    Vector< Vector<int> > startPositions(char ch);
    bool findWord(Vector<int> position, string word, int counter);
    bool checkInBounds(int row, int col);
    Vector< Vector<int> > getNeighbors(Vector<int> position);
    void addNeighbors(Vector< Vector<int> >& neighbors, int row, int col);
    //Vector< Vector<int> > startPositions(char ch);
    Set<string> computerWords(Vector<int> cube, string prefix, Set<string>& foundWords, Grid<bool>& checks);
    Grid<bool> makeBlankCheck();
    //Set<string> foundWords;
    int humanPoints=0;
    int computerPoints=0;
    //Set<string> usedWords;

    char setLetter(string cube);

};
ostream& operator<<(ostream& out, Boggle& boggle);


#endif // _boggle_h
