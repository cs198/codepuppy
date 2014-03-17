// Boogle.cpp implements the Boggle class behavior declared in Boggle.h
// See ArrayList.h for documentation of each member

#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "bogglegui.h"
#include "error.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    gameBoard.resize(4,4);
    if (boardText == "") {
        shuffle(CUBES,16);
        int count = 0;
        for (int i=0; i<gameBoard.numRows(); i++) {
            for (int j=0; j<gameBoard.numCols(); j++) {
                gameBoard[i][j] = CUBES[count][randomInteger(0,5)];
                count++;
            }
        }
    } else {
        for (int i=0; i<gameBoard.numRows(); i++) {
            for (int j=0; j<gameBoard.numCols(); j++) {
                gameBoard[i][j] = boardText[4*i+j];
            }
        }
    }
    dict = dictionary;
}

char Boggle::getLetter(int row, int col) const {
    return gameBoard[row][col];
}

bool Boggle::checkWord(string word) {
    return (word.size()>=4 && dict.contains(word) && !uniqueWords.contains(word));
}

bool Boggle::humanWordSearch(string word) {
    if (checkWord(word)) {
        // recurse to find word
        for (int row=0; row<gameBoard.numRows(); row++) {
            for (int col=0; col<gameBoard.numCols(); col++) {
                if (getLetter(row,col) == word[0]) {
                    char first = gameBoard[row][col];
                    BoggleGUI::labelCube(row, col, first, true);
                    gameBoard.set(row, col, '/');
                    //recursion
                    if (humanWordSearchHelper(word.substr(1), gameBoard, row, col)) {
                        humSum += (word.size() - 3);
                        uniqueWords.add(word);
                        gameBoard.set(row, col, first);
                        return true;
                    }
                    gameBoard.set(row, col, first);
                }
            }
        }
        return false;
    }
    return false;
}

bool Boggle::humanWordSearchHelper(string word, Grid<char> gameBoard, int row, int col) {
    if (word == "") return true;
    for (int i=row-1; i<=row+1; i++) {
        for (int j=col-1; j<=col+1; j++) {
            if (gameBoard.inBounds(i,j) && (i != row || j != col) && getLetter(i,j) == word[0] && gameBoard[i][j] != '/') {
                BoggleGUI::labelCube(i,j,gameBoard[i][j],true);
                gameBoard.set(i, j, '/');
                if (humanWordSearchHelper(word.substr(1), gameBoard, i, j)) return true;
            }
        }
    }
    return false;
}

int Boggle::getHumanScore() const {
    return humSum;
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    for (int row=0; row<gameBoard.numRows(); row++) {
        for (int col=0; col<gameBoard.numCols(); col++) {
            string word = "";
            computerWordSearchHelper(gameBoard, result, word, row, col);
        }
    }
    // takes out all the words in the human set and updates computer score accordingly
    for (string sameWords:uniqueWords) {
        result.remove(sameWords);
        compSum -= (sameWords.size()-3);
    }
    return result;
}

void Boggle::computerWordSearchHelper(Grid<char> gameBoard, Set<string>& result, string word, int row, int col) {
    for (int i=row-1; i<=row+1; i++) {
        for (int j=col-1; j<=col+1; j++) {
            if (gameBoard.inBounds(i,j) && (i != row || j != col) && gameBoard[i][j] != '/') {
                word += gameBoard[i][j];
                if (dict.containsPrefix(word)) {
                    char lastLetter = gameBoard[i][j];
                    gameBoard.set(i, j, '/');
                    if (dict.contains(word) && word.size() >= 4) {
                        if (!result.contains(word)) compSum += (word.size() - 3);
                            result.add(word);
                    }
                    computerWordSearchHelper(gameBoard, result, word, i, j);
                    gameBoard.set(i, j, lastLetter);
                }
                word.erase(word.size()-1,1);
            }
        }
    }
}

int Boggle::getScoreComputer() const {
    return compSum;
}

Set<string> Boggle::getSet() const {
    return uniqueWords;
}

void Boggle::resetWithWords(string boardText) {
    if (boardText.size() != 16) throw "Entered string is not 16 chars long";
    for (int i=0; i<gameBoard.numRows(); i++) {
        for (int j=0; j<gameBoard.numCols(); j++) {
            gameBoard[i][j] = boardText[4*i+j];
        }
    }
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int r=0; r<4; r++) {
        for (int c=0; c<4; c++) {
            out << boggle.getLetter(r,c);
        }
        out << endl;
    }
    return out;
}
