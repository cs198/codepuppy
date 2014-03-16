// This is the Boggle Game cpp file, which implements all
// of the Boggle Game functionality, like searching for
// words for both the human user and the computer and
// calculating the score and interacting with the GUI.

#include "Boggle.h"
#include "grid.h"
#include "vector.h"
#include "shuffle.h"
#include "strlib.h"
#include "bogglegui.h"

//static const int

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

/* The Boggle constructor takes in a dictionary and string called
 * boardText inputted by the user in boggleplay.cpp. If the boardText
 * is empty, it generates a random arrangement of characters from the
 * Boggle CUBES provided. If not, it puts the 16 characters into the
 * Boggle board.
 */
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    this -> dictionary = dictionary;

    boardText = toUpperCase(boardText);

    if(boardText==""){ //create random board
        createRandomizedBoard();
    }else{ //use the 16 character string to make the grid
        createCharacterBoard(boardText);
    }
}

/* createRandomizedBoard creates a randomized Boggle Board
 * using the CUBES array provided. It highlights and animates
 * the board as it creates it.
 */
void Boggle::createRandomizedBoard(){
    Vector < Vector<char> > cubes;

    for(int i=0; i<16; i++){
        string cubecharacters = CUBES[i];
        Vector<char> cube;
        for(int j=0; j<6; j++){
            cube.add(cubecharacters[j]);
        }
        cubes.add(cube);
    }

    shuffle(cubes);
    Vector<char> boardLetters;
    boggleBoard.resize(4,4);

    for(Vector<char> cube: cubes){
        int randomletter = randomInteger(0,5);
        char letter = cube[randomletter];
        boardLetters.add(letter);
    }

    int boardletterindex = 0;

    for (int r = 0; r < boggleBoard.numRows(); r++) {
        for (int c = 0; c < boggleBoard.numCols(); c++) {
            boggleBoard[r][c] = boardLetters[boardletterindex];
            BoggleGUI::labelCube(r, c, boardLetters[boardletterindex], true);
            BoggleGUI::setAnimationDelay(100);
            BoggleGUI::labelCube(r, c, boardLetters[boardletterindex], false);
            boardletterindex++;
        }
    }
}

/* createCharacterBoard creates the Boggle Board when the user decides
 * not to create a randomized Board. It highlights and animates the board
 * as it creates it.
 */
void Boggle::createCharacterBoard(string boardText){
    int boardtextindex = 0;
    boggleBoard.resize(4,4);

    for (int r = 0; r < boggleBoard.numRows(); r++) {
        for (int c = 0; c < boggleBoard.numCols(); c++) {
            boggleBoard[r][c] = boardText[boardtextindex];
            BoggleGUI::labelCube(r, c, boardText[boardtextindex], true);
            BoggleGUI::setAnimationDelay(100);
            BoggleGUI::labelCube(r, c,boardText[boardtextindex], false);
            boardtextindex++;
        }
    }
}

/* getBoggleBoard returns the boggleBoard object.
 */
Grid<char> Boggle::getBoggleBoard(){
    return boggleBoard;
}

/* getHumanWords returns the list of valid words the user has created
 * throughout the Boggle game.
 */
HashSet<string> Boggle::getHumanWords(){
    return humanWords;
}

/* updateHumanWords adds a new valid word to the humanWords HashSet
 * of valid words the user has created throughout the Boggle game.
 */
void Boggle::updateHumanWords(string word){
    humanWords.add(word);
}

/* getLetter first ensures that the requested row and column are
 * in bounds. If they are not, it throws an error. Otherwise, it
 * return the character at the requested row and column of the
 * Boggle board.
 */
char Boggle::getLetter(int row, int col) {
    if(row > (boggleBoard.numRows()-1) ||row < 0){
        throw "row";
    }else if(col > (boggleBoard.numCols()-1) || col <0){
        throw "col";
    }else{
        return boggleBoard[row][col];
    }
}

/* checkWords checks that a word passed to it is of the proper length,
 * is in the dictionary, and is not in the list of humanWords already
 * found by the user. If these conditions are satisfied, it returns
 * true. If any of these are not met, it returns false.
 */
bool Boggle::checkWord(string word) {
    if(word.length()<4){
        return false;
    }else if(!dictionary.contains(toLowerCase(word))){
        return false;
    }else if(humanWords.contains(word)){
        return false;
    }else{
        return true;
    }
}

/* humanWordSearch is responsible for finding if a word the user inputs
 * is a valid word on the Boggle board. It iterates over the full Boggle board,
 * and if a character matched the first character of the word, it marks this
 * character and calls the helper to explore the surrounding characters. It
 * returns true if the word is found. Otherwise, it unmarks the character,
 * eventually returning false if the word is not found.
 */
bool Boggle::humanWordSearch(string word) {
    Grid<bool> marker;
    marker.resize(4,4);

    for (int r = 0; r < boggleBoard.numRows(); r++) {
        for (int c = 0; c < boggleBoard.numCols(); c++) {
            unhighlight();
            if(getLetter(r, c)==word[0]){
                marker[r][c] = true;
                BoggleGUI::labelCube(r, c, boggleBoard[r][c], true);
                BoggleGUI::setAnimationDelay(100);
                bool present = humanWordSearchHelper(word.substr(1), marker, r, c);
                if(present){
                    return true;
                }
                marker[r][c] = false;
                BoggleGUI::labelCube(r, c, boggleBoard[r][c], false);
                BoggleGUI::setAnimationDelay(100);

            }
        }
    }

    return false;
}

/* humanWordSearchHelper checks the surrounding characters of the character
 * selected in humanWordSearch. It determines if a surrounding character matches
 * the next character in the word. If it does, it continues following this path.
 * If not, it continues searching neighboring characters. If it finds the word,
 * it returns true. If it does not, it returns false.
 */
bool Boggle::humanWordSearchHelper(string word, Grid<bool>& marker, int row, int col){
    if(word.length()==0){
        return true;
    }
    for(int r=row-1; r  <=row+1; r++){
        for(int c=col-1; c<=col+1; c++){
            if(boggleBoard.inBounds(r,c) && marker[r][c]==false){
                if(getLetter(r,c)==word[0]){
                    marker[r][c] = true;
                    BoggleGUI::labelCube(r, c, boggleBoard[r][c], true);
                    BoggleGUI::setAnimationDelay(100);
                    if(humanWordSearchHelper(word.substr(1), marker, r, c)){
                        return true;
                    }
                    marker[r][c] = false;
                    BoggleGUI::labelCube(r, c, boggleBoard[r][c], false);
                    BoggleGUI::setAnimationDelay(100);
                }
            }
        }
    }
    return false;
}

/* unhighlight is used to unhighlight words in the GUI that have been found,
 * so that they do not remain permanently highlighted.
 */
void Boggle::unhighlight(){
    for (int r = 0; r < boggleBoard.numRows(); r++) {
        for (int c = 0; c < boggleBoard.numCols(); c++) {
            BoggleGUI::labelCube(r,c,boggleBoard[r][c], false);
            BoggleGUI::setAnimationDelay(100);
        }
    }
}

/* humanScore iterates over the list of words the user has found stored in
 * humanWords and uses the length of each of these words to tally a score.
 */
int Boggle::humanScore() {
    int score=0;
    for(string word: humanWords){
        score += word.length()-3;
    }
    return score;
}

/* computerWordSearch finds all valid words on the Boggle board. It iterates
 * over each character on the board, marks the present character, and explores
 * all the possible words that can start with this character on the board using
 * its helper computerWordSearchHelper, which adds the results set. It then
 * unmakrs the present character. Once all the characters have been iterated over,
 * it returns the result set.
 */
Set<string> Boggle::computerWordSearch() {
    Grid<bool> marker;
    marker.resize(4,4);

    for (int r = 0; r < boggleBoard.numRows(); r++) {
        for (int c = 0; c < boggleBoard.numCols(); c++) {
            marker[r][c] = true;
            string letter = string() + getLetter(r,c);
            computerWordSearchHelper(letter, marker, r, c, result);
            marker[r][c] = false;
        }
    }
    return result;
}

/* computerWordSearchHelper checks all the neighboring characters of the current,
 * marked character. It attempts to add the neighboring characters to the existing
 * current string. If this forms a valid prefix in the dictionary lexicon, it continues
 * searching the neightboring characters, adding all valid current words to the results
 * set. If not, it removes the invalid neighboring character from current and continues
 * checking the neighboring chacters.
 */
void Boggle::computerWordSearchHelper(string current, Grid<bool>& marker, int row, int col, Set<string>& result){
    if(dictionary.contains(toLowerCase(current))){
        if(!result.contains(current)){
            if(current.length()>=4){
                result += current;
                BoggleGUI::recordWord(current, BoggleGUI::COMPUTER);
            }
        }
    }

    for(int r=row-1; r<=row+1; r++){
        for(int c=col-1; c<=col+1; c++){
            if(boggleBoard.inBounds(r,c) && marker[r][c]==false){
                current += getLetter(r,c);
                if(dictionary.containsPrefix(toLowerCase(current))){
                    marker[r][c] = true;
                    computerWordSearchHelper(current, marker, r, c, result);
                    marker[r][c] = false;
                }
                current = current.substr(0, current.length()-1);
            }
        }
    }
}

/* getScoreComputer calculates the score of the computer by iterating over the
 * results set created by computerWordSearch. It adds to the score based on the
 * length of each word found by the computer and returns this score.
 */
int Boggle::getScoreComputer() {
    int score=0;
    for(string word: result){
        score += word.length()-3;
    }
    return score;
}

/* The operator prints out each character of the boggleBoard at
 * each row and column position.
 */
ostream& operator<<(ostream& out, Boggle& boggle) {
    Grid<char> boggleBoard = boggle.getBoggleBoard();
    for (int r = 0; r < boggleBoard.numRows(); r++) {
        for (int c = 0; c < boggleBoard.numCols(); c++) {
            out << boggleBoard[r][c];
        }
        out << endl;
    }
    return out;
}
