/* Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * Program: Boggle.ccp
 * ------------------------------
 * The implementation of the Boggle class. For information
 * on the specific functions implemented here, go to the boggle.h file.
 */

#include "Boggle.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& inputDictionary, string boardText) {
    board.resize(NROWS,NCOL);
    dictionary = inputDictionary;
    BoggleGUI::initialize(NROWS,NCOL);
    if (boardText.length() != 0) {
        BoggleGUI::labelAllCubes(boardText);
    }
    if (boardText.length() != 0) {
        int counter = 0;
        for (int i = 0 ; i < board.numRows() ; i++) {
            for (int j = 0 ; j < board.numCols() ; j++) {
                board.set(i,j,boardText[counter]);
                counter++;
            }
        }
    } else {
        generateRandomBoard();
    }
    BoggleGUI::setAnimationDelay(PAUSE_TIME);
}

void Boggle::generateRandomBoard() {
    shuffle(CUBES,16);
    int counter = 0;
    for (int i = 0 ; i < NROWS ; i++) {
        for (int j = 0 ; j < NCOL ; j++) {
            string die = CUBES[counter];
            counter++;
            int randomPosition = randomInteger(0,die.length()-1);
            char value = die[randomPosition];
            board.set(i,j,value);
            BoggleGUI::labelCube(i,j,value);
        }
    }
}

char Boggle::getLetter(int row, int col) {
    return board.get(row,col);
}

bool Boggle::checkWord(string word) {
    return dictionary.contains(word) && word.length() >= 4 && !discoveredWords.contains(word);
}


bool Boggle::humanWordSearch(string word) {
    Grid <bool> usedDice(NROWS,NCOL);
    BoggleGUI::clearHighlighting();
    for (int i = 0 ; i < NROWS ; i++) {
        for (int j = 0 ; j < NCOL ; j++) {
            if (humanWordSearch(word,i,j,usedDice)) {
                humanPoints += (word.length() -3);
                discoveredWords.add(word);
                BoggleGUI::recordWord(word,BoggleGUI::HUMAN);
                return true;
            }
        }
    }
    BoggleGUI::clearHighlighting();
    return false;
}


bool Boggle::humanWordSearch(string word, int row, int col, Grid<bool>& usedDice) {
    if (row >= 0 && col >= 0 && row < 4 && col < 4) BoggleGUI::labelCube(row,col,board.get(row,col),true);
    if (!board.inBounds(row,col)) {
        return false;
    }
    if (board.get(row,col) != word[0] && !usedDice.get(row,col)) {
        if (row >= 0 && col >= 0 && row < 4 && col < 4) BoggleGUI::labelCube(row,col,board.get(row,col),false);
        return false;
    }
    if (usedDice.get(row,col)) {
        return false;
    }
    if (word.length() == 1 && board.get(row,col) == word[0]) {
        BoggleGUI::labelCube(row,col,board.get(row,col),true);
        return true;
    }
    usedDice.set(row,col,true);
    char temp = word[0];
    word.erase(0,1);
    for (int i = -1 ; i <= 1 ; i++) {
        for (int j = -1 ; j <= 1 ; j++) {
            if (humanWordSearch(word,row+i,col+j,usedDice)) {
                return true;
            }
        }
    }
    word = temp + word;
    usedDice.set(row,col,false);
    if (row >= 0 && col >= 0 && row < 4 && col < 4) BoggleGUI::labelCube(row,col,board.get(row,col),false);
    return false;
}

int Boggle::humanScore() {
    BoggleGUI::setScore(humanPoints,BoggleGUI::HUMAN);
    return humanPoints;
}

Set<string> Boggle::computerWordSearch() {
    BoggleGUI::clearHighlighting();
    Set<string> result;
    Grid<bool> usedDice(NROWS,NCOL);
    for (int i = 0 ; i < NROWS ; i++) {
        for (int j = 0 ; j < NCOL ; j++) {
            string word = "";
            computerWordSearch(i,j,result,usedDice,word);
        }
    } 
    return result;
}


bool Boggle::computerWordSearch(int row, int col, Set<string>& result, Grid<bool>& usedDice, string word) {
    if (!board.inBounds(row,col) || !dictionary.containsPrefix(word)) {
        return false;
    }
    if (usedDice.get(row,col)) {
        return false;
    }
    word += board.get(row,col);
    usedDice.set(row,col,true);
    bool success = false;
    for (int i = -1 ; i <= 1 ; i++) {
        for (int j = -1 ; j <= 1 ; j++) {
            if (computerWordSearch(row+i,col+j,result,usedDice,word)) {
                success = true;
            }
        }
    }
    word.erase(word.length()-1,1);
    usedDice.set(row,col,false);
    if (dictionary.contains(word) && !discoveredWords.contains(word) && word.length() >= 4) {
        result.add(word);
        discoveredWords.add(word);
        computerPoints += (word.length() -3);
        BoggleGUI::recordWord(word,BoggleGUI::COMPUTER);
        return true;
    }
    return success;
}

int Boggle::getScoreComputer() {
    BoggleGUI::setScore(computerPoints,BoggleGUI::COMPUTER);
    return computerPoints;
}

int Boggle::numRows() {
    return NROWS;
}

int Boggle::numCols() {
    return NCOL;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int i = 0 ; i < boggle.numRows() ; i++) {
        string line = "";
        for (int j = 0 ; j < boggle.numCols() ; j++) {
            line += boggle.getLetter(i,j);
        }
        out << line << endl;
    }
    return out;
}
