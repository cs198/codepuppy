// This is a .cpp file which contains the bodies of the functions contained in the
// boggle class. It contains a constructor, a pair of recursive searching functions,
// and several getter functions to return private values to the user. It also overloads
// the << operator to facilitate the printing of the current state of the boggle game to
// the console.

#include "Boggle.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    setUpBoard(boardText);
    validLexicon = dictionary;
    indicesChecked.resize(4, 4);
}

void Boggle::setUpBoard(string boardText) {
    board.resize(4, 4);
    bool shuffleBoard = false;
    boardText = toUpperCase(boardText);
    if (boardText == "") { // Need to generate random board.
        shuffleBoard = true; // Will shuffle grid after generating characters.
        for (int i = 0; i < 16; i++) {
            string sides = CUBES[i];
            int random = randomInteger(0, 5);
            boardText += sides[random]; // Build a version of boardText with randomly generated chars.
        }
    }
    for (int i = 0; i < 4; i ++) { // Put the characters into the grid (board).
        for (int j = 0; j < 4; j++) {
            board[i][j] = boardText[0];
            boardText = boardText.substr(1);
        }
    }
    if (shuffleBoard) { // Shuffle the board if necessary.
        shuffle(board);
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            BoggleGUI::labelCube(i, j, board[i][j], false); // Iterate over grid to fill in GUI board (after the board has been shuffled).
        }
    }
}

char Boggle::getLetter(int row, int col) {
    return board[row][col];
}

int Boggle::getHumanWords() {
    return foundHuman.size();
}

bool Boggle::checkWord(string word) {
    if (word.length() < 4) {
        return false;
    }
    if (!validLexicon.contains(word)) {
        return false;
    }
    if (foundHuman.contains(word)) {
        return false;
    }
    return true;
}

bool Boggle::humanWordSearch(string word) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            indicesChecked[i][j] = false;
        }
    }
    clearGUI();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            indicesChecked[i][j] = true;
            BoggleGUI::labelCube(i, j, getLetter(i, j), true); // Highlight the current cube.
            if (checkForWord(i, j, word)) { // Helper method
                incrementPlayerScore(word);
                foundHuman.add(word);
                BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                return true;
            } else {
                indicesChecked[i][j] = false;
                BoggleGUI::labelCube(i, j, getLetter(i, j), false); // Unhighlight for backtracking.
            }
        }
    }
    return false;
}

void Boggle::incrementPlayerScore(string word) {
    playerScore += word.length() - 3;
    BoggleGUI::setScore(playerScore, BoggleGUI::HUMAN);
}

/* Base case is word of length 1 to avoid bug where last letter of word is
 * surrounded by indices that have already been checked, wherein the last recursive
 * call never gets made.
 */

bool Boggle::checkForWord(int row, int col, string word) {
    if (word.length() == 1 && word[0] == getLetter(row, col)) {
        indicesChecked[row][col] = false;
        return true;
    } else {
        if (board[row][col] != word[0]) {
            return false;
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int rowIndex = row - 1 + i;
                int colIndex = col - 1 + j;
                if (board.inBounds(rowIndex, colIndex)) {
                    if (!indicesChecked[rowIndex][colIndex]) {
                        indicesChecked[rowIndex][colIndex] = true;
                        BoggleGUI::labelCube(rowIndex, colIndex, getLetter(rowIndex, colIndex), true);
                        if (checkForWord(rowIndex, colIndex, word.substr(1))) {
                            return true;
                        } else {
                            indicesChecked[rowIndex][colIndex] = false;
                            BoggleGUI::labelCube(rowIndex, colIndex, getLetter(rowIndex, colIndex), false);
                        }
                    }
                }
            }
        }
        indicesChecked[row][col] = false;
    }
    return false;
}

void Boggle::clearGUI() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            BoggleGUI::labelCube(i, j, getLetter(i, j), false);
        }
    }
}

int Boggle::humanScore() {
    return playerScore;   // remove this
}

string Boggle::playerWords() {
    return foundHuman.toString();
}

Set<string> Boggle::computerWordSearch() {
    clearGUI();
    string curr;
    Set <string> foundComputer;
    for (int i = 0; i < 4; i++) { //Reset indicesChecked after humanWordSearch.
        for (int j = 0; j < 4; j++) {
            indicesChecked[i][j] = false;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            indicesChecked[i][j] = true;
            curr = "";
            curr += board[i][j];
            findWords(i, j, curr); // Helper method
            indicesChecked[i][j] = false;
        }
    }
    return foundComputer;
}

// Recursive helper method for computer word search.

void Boggle::findWords(int row, int col, string& curr) {
    if (validLexicon.contains(curr) && curr.length() >= 4) { // Valid word of requisite length
        if (!foundHuman.contains(curr)) // Not found by the human player
            foundComputer.add(curr);
            BoggleGUI::recordWord(curr, BoggleGUI::COMPUTER);
            incrementComputerScore(curr);
    }
    if (validLexicon.containsPrefix(curr)) { // Base case is prefix that doesn't match any words.
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int rowIndex = row - 1 + i;
                int colIndex = col - 1 + j;
                if (board.inBounds(rowIndex, colIndex)) {
                    if (!indicesChecked[rowIndex][colIndex]) {
                        indicesChecked[rowIndex][colIndex] = true;
                        curr += board[rowIndex][colIndex];
                        findWords(rowIndex, colIndex, curr);
                        curr = curr.substr(0, curr.length() - 1);
                        indicesChecked[rowIndex][colIndex] = false;
                    }
                }
            }
        }
    }
}

void Boggle::incrementComputerScore(string word) {
    computerScore += word.length() - 3;
    BoggleGUI::setScore(computerScore, BoggleGUI::COMPUTER);
}

int Boggle::getScoreComputer() {
    return computerScore;
}

int Boggle::getComputerWords() {
    return foundComputer.size();
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out << boggle.getLetter(i, j);
        }
        out << endl;
    }
    out << endl;
    out << "Your words (" << boggle.getHumanWords() << "): " << boggle.playerWords() << endl;
    out << "Your score: " << integerToString(boggle.humanScore()) << endl;
    return out;
}
