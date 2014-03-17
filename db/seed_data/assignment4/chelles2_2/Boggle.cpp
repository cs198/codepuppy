/* This cpp file includes the definitions needed for implementation of the Boggle class.
A Boggle object represents the current board and state for a single Boggle game,
and the following member functions perform most of the major game functions*/

#include "Boggle.h"
#include "shuffle.h"
#include "grid.h"
#include "bogglegui.h"
#include "error.h"
#include "strlib.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

/*Constructs a new Boggle board that uses the given dictionary lexicon to look up
words, and uses the given 16-letter string to initialize the 16 board cubes from top-left
to bottom-right. If the string is empty, generates a random shuffled board.*/
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    dict = dictionary;
    board.resize(BOARD_SIZE, BOARD_SIZE);
    //If string is empty, generates a random shuffled board
    if (boardText == "") {
        shuffle(CUBES, NUM_SQUARES);
        for (int i = 0; i < NUM_SQUARES; i++) {
            int ch = randomInteger(0, 5);
            int row = i/BOARD_SIZE;
            int col = i%BOARD_SIZE;
            board.set(row, col, CUBES[i][ch]);
        }
    } else {
        //Use the given 16-letter string to initialize the cubes
        int boardTextLength = boardText.length();
        if (boardTextLength == NUM_SQUARES) {
            for (int i = 0; i < boardTextLength; i++) {
                int row = i/BOARD_SIZE;
                int col = i%BOARD_SIZE;
                board.set(row, col, boardText[i]);
            }
        } else {
            //Throw an error if the string is not made of 16 letters
            throw("Number of characters is not equal to board size");
        }
   }
}

/* This function prints the Boggle board on the GUI*/
void Boggle::displayGrid() {
    BoggleGUI::initialize(BOARD_SIZE, BOARD_SIZE);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            BoggleGUI::labelCube(i, j, board.get(i,j), false);
        }
    }
}

/* This function returns the character in the specified row and column of the board*/
char Boggle::getLetter(int row, int col) {
    if (!board.inBounds(row, col)) {
        throw 0; //int exception if out of bounds
    }
    return board.get(row, col);
}

/* This function checks if the given word is valid (more than 4 characters, in the dictionary, and unused)
 and returns true if it is valid and false if it is invalid.*/
bool Boggle::checkWord(string word) {
    word = toLowerCase(word);
    if (word.length() < 4 || !dict.contains(word) || foundWords.contains(word)) {
        return false;
    }
    return true;
}

/* This is a helper function for humanWordSearch. It takes a word, row, column, a boolean grid, and a character index.
 * The base case returns false if it is out of bounds, used, or not on the board. It returns true if the word length is the same as
 * the character index. It highlights the successful searches of the word and recurses over each of its neighbors to
 * search for the remainder of the given word. */
bool Boggle::humanWordSearchHelper(string& word, int row, int col, Grid<bool>& used, int currentCharIndex) {

    //base cases
    if (!board.inBounds(row, col)) return false;
    if (used.get(row, col)) return false;
    if (word[currentCharIndex] != board.get(row, col)) return false;
    if (word.length()-1 == currentCharIndex) return true;

    //Mark as used, increment the character index, initialize boolean for found word
    used.set(row, col, true);
    currentCharIndex++;
    bool foundWord = false;

    //Initialize animation pause intervals and highlight the current cube
    BoggleGUI::setAnimationDelay(PAUSE_INTERVAL);
    BoggleGUI::labelCube(row, col, board.get(row, col), true);

    //For each neighbor of the current cube
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            //Recurse over each neighbor, and if the next character in the word is found,
            if (humanWordSearchHelper(word, row+i, col+j, used, currentCharIndex)) {
                //highlight that character and continue
                char ch = board.get(row+i, col+j);
                BoggleGUI::labelCube(row+i, col+j, ch, true);
                foundWord = true;
            }
        }
    }

    //Return whether or not the word given by the user is found on the board.
    return foundWord;
}

/* This function takes an input of string and searches for it in the Boggle board*/
bool Boggle::humanWordSearch(string word) {
    //Check if it's a valid word
    if(!checkWord(word)) return false;
    word = toUpperCase(word);

    Grid<bool> used;
    int currentCharIndex;

    //For each cube in the board,
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            used.resize(BOARD_SIZE, BOARD_SIZE);
            currentCharIndex = 0;
            //Call the helper function to check if its neighbors can form that word
            if (humanWordSearchHelper(word, row, col, used, currentCharIndex)) {
                return true;
           }
        }
    }

    //If the word isn't found on the board, display a message and return false.
    cout << "That word can't be formed on this board." << endl;
    return false;
}

/* This function prints the user's found words and the score and displays them on the GUI */
void Boggle::printFoundWords(string word) {
    BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
    toLowerCase(word);
    foundWords.add(word);
}

/* This function calculates the user's score (3 points for 4-letter word, 4 points for 5-letter word, etc) */
int Boggle::humanScore() {
    int totalScore = 0;
    for (string word: foundWords) {
        int score = word.length() - 3;
        totalScore += score;
    }
    BoggleGUI::setScore(totalScore, BoggleGUI::HUMAN);
    return totalScore;
}

/* This is a helper function for computerWordSearch. It takes the row, column, a boolean grid, an empty string,
 and an empty set of strings. For the base cases, it adds the current character to a string, and if the dictionary
 does not contain a word that starts with that string, it returns the set. If the word is valid, it's added to the
 set. The function recurses over each neighbor to find a string of characters that forms a valid word.*/
Set<string> Boggle::computerWordSearchHelper(int row, int col, Grid<bool> used, string soFar, Set<string> result) {

    //base cases
    char ch = board.get(row, col);
    soFar += charToString(ch);
    soFar = toLowerCase(soFar);
    if (!dict.containsPrefix(soFar)){
        return result;
    }

    //Set the current cube as used
    used.set(row, col, true);

    //If valid word, add to the set
    if (soFar.length() >= 4 && dict.contains(soFar)) result.add(soFar);

    //For each neighboring cube
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            //If it's in bounds and unused, recurse
            if (board.inBounds(row + i, col + j) && !used.get(row + i, col + j)) {
                string copy = soFar;
                result = computerWordSearchHelper(row+i, col+j, used, copy, result);
            }
        }
    }

    //Reset the current cube as unused
    used.set(row, col, false);

    //Return the final set of strings containing all the valid words formed
    return result;
}

/* This function uses recursive backtracking to perform a search on the board for all words
 * that can be formed, returning them as a Set of strings.*/
Set<string> Boggle::computerWordSearch() {
    Set<string> compFoundWords;

    //For each cube in the board,
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            //Call the helper function to get a set of strings containing all valid words formed,
            //and add it to the final set
            Grid<bool> used;
            used.resize(BOARD_SIZE, BOARD_SIZE);
            compFoundWords += computerWordSearchHelper(row, col, used, "", compFoundWords);
        }
    }

    //Return all possible valid words
    return compFoundWords;
}

/* This function takes the set of words the computer found in Boggle and calculates the score. */
int Boggle::getScoreComputer(Set<string> compFoundWords) {
    int totalScore = 0;
    for (string word: compFoundWords) {
        int score = word.length() - 3;
        totalScore += score;
    }
    BoggleGUI::setScore(totalScore, BoggleGUI::COMPUTER);
    return totalScore;
}

/* This function prints all the words the computer found and the score */
void Boggle::printCompFoundWords(Set<string> compFoundWords) {
    for (string word: compFoundWords) {
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
        toLowerCase(word);
    }
}

/* This function returns the size of the Boggle board */
int Boggle::getBoardSize(){
    return BOARD_SIZE;
}

/*This is a << operator for printing a Boggle object to the console */
ostream& operator <<(ostream& out, Boggle& boggle) {
    for (int row = 0; row < boggle.getBoardSize(); row++) {
        for (int col = 0; col < boggle.getBoardSize(); col++) {
            out << boggle.getLetter(row, col);
            out << " ";
        }
        out << endl;
    }
    return out;
}
