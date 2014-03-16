// This is the cpp file for the class that plays
// the game of Boggle. Using the different methods
// provided here, a user has all of the means to
// play the game against the computer and ultimately
// lose.

#include <string>
#include "Boggle.h"
#include "grid.h"
#include "random.h"
#include "strlib.h"
#include "bogglegui.h"


// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
/**
 * @brief Boggle::Boggle
 * @param dictionary
 * @param boardText
 * This method takes care of all initialization required for the board and
 * GUI.
 */
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    if(!BoggleGUI::isInitialized()){
        BoggleGUI::initialize(4, 4);
    }
    BoggleGUI::reset();
    createBoard();
    string str;
    for(int i = 0; i < 4; i++){
        for(int z = 0; z < 4; z++){
            str += board[i][z];
        }
    }
    BoggleGUI::labelAllCubes(str);

}
/**
 * @brief Boggle::createBoard
 * createBoard fills the board with random letters from the die.
 */
void Boggle::createBoard(){
    board.resize(4, 4);
    int counter = 0;
    for(int i = 0; i < 4; i++){
        for(int z = 0; z < 4; z++){
            int randomInt = randomInteger(0, 5);
            string str = CUBES[counter].substr(randomInt, 1);
            board[i][z] = str;
            counter++;
        }
    }
    shuffle(board);
}

/**
 * @brief Boggle::getLetter
 * @param row
 * @param col
 * @return
 * getLetter returns a char of the letter
 * that is at the row and col passed in.
 */
char Boggle::getLetter(int row, int col) {
    if(row > 3 || row < 0 || col > 3 || col < 0)throw 0;
    return stringToChar(board[row][col]);
}
/**
 * @brief Boggle::checkWord
 * @param word
 * @param dictionary
 * @return
 * checkWord checks the dicitionary to see if the word
 * passed in is a word. It also checks word length and if it
 * has already been used.
 */
bool Boggle::checkWord(string word, Lexicon& dictionary) {
    if(dictionary.contains(word)){
        if(word.length() > 3 && !foundByPlayer.contains(word)){
            return true;
        }
    }
    BoggleGUI::setStatusMessage("Invalid entry.");
    return false;
}
/**
 * @brief Boggle::humanWordSearchHelper
 * @param word
 * @param row
 * @param column
 * @param usedCheck
 * @return
 * this is the helper method for humanWordSearch(). It does the actual
 * recursion while checking to see if word is a viable choice for
 * the board.
 */
bool Boggle::humanWordSearchHelper(string word, int row, int column, Grid<bool>& usedCheck) {
    if(word.length() > 0){
        for(int i = -1; i < 2; i++){
            for(int z = -1; z < 2; z++){
                if(board.inBounds(row + i, column + z)){
                    string firstLetter = word.substr(0, 1);
                    BoggleGUI::labelCube(row + i, column + z, stringToChar(board[row + i][column + z]), true);
                    BoggleGUI::setAnimationDelay(30);
                    if(firstLetter == board[row + i][column + z]){
                        if(usedCheck[row + i][column + z] != false){
                            usedCheck[row + i][column + z] = false;
                            string restLetters = word.substr(1);
                            if(humanWordSearchHelper(restLetters, row + i, column + z, usedCheck) == true){
                                return true;
                            }
                            usedCheck[row + i][column + z] = true;
                            BoggleGUI::labelCube(row + i, column + z, stringToChar(board[row + i][column + z]),  false);

                        }else {
                            BoggleGUI::labelCube(row + i, column + z, stringToChar(board[row + i][column + z]),  false);
                        }
                    }
                    if(usedCheck[row + i][column + z] != false){
                        BoggleGUI::labelCube(row + i, column + z, stringToChar(board[row + i][column + z]),  false);
                    }
                }
            }
        }
        return false;
    }else {
        return true;
    }
}
/**
 * @brief Boggle::humanWordSearch
 * @param word
 * @return
 * humanWordSearch takes in a word and checks to see
 * if that word is valid in the game of boggle by utilizing
 * a helper method.
 */
bool Boggle::humanWordSearch(string word) {
        BoggleGUI::clearHighlighting();
        Grid<bool> usedCheck(4, 4);
        for(int i = 0; i < 4; i++){
            for(int z = 0; z < 4; z++){
                usedCheck.set(i, z, true);
            }
        }
        for(int i = 0; i < 4; i++){
            for(int z = 0; z < 4; z++){
                string firstLetter = word.substr(0, 1);
                if(firstLetter == board[i][z]){
                    usedCheck[i][z] = false;
                    BoggleGUI::labelCube(i, z, stringToChar(firstLetter),  true);
                    BoggleGUI::setAnimationDelay(30);
                    string restLetters = word.substr(1);
                    if(humanWordSearchHelper(restLetters, i, z, usedCheck) == true){
                        foundByPlayer += word;
                        BoggleGUI::setStatusMessage("You Found a new word! \"" + word + "\"");
                        BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                        return true;
                    }else{
                        BoggleGUI::labelCube(i, z, stringToChar(firstLetter),  false);
                        usedCheck[i][z] = true;
                    }
                }
            }
        }
        return false;
}
/**
 * @brief Boggle::humanScore
 * @return
 * humanScore() returns the current score of the player.
 */
int Boggle::humanScore() {
    int score = 0;
    for(string word: foundByPlayer){
        score += (word.length() - 3);
    }
    BoggleGUI::setScore(score, BoggleGUI::HUMAN);
    return score;
}

/**
 * @brief Boggle::computerWordSearch
 * @param dictionary
 * @return
 * computerWordSearch() checks all possible options
 * to find all words that exist within the boggle
 * board.
 */
Set<string> Boggle::computerWordSearch(Lexicon& dictionary) {
    BoggleGUI::clearHighlighting();
    Set<string> result;
    Grid<bool> usedCheck(4, 4);
    for(int i = 0; i < 4; i++){
        for(int z = 0; z < 4; z++){
            usedCheck.set(i, z, true);
        }
    }
    for(int i = 0; i < 4; i++){
        for(int z = 0; z < 4; z++){
            usedCheck.set(i, z, false);
            computerWordSearchHelper(result, board[i][z], i, z, dictionary, usedCheck);
            usedCheck.set(i, z, true);
        }
    }
    return result;
}
/**
 * @brief Boggle::computerWordSearchHelper
 * @param result
 * @param word
 * @param row
 * @param column
 * @param dictionary
 * @param usedCheck
 * computerWordSearchHelper() is the helper method
 * for computerWordSearch. It does the actual recursion.
 */
void Boggle::computerWordSearchHelper(Set<string>& result, string word,
                                      int row, int column, Lexicon& dictionary, Grid<bool>& usedCheck){
    if(dictionary.contains(word) && word.length() > 3 && !foundByPlayer.contains(word)){
        result += word;
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }
    if(dictionary.containsPrefix(word)){
        for(int i = -1; i < 2; i++){
            for(int z = -1; z < 2; z++){
                if(board.inBounds(row + i, column + z)){
                    if(usedCheck[row + i][column + z] != false){
                        usedCheck[row + i][column + z] = false;
                        string restLetters = word + board[row + i][column + z];
                        computerWordSearchHelper(result, restLetters, row + i, column + z, dictionary, usedCheck);
                        usedCheck[row + i][column + z] = true;
                    }
                }
            }
        }
    }
}
/**
 * @brief Boggle::getScoreComputer
 * @param foundByComputer
 * @return
 * getScorecomputer() returns the score of the set of strings
 * passed into it.
 */
int Boggle::getScoreComputer(Set<string> foundByComputer) {
    int score = 0;
    for(string word: foundByComputer){
        score += (word.length() - 3);
    }
    BoggleGUI::setScore(score, BoggleGUI::COMPUTER);
    BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    return score;
}

/**
 * @brief operator <<
 * @param out
 * @param boggle
 * @return
 * operator << prints out what the boggle game board looks like.
 */
ostream& operator<<(ostream& out, Boggle& boggle) {
    string line = boggle.board[0][0] + boggle.board[0][1] + boggle.board[0][2] + boggle.board[0][3];
    cout << line << endl;
    line = boggle.board[1][0] + boggle.board[1][1] + boggle.board[1][2] + boggle.board[1][3];
    cout << line << endl;
    line = boggle.board[2][0] + boggle.board[2][1] + boggle.board[2][2] + boggle.board[2][3];
    cout << line << endl;
    line = boggle.board[3][0] + boggle.board[3][1] + boggle.board[3][2] + boggle.board[3][3];
    cout << line << endl;
    return out;
}
