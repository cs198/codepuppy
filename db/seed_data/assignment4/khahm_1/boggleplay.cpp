/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * boggleplay.cpp file
 * players one game of boggle
 */

#include "lexicon.h"
#include "bogglegui.h"
#include "simpio.h"
#include "Boggle.h"
#include "strlib.h"
#include "ctype.h"

// the size of one side of the board
const int BOARD_SIZE = 4;

// checks if the given line is valid to set up a board
bool isValid(string &line);
// prints the console message for human player
void printHumanConsoleMsg(Boggle &b);
// runs the human player's turn
void humanTurn(Boggle &b, string& boardLetters);
// gets the text on the board
string getBoardText();
// runs the computer's turn
void computerTurn(Boggle &b);
// prints the resulting message
void printResultMsg(Boggle& b);

/*
 * plays one game of Boggle consisting of
 * the human player's turn and computer's turn
 */
void playOneGame(Lexicon& dictionary) {
    if (BoggleGUI::isInitialized()) {
        BoggleGUI::reset();
    }
    else {
        BoggleGUI::initialize(BOARD_SIZE, BOARD_SIZE);
    }
    string boardText = getBoardText();
    Boggle b (dictionary, boardText);
    string boardLetters;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            boardLetters += b.getLetter(r,c);
        }
    }
    BoggleGUI::labelAllCubes(boardLetters);
    clearConsole();
    humanTurn(b, boardLetters);
    cout << endl;
    computerTurn(b);
    printResultMsg(b);
}

// gets the text for generating the board
string getBoardText() {
    string boardText;
    if (getYesOrNo("Do you want to generate a random board?")) {
        boardText = "";
    }
    else {
        string line = getLine("Type the 16 letters to appear on the board: ");
        while (!isValid(line)) {
            cout << "That is not a valid 16-letter string. Try again." << endl;
            line = getLine("Type the 16 letters to appear on the board: ");
        }
        boardText = toUpperCase(line);
    }
    return boardText;
}

// checks if the input line is valid to make a board
bool isValid(string& line) {
    if (line.size() != 16) {
        return false;
    }
    for (int i = 0; i < line.size(); i++) {
        if (!isalpha(line[i])) {
            return false;
        }
    }
    return true;
}

// runs the human's turn and displays it
void humanTurn(Boggle& b, string& boardLetters) {
    cout << "It's your turn!" << endl;
    printHumanConsoleMsg(b);
    string word = getLine("Type a word (or Enter to stop): ");
    while (word != "") {
        clearConsole();
        word = toUpperCase(word);
        if (b.humanWordSearch(word, boardLetters)) {
            cout << "You found a new word! \"" << word << "\"" << endl;
            printHumanConsoleMsg(b);
        }
        else if (b.checkWord(word)) {
            cout << "That word can't be formed on this board." << endl;
            printHumanConsoleMsg(b);
        }
        else {
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
            printHumanConsoleMsg(b);
        }
        word = getLine("Type a word (or Enter to stop): ");
    }
}

// prints the console message for the human player's turn
void printHumanConsoleMsg(Boggle& b) {
    cout << b << endl;
    cout << "Your words (" << b.getFoundWords().size() << "): " << b.getFoundWords().toString() << endl;
    cout << "Your score: " << b.humanScore() << endl;
}

// runs the computer's turn and displays it
void computerTurn(Boggle& b) {
    cout << "It's my turn!" << endl;
    Set<string> compList = b.computerWordSearch();
    cout << "My words (" << compList.size() << "): " << compList.toString() << endl;
    cout << "My score: " << b.getScoreComputer() << " " << endl;
}

// prints whether the human or the computer won
void printResultMsg(Boggle& b) {
    if (b.getScoreComputer() > b.humanScore()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    }
    else {
        cout << "Wow you defeated me! Congratulations!" << endl << endl;
    }
}
