/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * Boggle.cpp file
 * defines the functions and variables to use in a Boggle class
 */

#include "Boggle.h"
#include "bogglegui.h"
#include "random.h"
#include "hashset.h"
#include "shuffle.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// length of one size of the board
const int BOARD_SIZE = 4;
// the minimum length of the word required
const int MIN_WORD_SIZE = 4;
// number of miliseconds of delay alotted per highlighting
const int ANIMATION_DELAY = 100;
// character to mark parts of the grid that have been used while searching
const char MARKER = '0';

/*
 * constructs the Boggle object
 * parameter: dictionary to look up words, text to determine the board
 * if the text is empty, generates a random board from given cubes
 * if the text has letters in it, it constructs it accordingly.
 * precondition: if the text has letters in it, it is already 16 characters
 * and all characters. This condition is checked in bogglePlay.cpp.
 */
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    dict = dictionary;
    board.resize(BOARD_SIZE, BOARD_SIZE);
    if (boardText.empty()) {
        string temp;
        // gets a random letter from each cube and adds it to grid
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
            int random = randomInteger(0,5);
            int rowNum = i / BOARD_SIZE;
            int colNum = i - BOARD_SIZE * (i / BOARD_SIZE);
            board[rowNum][colNum] = CUBES[i][random];
            temp += CUBES[i][random];
        }
        // randomly shuffles the board
        shuffle(board);
    }
    else {
        for (int i = 0; i < boardText.size(); i++) {
            int rowNum = i / BOARD_SIZE;
            int colNum = i - BOARD_SIZE * (i / BOARD_SIZE);
            board[rowNum][colNum] = boardText[i];
        }
    }
}

/*
 * parameter: row and column number
 * Returns the character stored at the given row/column on the board.
 * If row or col are out of bounds, throws an int exception.
 */
char Boggle::getLetter(const int row, const int col) const {
    if (row >= 0 && col >= 0 && row < BOARD_SIZE && col < BOARD_SIZE) {
        return board[row][col];
    }
    throw "The row or col are out of bounds!";
}

/*
 * checks if the given word is suitable under these conditions:
 * 1. in the dictionary, 2. at least MIN_WORD_SIZE long,
 * 3. if it hasn't been found yet
 * parameter: word to check
 * returns: true if word is suitable, false if word is not suitable.
 */
bool Boggle::checkWord(const string word) const {
    if (dict.contains(word)) {
        if (word.size() >= MIN_WORD_SIZE) {
            if (!foundWords.contains(word)) {
                return true;
            }
        }
    }
    return false;
}

/*
 * Returns true if the desired word can be formed on the board
 * returns false if the given word does not meet the folowing conditions:
 * 1. at least 4 letters long, 2. found in the dictionary,
 * 3. not found yet, 4. valid on the board
 * parameter: the desired word found by the human
 * precondition: this method assumes the word in the parameter consists of
 * all upper case letters. It will be satisfied in the boggleplay.cpp file.
 */
bool Boggle::humanWordSearch(string word, string& boardLetters) {
    // checks if the word is suitable
    if (!checkWord(word)) {
        return false;
    }
    BoggleGUI::labelAllCubes(boardLetters);
    // find all occurances of the first letter of the word to search from
    for (int c = 0; c < BOARD_SIZE; c++) {
        for (int r = 0; r < BOARD_SIZE; r++) {
            BoggleGUI::labelCube(r, c, board[r][c], true);
            BoggleGUI::setAnimationDelay(ANIMATION_DELAY);
            if (board[r][c] == word[0]) {
                Grid<char> temp = board;
                if (isValidOnBoard(temp, word, r, c)) {
                    foundWords.add(word);
                    humanScoreNum += word.size() - 3;
                    BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                    BoggleGUI::setScore(humanScoreNum, BoggleGUI::HUMAN);
                    return true;
                }
            }
            BoggleGUI::labelCube(r, c, board[r][c], false);
        }
    }
    return false;
}

/*
 * recursive backtracking helper method for humanWordSearch
 * parameters: temporary grid, word the human player desires to find,
 * the row number and column number.
 * it checks the base case if the current word to be checked is only a letter
 * returns true if word can be found on the board
 * returns false if word cannot be found on the board
 */
bool Boggle::isValidOnBoard(Grid<char>& temp, string word, int r, int c) const {
    // base case when word size is only 1
    if (word.size() == 1) {
        BoggleGUI::labelCube(r, c, board[r][c], true);
        return true;
    }
    // mark to not revisit same letter
    temp[r][c] = MARKER;
    for (int i = r - 1; i <= r + 1; i++) {
        for (int j = c - 1; j <= c + 1; j++) {
            if (isInBounds(i, j)) {
                if (temp[i][j] != MARKER) {
                    BoggleGUI::labelCube(i, j, board[i][j], true);
                    BoggleGUI::setAnimationDelay(ANIMATION_DELAY);
                    BoggleGUI::labelCube(i, j, board[i][j], false);
                    if (board[i][j] == word[1]) {
                        BoggleGUI::labelCube(i, j, board[i][j], true);
                        BoggleGUI::setAnimationDelay(ANIMATION_DELAY);
                        if (isValidOnBoard(temp, word.substr(1), i, j)) {
                            return true;
                        }
                        else {
                            BoggleGUI::labelCube(i, j, board[i][j], false);
                        }
                    }
                }
            }
        }
    }
    return false;
}

/*
 * checks to see if the given row and column values are in bounds
 * parameter: i = row number, j = column number
 * returns boolean: true if in bounds, false if out of bounds
 */
bool Boggle::isInBounds(const int i, const int j) const {
    if (i >= 0 && i < BOARD_SIZE) {
        if (j >= 0 && j < BOARD_SIZE) {
            return true;
        }
    }
    return false;
}

//Returns the human player's score as int
int Boggle::humanScore() const {
    return humanScoreNum;
}

/*
 * Returns a set of strings of all possible words that satisfies the conditions:
 * 1. word is in the dictionary, 2. word has not been found by the human player,
 * 3. word is at least 4 letters long, 4. word can be formed on the board
 * it utilizes the helper recursive backtracking method "findAllWordsStartingHere".
 * This will be the set of words the computer will find.
 */
Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Grid<char> temp = board;
            string word;
            word += board[r][c];
            findAllWordsStartingHere(word, temp, result, r, c);
        }
    }
    // iterates through each word computer found to display it
    for (string computerWord : result) {
        BoggleGUI::recordWord(computerWord, BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(computerScoreNum, BoggleGUI::COMPUTER);
    return result;
}

/*
 * recursive backtracking helper method for computerWordSearch
 * parameters: current word formed so far, temporary grid, set of strings as result,
 * the row number and column number.
 * it checks the base case if the current word formed is a valid word
 * that satisfies the conditions: 1. longer than 4 letters, 2. in the dictionary,
 * 3. hasn't been used yet, and 4. can be formed on the current board
 * returns true if any word can be found on the board
 * returns false if no word is found anywhere on the board
 */
bool Boggle::findAllWordsStartingHere
            (string& word, Grid<char>& temp, Set<string>& result, int r, int c) {
    // base case: if found a valid word
    if (checkWord(word)) {
        result.add(word);
        foundWords.add(word);
        computerScoreNum += word.size() - 3;
        if (findAllWordsStartingHere(word, temp, result, r, c)) {
            return true;
        }
    }
    temp[r][c] = MARKER;
    for (int i = r - 1; i <= r + 1; i++) {
        for (int j = c - 1; j <= c + 1; j++) {
            if (isInBounds(i, j) && temp[i][j] != MARKER) {
                word += temp[i][j];
                if (dict.containsPrefix(word)) {
                    if (findAllWordsStartingHere(word, temp, result, i, j)) {
                        return true;
                    }
                }
                temp[i][j] = board[i][j];
                word.erase(word.size()-1);
            }
        }
    }
    return false;
}

// Returns the computer's points as an int
int Boggle::getScoreComputer() const {
    return computerScoreNum;
}

// Returns the set of strings of found words so far
Set<string> Boggle::getFoundWords() const {
    return foundWords;
}

// overloads the << operator to print the board layout onto the console
ostream& operator<<(ostream& out, const Boggle& boggle) {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            out << boggle.getLetter(r, c);
        }
        out << endl;
    }
    return out;
}
