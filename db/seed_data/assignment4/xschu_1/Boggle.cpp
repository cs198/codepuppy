// Xinsheng Chu
// This is the cpp file to be internally used in the boggle program.

#include "Boggle.h"
#include "grid.h"
#include "random.h"
#include "shuffle.h"
#include "set.h"
#include "vector.h"
#include "console.h"
#include "bogglegui.h"
// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

/*
 * Constructor.
 * Build the board randomly if the input stream is empty.
 * Otherwise, it get the user input to call functions to build the grid.
 */
Boggle::Boggle(const Lexicon& dictionary, const string boardText) {
    dict = dictionary;
    board.resize(4,4);
    if (boardText == "") {
        buildRandomBoard();
    } else {
        buildInputBoard(boardText);
    }
}

/*
 * This method build the random board, and store it in the private instance grid.
 * It generates 16 random word from 16 dies in a random order.
 */
void Boggle::buildRandomBoard() {
    shuffle(CUBES, 16);
    int cubeNum = 0;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            string oneCube = CUBES[cubeNum];
            board[row][col] = oneCube[randomInteger(0, 5)];
            cubeLetters += board[row][col];
            cubeNum++;
        }
    }
    BoggleGUI::labelAllCubes(cubeLetters);
}

/*
 * This method build the board based on the user input, and store it in the private instance grid.
 * It generates 16 words based on the string input.
 */
void Boggle::buildInputBoard(const string text){
    int index = 0;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            board[row][col] = text[index];
            index++;
        }
    }
    cubeLetters = text;
    BoggleGUI::labelAllCubes(cubeLetters);
}

/*
 * This function returns one letter at the input row and column.
 */
char Boggle::getLetter (const int row, const int col) const {
    // TODO: implement
    string letter = board[row][col];
    return letter[0];   // remove this
}

/*
 * This method checks if the user input word is valid.
 * If the word lenght is less than four, or the word has already been recorded, or the word is not found in the dictionary.
 * It returns false.
 */
bool Boggle::checkWord(const string word) const{
    if (word.size() < 4) return false;
    for (int i = 0; i < humanSearchResult.size(); i++) {
        if (humanSearchResult[i] == toUpperCase(word)) return false;
    }
    if (!dict.contains(toLowerCase(word))) return false;
    return true;
}

/*
 * This function finds if the first letter of the word is found in the grid.
 * If not, it returns false.
 * It yes, it calls the helper function, and return the results if the word is found in the board.
 */
bool Boggle::humanWordFindable(const string word){
    int index = 0;
    for (int row = 0; row < 4; row ++) {
        for (int col = 0; col < 4; col++) {
            if (board[row][col] == word.substr(0,1)) {
                Grid<string> tempBoard = board;
                tempBoard[row][col] = "?";
                if (humanWordFindableHelper(tempBoard, word, index + 1, row, col)) {
                    BoggleGUI::labelCube(row, col, word[0], true);
                    return true;
                }
            }
        }
    }
    return false;
}

/*
 * This is the helper function to find a human input word, using recursive backtracking.
 * The base case is the complete word is found in the board, it adds the word to the set and updates the score.
 * It changes the neighbour word to "?", and explore that word is the exact same as the user input word, in a recursive way.
 */
bool Boggle::humanWordFindableHelper(Grid<string>& tempBoard, const string word, int index, int row, int col){
    if (index == word.size()) { // base case
        humanSearchResult.add(word);
        BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
        hScore += word.size() - 3;
        BoggleGUI::setScore(hScore, BoggleGUI::HUMAN);
        return true;
    } else {
        for (int r = row - 1; r <= row + 1; r++) {
            for (int c = col - 1; c <= col + 1; c++) {
                if (tempBoard.inBounds(r, c)) {
                    if (tempBoard[r][c] == word.substr(index, 1)) {
                        string temp = tempBoard[r][c];
                        BoggleGUI::labelCube(r, c, temp[0], true);
                        tempBoard[r][c] = "?"; // do
                        if (humanWordFindableHelper(tempBoard, word, index + 1, r, c)) {
                            return true; // recursion
                        }
                        BoggleGUI::labelCube(r, c, temp[0], false);
                        tempBoard[r][c] = temp; // undo
                    }
                }
            }
        }
        return false;
    }
}

/*
 * This function checks if the user input word is valid, if not it returns false.
 * If valid, it return the result if the word is found in the board.
 */
bool Boggle::humanWordSearch(const string word) {
    BoggleGUI::labelAllCubes(cubeLetters);
    if (!checkWord(word)) return false;
    else {
        return humanWordFindable(toUpperCase(word));
    }
}

/*
 * This function return the current score of user.
 */
int Boggle::humanScore() const {
    return hScore;
}

/*
 * This function return the all the words human has found in a vector of string.
 */
Vector<string> Boggle::humanWords() const {
    return humanSearchResult;
}

/*
 * This is the helper function to find all valid words on the board, using recursive backtracking.
 * If part of the word is the prefix found in the dictionary, it checks if it is a valid word.
 * If yes, it adds the word to the set and updates score.
 * If not it check the neighbour letter, using recursive backtracking.
 */
void Boggle::computerWordSearchHelper(string word, int row, int col, Grid<string>& tempBoard) {
    if (dict.containsPrefix(word)) {
        if (dict.contains(word) && checkWord(word) && !computerSearchResult.contains(word)) {
            cScore += word.size() - 3;
            BoggleGUI::setScore(cScore, BoggleGUI::COMPUTER);
            computerSearchResult.add(word);
            BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
        }
        for (int r = row - 1; r <= row + 1; r++) {
            for (int c = col - 1; c <= col + 1; c++) {
                if (tempBoard.inBounds(r, c)) {
                    string temp = tempBoard[r][c]; // do
                    if (temp != "?") {
                        word += temp;
                        tempBoard[r][c] = "?";
                        computerWordSearchHelper(word, r, c, tempBoard); // recursion
                        tempBoard[r][c] = temp;
                        word = word.substr(0, word.size() - 1); // undo
                    }
                }
            }
        }
    }
}

/*
 * This function call the helper function to find all the valid words in the board.
 * And return the set of all valid words.
 */
Set<string> Boggle::computerWordSearch(){
    string word = "";
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            Grid<string> tempBoard = board;
            word = tempBoard[row][col];
            tempBoard[row][col] = "?";
            computerWordSearchHelper(word, row, col, tempBoard);
        }
    }
    return computerSearchResult;
}

/*
 * This function return the current score of computer.
 */
int Boggle::getScoreComputer() const {
    return cScore;
}

/*
 * This function is the operator overload.
 * When cout << Boggole is called, it prints the 4*4 grid on console.
 */
ostream& operator<<(ostream& out, const Boggle& boggle) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            out << boggle.getLetter(row, col);
        }
        out << endl;
    }
    return out;
}



