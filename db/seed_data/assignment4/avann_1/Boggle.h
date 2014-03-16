//This is the header file for the Boggle Class
//to be used to create the Boggle Game for CS106B.

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
    Boggle(Lexicon& dictionary, string boardText = "");
    Grid<char> getBoggleBoard(); //get Boggle Board
    HashSet<string> getHumanWords();
    void updateHumanWords(string word);
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    int humanScore();
    Set<string> computerWordSearch();
    int getScoreComputer();


private:
    Grid<char> boggleBoard;
    HashSet<string> humanWords;
    Lexicon dictionary;
    Set<string> result;
    bool humanWordSearchHelper(string word, Grid<bool>& marker, int row, int col);
    void computerWordSearchHelper(string current, Grid<bool>& marker, int row, int col, Set<string>& result);
    void unhighlight();
    void createRandomizedBoard();
    void createCharacterBoard(string boardText);
};

ostream& operator<<(ostream& out, Boggle& boggle);

#endif // _boggle_h
