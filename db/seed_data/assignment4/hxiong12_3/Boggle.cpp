/*
 * CS 106B Boggle
 * This file implements the methods given in Boggle.h and allows user to play a game of Boggle.
 *
 * Author : Helen Xiong
 * EXTENSION: allows user to set the size of the board. Works for dimensions 2-6.
 * Code added in Boggle.h method and boggleplay method.
 */

#include "Boggle.h"

// letters on all 6 sides of every cube
static string CUBES[36] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ",
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

//number of faces on a die
const int DICE_SIZE = 6;

//size of the edge of the Boggle board
//const int boggle_size = 5;

Boggle::Boggle(Lexicon& dictionary, string boardText, int boggleSize) {
    boggle_size = boggleSize;                   //initializes the size of board
    if (!BoggleGUI::isInitialized()) { //initializes graphics window
        BoggleGUI::initialize(boggle_size, boggle_size);
        BoggleGUI::setAnimationDelay(100);       //animation delay allows user to "see" the recursive algorithm working
    }
    if (boardText != "") {
        createBoard(boardText);                 //custom board
    } else {
        createBoard();                          //random board
    }
    b_dictionary = dictionary;                  //stores dictionary
}

int Boggle::boardSize() {
    return boggle_size;
}

void Boggle::resetVisited() {
    for (int i = 0; i < boggle_size; i++) {
        for (int j = 0; j < boggle_size; j++) {
            visited.set(i, j, false);            //sets all letters to false
            BoggleGUI::labelCube(i, j, board.get(i, j), false); //un-highlights everything
        }
    }
}

char Boggle::getLetter(int row, int col) {
    return board.get(row, col);
}

bool Boggle::checkWord(string word) { //CONDITIONS:
    if (b_dictionary.contains(word)) {  //if word exists
        if (word.size() >= 4) {  //if word is equal to or longer than 4 letters
            if (!humanWordsFound.contains(word)) {  //if word has not already been found
                return true;
            }
        }
    }
    return false; //returns false if three conditions have not been met
}

Set<string> Boggle::getHumanWords() {
    return humanWordsFound;
}

bool Boggle::humanWordSearchHelper(string word,int i, int j) {
    // base case: index is greater than word size, i.e. word can be formed from adjacent cubes
    if (word == "") {
        visited.set(i, j, true);
        BoggleGUI::labelCube(i, j, board.get(i, j), true); //highlights the last letter
        return true; // end of search
    } else {
        visited.set(i, j, true); //CHOOSE and mark cube as chosen
        BoggleGUI::labelCube(i, j, board.get(i, j), true);
        for (int h = i - 1; h < i + 2; h++) { //EXPLORE all cubes surrounding chosen cube
            for (int k = j - 1; k < j + 2; k++) {
                if (board.inBounds(h, k) && (visited.get(h, k) == false)) { //if cube has not yet been chosen
                    //recurses; if cube = next letter of the word, recursive method checks all the surrounding cubes again
                    if ((board.get(h, k) == word[0]) && humanWordSearchHelper(word.substr(1), h, k)) {
                        return true; //end search
                    }
                }
            }
        }
        visited.set(i, j, false); //UN-CHOOSE
        BoggleGUI::labelCube(i, j, board.get(i, j), false);
    }
    return false; //word cannot be formed from the cube at (i, j)
}


bool Boggle::humanWordSearch(string word) {
    for (int i = 0; i < boggle_size; i++) {
        for (int j = 0; j < boggle_size; j++) {
            resetVisited(); //clear the board
            if (board.get(i, j) == word[0]) { //if the cube is the first letter of the word
                if (humanWordSearchHelper(word.substr(1), i, j)) { //starts the search
                    humanWordsFound.add(word); //adds completed word to the humanWordsFound set
                    BoggleGUI::recordWord(word, BoggleGUI::HUMAN); //adds word to the human side of the GUI
                    return true;
                }
            }
        }
    }
    return false; //word cannot be formed
}

int Boggle::humanScore() {
    int score = 0;
    for (string word : humanWordsFound) { //for each word found so far
        score += word.size() - 3; //adds 1 pt for 4-letter word; 2 pt for 5-letter word, etc.
    }
    BoggleGUI::setScore(score, BoggleGUI::HUMAN); //updates GUI
    return score;
}

void Boggle::computerWordSearchHelper(Set<string>& wordsFound, string word, int i, int j) {
    visited.set(i, j, true); //choose a cube
    for (int h = i - 1; h < i + 2; h++) { //explore all cubes surrounding the chosen cube
        for (int k = j - 1; k < j + 2; k++) {
            if (board.inBounds(h, k) && (visited.get(h, k) == false)) { //if cube has not been explored yet
                word += board.get(h, k); //CHOOSE: adds the new letter to the potential word
                if (b_dictionary.contains(word)) { //dictionary contains the potential word; i.e. a word has been found
                    if (word.size() >= 4) { //if the word is long enough, i.e. greater than 4 characters
                        if (!humanWordsFound.contains(word)) wordsFound.add(word); //if the human player didn't find the word, add it to the ComputerWordsFound set
                    }
                }
                if (b_dictionary.containsPrefix(word)) { //if there is a word in the lexicon that begins with the potential word string
                    computerWordSearchHelper(wordsFound, word, h, k); //explore all cubes surrounding the last cube explored
                }
                word = word.substr(0, word.size() - 1); //UN-CHOOSE: removes the explored letter
            }
        }
    }
    visited.set(i, j, false); //un-choose the cube
}

Set<string> Boggle::computerWordSearch() {
    Set<string> wordsFound; //set of words found by computer
    string word = ""; //initial empty word
    resetVisited(); //clears the board
    for (int i = 0; i < boggle_size; i++) {
        for (int j = 0; j < boggle_size; j++) {
            word = board.get(i, j); //tests all cubes on the board
            computerWordSearchHelper(wordsFound, word, i, j); //recursive test method
        }
    }
    return wordsFound;
}

int Boggle::getScoreComputer() {
    int score = 0;
    for (string word : computerWordSearch()) { //for each word found by the computer
        score += word.size() - 3; //adds 1 pt for 4-letter word; 2 pt for 5-letter word, etc.
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(score, BoggleGUI::COMPUTER); //updates score on GUI
    return score;
}

//Creates a RANDOM BOARD
void Boggle::createBoard() {
    board.resize(boggle_size, boggle_size);
    visited.resize(boggle_size, boggle_size); //sets up Boggle board and "visited" grid
    int i, j; char charOnFace;
    for (i = 0; i < boggle_size; i++) {
        for (j = 0; j < boggle_size; j++) {
            int h = boggle_size * i + j;
            int random = randomInteger(0, DICE_SIZE - 1); //chooses random face on die
            charOnFace = CUBES[h][random]; //selects a random string
            board.set(i, j, charOnFace); //puts selected character on cube
        }
    }
    shuffle(board); //shuffles the cubes
    string letters;
    for (int i = 0; i < boggle_size; i++) { //creates a string with all the shuffled letters in order
        for (int j = 0; j < boggle_size; j++) {
            BoggleGUI::labelCube(i, j, board.get(i, j), false);
            visited.set(i, j, false);
        }
    }
}

//Creates a CUSTOM BOARD
void Boggle::createBoard(string boardText) {
    board.resize(boggle_size, boggle_size); //initialize Boggle board
    visited.resize(boggle_size, boggle_size); //initialize visited grid
    for (int i = 0; i < boggle_size; i++) {
        for (int j = 0; j < boggle_size; j++) {
            int h = boggle_size * i + j;
            char charOnFace = boardText[h]; //chooses proper letter for each position
            board.set(i, j, charOnFace); //puts letter in proper place
            visited.set(i, j, false);
        }
    }
    BoggleGUI::labelAllCubes(boardText); //updates GUI
}

ostream& operator<<(ostream& out, Boggle& boggle) { //outputs the Boggle board as four lines of four characters
    for (int i = 0; i < boggle.boardSize(); i++) {
        string line = "";
        for (int j = 0; j < boggle.boardSize(); j++) {
            line += boggle.getLetter(i, j);
        }
        cout << line << endl;
    }
}
