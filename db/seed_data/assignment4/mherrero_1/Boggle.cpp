// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "Boggle.h"
#include "grid.h"
#include "shuffle.h"
#include "random.h"
#include "simpio.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

/*
 * This is the constructor method for the Boggle class. First, it initializes the GUI board and sets the
 * GUI animation delay to 25 ms. Then, the method initializes the string that will store a string version
 * of the board's letters. Next, the method sets the human's and computer's scores to 0. The method stores
 * the passed dictionary lexicon into an internal dictionary lexicon object, and then it determines if
 * it needs to generate a board of random letters or use the letters passed in the string by the user.
 * It takes the appropriate action based on this determination.
 */
Boggle::Boggle(const Lexicon& dictionary, string boardText) {
    BoggleGUI::initialize(4,4);
    BoggleGUI::setAnimationDelay(25);
    boardString = "";
    diceBoard.resize(4,4);
    humScore = 0;
    compScore = 0;
    this->dictionary = dictionary;
    if(boardText=="") {
        randomFillBoard();
    }else{
        userFillBoard(boardText);
    }
}

/*
 * This method first shuffles all the string values in the CUBES array and then iterates through all the
 * values of the array. At each place in the array, a random character is chosen from the string contained
 * in the array. Then, this random character is added to the string that holds the board's characters and
 * the character is added to the corresponding place in the diceBoard grid. After this, the GUI is updated
 * using the full string of board characters.
 */
void Boggle::randomFillBoard() {
    shuffle(CUBES, 16);
    for(int i = 0; i < 16; i++) {
        string possibleLetters = CUBES[i];
        char nextLetter = possibleLetters[randomInteger(0,5)];
        boardString += nextLetter;
        diceBoard.set(i/4, i%4, nextLetter);
    }
    BoggleGUI::labelAllCubes(boardString);
}

/*
 * This method fills the diceBoard grid and GUI board based on the string inputted by the user.
 */
void Boggle::userFillBoard(string boardText) {
    boardString = boardText;
    BoggleGUI::labelAllCubes(boardText);
    for(int i = 0; i < 16; i++) {
        diceBoard.set(i/4, i%4, boardText[i]);
    }
}

/*
 * This method returns a set of all the valid, scored words the human player has inputted.
 */
Set<string> Boggle::getHumanFoundWords() const {
    return humanWords;
}

/*
 * This method returns the character in a specific row and column in the dice board. The values passed to
 * the method must be the index values in the grid.
 */
char Boggle::getLetter(const int row, const int col) const {
    return diceBoard.get(row, col);
}

/*
 * This method checks if a word is a valid, scorable word, but does not check if the word is contained
 * in the board. This is determined based on if the word is 4 letters or more, if it has not been played
 * by a human player, and if it is a valid english word.
 */
bool Boggle::checkWord(string word) const {
    if(word.length() < 4 || humanWords.contains(word) || !dictionary.contains(word)) return false;
    return true;
}

/*
 * This method adds a word that the user has found. It does this by adding it to the GUI display, adding
 * it to the set of human words that have been found, and increases the human's score based on the length
 * of the word.
 */
void Boggle::addFoundWord(const string word){
    BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
    humanWords.add(word);
    humScore += word.size()-3;
}

/*
 * This method searches the board for a word inputed by the user. First, it declares a vector of bools
 * in which each index place refers to a particular place on the board and the bool value in that index
 * determines if that place in the board is selected or not (true for selected). Then, the GUI game board
 * is redeclared so that any highlighted places on the board are eliminated. Next, the method iterates
 * through each place on the Vector. Each place in the vector is "selected," which involves setting the
 * vector's value to true and highlighting the corresponding spot on the GUI game board. Next, the
 * recursive helper function is called to determine if that specific place on the board could be used as
 * the start of the word. If any call to the helper function results in finding the word (which we know
 * by the helper fucntion returning true), then the method returns true. Each call that does not result
 * in finding the word will "unselect" that cube and move to the next. If the word is never found, the
 * method returns false.
 */
bool Boggle::humanWordSearch(const string word) {
    Vector<bool> selectedSpots(16, false);
    BoggleGUI::labelAllCubes(boardString);
    for(int i = 0; i < 16; i++) {
        selectedSpots.set(i, true);
        BoggleGUI::labelCube(i/4, i%4, getLetter(i/4, i%4), true);
        if(humanWordSearchHelper(i/4, i%4, word, selectedSpots)){
            return true;
        }
        BoggleGUI::labelCube(i/4, i%4, getLetter(i/4, i%4), false);
        selectedSpots.set(i,false);
    }
    return false;
}

/*
 * This is the recursive helper function for the humanWordSearch method. This method is passed a row and
 * column index for a place on the board to check from, a word to search for, the Vector<bool> that stores
 * the "selected" statuses of grid's places, and the prefix of the word that has been built so far. If the
 * builtWord plus the letter at the row and column that has just been passed to the is the search word, the
 * method returns true. If this is not true, then the method checks if the builtWord plus the next letter
 * is a starting portion of the word. If this is not true, the method returns false. If neither of these
 * base cases are true, then the method iterates through all the cubes immediately surrounding the current
 * cube and, if it is in bounds, not equal to the current cube, and is not currently selected, then that
 * cube is "selected." Then, the method is recursively called to see if the built word plus the current
 * cube could lead to the word. If this is found to be true, then the method returns true. If not, the
 * cube is "deselected" and the next cube is looked at. If no cubes are found to work, the method returns
 * false.
 */
bool Boggle::humanWordSearchHelper(const int row, const int col, const string word, Vector<bool>& selectedSpots, string builtWord) {
    if(builtWord+getLetter(row, col) == word) {
        return true;
    }
    if(!startsWith(word, builtWord+getLetter(row, col))) {
        return false;
    }
    for(int iRow = row-1; iRow < row+2; iRow++) {
        for(int iCol = col-1; iCol < col+2; iCol++) {
            if(diceBoard.inBounds(iRow, iCol) && (iRow*4)+iCol != (row*4)+col &&
                    !selectedSpots.get((iRow*4)+iCol)){
                selectedSpots.set((iRow*4) + iCol, true);
                BoggleGUI::labelCube(iRow, iCol, getLetter(iRow, iCol), true);
                if(humanWordSearchHelper(iRow, iCol, word, selectedSpots, builtWord+getLetter(row, col))) {
                    return true;
                }
                BoggleGUI::labelCube(iRow, iCol, getLetter(iRow, iCol), false);
                selectedSpots.set((iRow*4) + iCol, false);
            }
        }
    }
    return false;
}

/*
 * This method updates the human score on the GUI and returns the score as well.
 */
int Boggle::humanScore() const{
    BoggleGUI::setScore(humScore, BoggleGUI::HUMAN);
    return humScore;
}


/*
 * if the built word is not the start of an actual word
 *      return (end this tree branch)
 * else if checkword returns true
 *      add the word to the set
 * for the current, one-before, and one-after rows
 *      for the current, one-before, and one-after columns
 *          select that place on the grid
 *          call the helper function
 *          deselect that place on the grid
 *      }
 * }
 *
 *
 * create a new bool Vector
 * for each place in the grid
 *      select that place on the grid
 *      call the helper function
 *      deselect that place on the grid
 *
 * This method recursively searches the game board to find all the valid english words it contains that
 * the user has not found. It resets the GUI board so that no cubes are highlighted
 *
 */
Set<string> Boggle::computerWordSearch(){
    Set<string> result;
    BoggleGUI::labelAllCubes(boardString);
    Vector<bool> selectedSpots(16, false);
    for(int i = 0; i < 16; i++) {
        selectedSpots.set(i, true);
        computerWordSearchHelper(result, selectedSpots, i/4, i%4);
        selectedSpots.set(i, false);
    }
    return result;
}

void Boggle::computerWordSearchHelper(Set<string>& result, Vector<bool>& selectedSpots, int row, int col,
                                      string builtWord){
    string checkString = builtWord+getLetter(row, col);
    if(checkWord(checkString)) {
        compScore += checkString.length() - 3;
        BoggleGUI::recordWord(checkString, BoggleGUI::COMPUTER);
        result.add(checkString);
    } else if(!dictionary.containsPrefix(builtWord)) {
        return;
    }
    for(int iRow = row-1; iRow < row+2; iRow++) {
        for(int iCol = col-1; iCol < col+2; iCol++) {
            if(diceBoard.inBounds(iRow, iCol) && (iRow*4)+iCol != (row*4)+col &&
                    !selectedSpots.get((iRow*4)+iCol)) {
                selectedSpots.set((iRow*4)+iCol, true);
                computerWordSearchHelper(result,selectedSpots,iRow, iCol, checkString);
                selectedSpots.set((iRow*4)+iCol, false);
            }
        }
    }

}

/*
 * This method sets the computer player's score to the internally stored computer score. Then, the method
 * returns the compScore.
 */
int Boggle::getScoreComputer() const {
    BoggleGUI::setScore(compScore, BoggleGUI::COMPUTER);
    return compScore;
}

/*
 * This method overrides the '<<' operator to print out a properly formatted version of the boggle board.
 * It iterates through each row and column and prints out each character in the grid. After all the
 * characters of a single row are printed, the print command transfers to the next line and prints the
 * next row.
 */
ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int iRow = 0; iRow < 4; iRow++) {
        for(int iCol = 0; iCol < 4; iCol++) {
            out << boggle.getLetter(iRow, iCol);
        }
        out << endl;
    }
    return out;
}
