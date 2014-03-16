// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "Boggle.h"
#include "grid.h"
#include "shuffle.h"
#include "set.h"
#include "stack.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

/*
 * Boggle constructor that creates the 4x4 boggle playing board. Human score is initially set to 0,
 */
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    gameDictionary = dictionary;
    hScore = 0;
    board.resize(4, 4);
    createBoard(boardText);
}

/*
 * Creates the board given a valid string of 16 characters representing letters for each of the 16 cubes.
 * Also updates the GUI with the given string.
 */
void Boggle::createBoard(string boardText){
    string boardStr = "";
    getBoardString(boardText, boardStr);
    BoggleGUI::labelAllCubes(toUpperCase(boardStr));
    fillBoard(boardStr);
}

/*
 * Separates each character of the string so that each index of the grid contains one letter.
 */
void Boggle::fillBoard(string boardStr){
    int index = 0;
    for(int r = 0; r < board.numRows(); r++) {
        for (int c = 0; c < board.numCols(); c++) {
            board[r][c] = boardStr[index];
            index++;
        }
    }
}

/*
 * Returns a valid 16 character string based on the value of boardText.
 * If boardText is empty, a board is randomly generated from the CUBES array.
 * If boardText is not empty, this method returns boardText.
 */
void Boggle::getBoardString(string boardText, string& boardStr) const{
    if(boardText == "") {
        shuffle(CUBES, 16);
        for(int i = 0; i < 16; i++) {
            string curCube = shuffle(CUBES[i]);
            boardStr += curCube[0];
        }
    } else {
        boardStr = boardText;
    }
}

/*
 * Returns the character stored at the given row/column on the board. If row or col are out of bounds, throws an int exception.
 */
char Boggle::getLetter(int row, int col) const{
    if(board.inBounds(row, col)) {
        return board[row][col];
    } else {
        throw row, col;
    }
}

/*
 * Checks whether the given word string is suitable to search for: that is in the dictionary, long enough to be a valid Boggle word and has not already been found.
 * Returns a boolean result of true if the word is suitable. Otherwise returns false.
 */
bool Boggle::checkWord(string word){
    return gameDictionary.contains(word) && word.length() > 3 && !foundWords.contains(word);
}

/*
 * Helper function for the humanWordSearch function.
 * A grid of booleans tracks whether a cell on the boggle board has already been used. Chosen represents how many letters
 * of the word have been found. Row and column are necessary for recursive calls so that the function knows which cell to start looking around.
 * The base case is when the original word is empty. This function returns true and exits as soon as the base case returns true. Otherwise, it
 * returns false.
 */
bool Boggle::humanWordSearchHelper(string word, Grid<bool>& isUsed, int row, int col, string chosen) const {
    if(word == "") {
        return true;
    } else {
        for(int i = row - 1; i < row + 2; i++) { 
            for (int j = col - 1; j < col + 2; j++){
                if(board.inBounds(i, j) && isUsed[i][j] != true){
                    BoggleGUI::setAnimationDelay(100);
                    BoggleGUI::labelCube(i, j, board[i][j], true);
                    if(board[i][j] != word[0]){
                        BoggleGUI::labelCube(i, j, board[i][j], false);
                    } else {
                        isUsed[i][j] = true;
                        string newWord = word.substr(1);
                        if(humanWordSearchHelper(newWord, isUsed, i, j, chosen += word.substr(0, 1))){
                            return true;
                        }
                        chosen.erase(chosen.length() - 1, 1);
                        isUsed[i][j] = false;
                        BoggleGUI::labelCube(i, j, board[i][j], false);
                    }
                }
            }
        }
        return false;
    }
}

/*
 * Performs a search on the board for an individual word, and returns a boolean result of whether the word can be formed.
 * Begins the search by looking at all the letters on the board. If a letter matches the first letter of the desired word, the humanWordSearch helper
 * function is called. If the helper function returns true, that means a word has been found. After looping throuhg all the letters, this means that
 * the word can not be formed on the board.
 */
bool Boggle::humanWordSearch(string word) {
    Grid<bool> isUsed(4, 4);
    if(checkWord(word)) {
        for(int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++){
                BoggleGUI::setAnimationDelay(100);
                BoggleGUI::labelCube(i, j, board[i][j], true);
                if(board[i][j] == word[0]){
                    if(humanWordSearchHelper(word, isUsed, i, j)) {
                        updateHumanData(word);
                        return true;
                    }
                } else {
                    BoggleGUI::labelCube(i, j, board[i][j], false);
                }
            }
        }
    }
    return false;
}

/*
 * Pre-condition: word is a valid word that can be formed on the board.
 * Updates the data associated with the human player by adding word to the set of words that have been found and
 * incrementing the human score based on the length of the word.
 */
void Boggle::updateHumanData(string word){
    foundWords.add(word);
    hScore += (word.length() - 3);
}

/*
 * Returns the set of words that have been found by the user.
 */
Set<string> Boggle::foundWordsSet(){
    return foundWords;
}

/*
 * Returns the score of the player.
 */
int Boggle::humanScore() const{
    return hScore;
}

/*
 * Helper function for the computerWordSearch function.
 * Checks every possible path given an initial row and column, a grid of used cells and a string of the currently chosen letters.
 * If the chosen does not exist as a prefix in the dictionary, we stop exploring that path and move on to the next path.
 * Otherwise if the computer hasn't already found chosen previously and the word is a valid word, then it will be added to the set of results.
 */
void Boggle::computerWordSearchHelper(int r, int c, Grid<bool> &isUsed, Set<string>& result, string chosen) {
    for(int i = r - 1; i < r + 2; i++){
        for(int j = c - 1; j < c + 2; j++){
            if(board.inBounds(i, j) && isUsed[i][j] != true) {
                isUsed[i][j] = true;
                string next = chosen + board[i][j] + "";
                if(gameDictionary.containsPrefix(chosen)){
                    if(!result.contains(chosen) && checkWord(chosen)){
                        result.add(chosen);
                    }
                    computerWordSearchHelper(i, j, isUsed, result, next);
                }
                isUsed[i][j] = false;
            }
        }
    }
}

/*
 * Performs a search on the board for all words that can be formed, returning them as a Set of strings.
 * It calls the recursive helper function for every letter on the board.
 */
Set<string> Boggle::computerWordSearch()  {
    Set<string> result;
    Grid<bool> isUsed(4, 4);
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            char chosen = board[i][j];
            string chosenStr = string(1, chosen);
            isUsed[i][j] = true;
            computerWordSearchHelper(i, j, isUsed, result, chosenStr);
            isUsed[i][j] = false;
        }
    }
    return result;
}

/*
 * Returns the total number of points the computer player has scored in teh game so far as an integer. Initially 0, but after a computer word search
 * is performed, all points for those words are added in to the computer's total.
 */
int Boggle::computerScore(){
    int cScore = 0;
    for(string word: computerWordSearch()){
        cScore += (word.length() - 3);
    }
    return cScore;
}

/*
 * Overrides the operator so that boggle objects can be printed out in a 4x4 grid.
 */
ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out << boggle.getLetter(i, j);
            if(j == 3) {
                out << endl;
            }
        }
    }
    return out;
}
