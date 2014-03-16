/**
 * Name: Milan Doshi
 * Assignment: Boggle.cpp
 * TA: Jimmy Lee
 *
 * This file is implements all the methods that the Boggle class is made of. This includes all the necessary methods that had to be implemented
 * along with a few helper methods that help the flow of the program better.
 */

#include "Boggle.h"
#include "lexicon.h"
#include "random.h"
#include "vector.h"
#include "shuffle.h"
#include "bogglegui.h"

const int squares = 16;
const int rows = 4;
const int cols = 4;
// letters on all 6 sides of every cube
static string CUBES[squares] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    allWords = dictionary;
    remainingWords = dictionary;
    string cube;
    int n = 0;
    boggleboard.resize(rows,cols);

    if(boardText.empty()) {
        for(int i = 0; i < squares; i++ ) {
            cube = CUBES[i];
            boardText += cube[randomInteger(0,5)];
        }
        shuffleBoard(boardText);
        for(int r = 0; r < boggleboard.numRows(); r++) {
            for(int c = 0; c < boggleboard.numCols(); c++) {
                boggleboard[r][c] = boardText[n];
                n++;
            }
        }
    } else {
        for(int r = 0; r < boggleboard.numRows(); r++) {
            for(int c = 0; c < boggleboard.numCols(); c++) {
                boggleboard[r][c] = boardText[n];
                n++;
            }
        }
    }

    usableBoardText = boardText;
    allLetters = boardText;
    BoggleGUI::labelAllCubes(boardText);
}

void Boggle::shuffleBoard(string& boardText) {
    Vector<char> letters;
    for(int i = 0; i < boardText.size(); i++) {
        letters.add(boardText[i]);
    }
    boardText = "";
    shuffle(letters);
    for(int j = 0; j < letters.size(); j++) {
        boardText += letters.get(j);
    }
}

char Boggle::getLetter(int row, int col) {

    return boggleboard[row][col];
}

bool Boggle::checkWord(string word) {
    if(allWords.contains(word)) {
        if(word.length() >= 4) {
            if(remainingWords.contains(word)) {
                return true;
            }
        }
    }
}

bool Boggle::humanWordSearch(string word) {
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            BoggleGUI::labelCube(r,c,boggleboard[r][c],true);
            if(word[0] == boggleboard[r][c]) {
                BoggleGUI::labelCube(r,c,boggleboard[r][c],true);
                Grid<bool>booleanGrid;
                booleanGrid.resize(rows,cols);
                if(humanWordSearchHelp(word.substr(1), r, c, booleanGrid)) {
                    return true;
                }
            }
            BoggleGUI::labelCube(r,c,boggleboard[r][c],false);
        }
    }
    return false;
}

bool Boggle::humanWordSearchHelp(string word, int row, int col, Grid<bool>& booleanGrid) {
    if (booleanGrid[row][col]) return false;
    if(word.size() == 0) {
        return true;
    }
    BoggleGUI::labelCube(row,col,boggleboard[row][col],true);
    booleanGrid[row][col] = true;
    for(int r = row - 1; r < row + 2; r++) {
        for(int c = col - 1; c < col + 2; c++) {
            if(boggleboard.inBounds(r,c) == true) {
                if(word[0] == boggleboard[r][c]) {
                    BoggleGUI::labelCube(r,c,boggleboard[r][c],true);
                    if(humanWordSearchHelp(word.substr(1), r, c, booleanGrid)) return true;
                }
            }
        }
    }
    BoggleGUI::labelCube(row,col,boggleboard[row][col]);
    booleanGrid[row][col] = false;
    return false;
}

int Boggle::humanScore() {
    int score = 0;
    for(int i = 0; i < humanWords.size(); i++) {
        score += humanWords[i].size() - 3;
    }
    BoggleGUI::setScore(score,BoggleGUI::HUMAN);
    return score;
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            string word = "";
            Grid<bool> boolGrid;
            boolGrid.resize(rows, cols);
            computerWordSearchHelp(word, r, c, result, boolGrid);
        }
    }
    return result;
}
void Boggle::computerWordSearchHelp(string word, int row, int col, Set<string>& result, Grid<bool>& boolGrid) {
    if(!allWords.containsPrefix(word)) return;
    if (boolGrid[row][col]) return;
    if(checkWord(word)) {
        result.add(word);
        recordWord(word,BoggleGUI::COMPUTER);
    }
    boolGrid[row][col] = true;
    for(int r = row - 1; r < row + 2; r++) {
        for(int c = col - 1; c < col + 2; c++) {
            if(boggleboard.inBounds(r,c)) {
                char letter = boggleboard[r][c];
                string currentWord = word + letter;
                computerWordSearchHelp(currentWord, r, c, result, boolGrid);
            }
        }
    }
    boolGrid[row][col] = false;
}

int Boggle::getScoreComputer() {
    int score = 0;
    Set<string>result = computerWordSearch();
    int size = result.size();
    for(int i = 0; i < size; i++) {
        string first = result.first();
        score += first.size() - 3;
        result.remove(first);
    }
    BoggleGUI::setScore(score,BoggleGUI::COMPUTER);
    return score;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
           out << boggle.getLetter(r,c);
        }
        out << "" << endl;
    }
    return out;
}

void Boggle::removeWord(string word) {
    remainingWords.remove(word);
}

string Boggle::getBoardText() {
    return usableBoardText;
}
