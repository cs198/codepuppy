// Name: Austin Poore
// This is a .cpp file which implements the playOneGame method that gets called by
// the boggleMain class. It makes use of a handful of helper methods to accomplish its task.
//
// I used lecture slides and the Stanford C++ library reference documentation.

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include <cctype>
#include "bogglegui.h"

// This function asks the user for a string to use as board text and stores it in a reference
// called boardText.
void getUserInput(string& boardText);

// This function verifies that the user's input string is a valid sequence of 16 alphabetic characters.
// It returns true if the input is valid and false otherwise.
bool validInput(string& boardText);

// This function reads in words entered by the user and verifies that they are, indeed, valid words.
void getWordsFromUser(Boggle& currentBoggle);

// This function compares the scores of the human and computer to figure out the winner, and then prints
// the appropriate message.
void checkWinner(Boggle& currentBoggle);


void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(4, 4);
    BoggleGUI::setAnimationDelay(100);
    string boardText;
    cout << endl;
    getUserInput(boardText);
    Boggle currentBoggle(dictionary, boardText);
    clearConsole();
    cout << "It's your turn!" << endl;
    cout << currentBoggle;
    getWordsFromUser(currentBoggle);
    cout << endl;
    cout << "It's my turn!" << endl;
    Set<string> computerWords = currentBoggle.computerWordSearch();
    cout << "My words (" << currentBoggle.getComputerWords() << "): " << computerWords.toString() << endl;
    cout << "My score: " << currentBoggle.getScoreComputer() << endl;
    checkWinner(currentBoggle);
}

void getUserInput(string& boardText) {
    if (!getYesOrNo("Do you want to generate a random board? ", "Please type a word that begins with 'y' or 'n'. ")) {
        while(true) {
            boardText = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
            if (validInput(boardText)) {
                break;
            }
            cout << "That is not a valid 16-letter board string. Try again." << endl;
        }
    }
}

bool validInput(string& boardText) {
    if (boardText.length()!= 16) {
        return false;
    }
    for (int i = 0; i < boardText.length(); i ++) {
        if (!isalpha(boardText[i])) {
            return false;
        }
    }
    return true;
}

void getWordsFromUser(Boggle& currentBoggle) {
    while(true) {
        string word = toUpperCase(getLine("Type a word (or Enter to stop): "));
        if (word == "") {
            break;
        }
        clearConsole();
        if (currentBoggle.checkWord(word)) {
            if (currentBoggle.humanWordSearch(word)) {
                cout << "You found a new word! \""<< word << "\"" << endl;
                BoggleGUI::setStatusMessage("You found a new word! \"" + word + "\"");
            } else {
                cout << "That word can't be formed on this board." << endl;
                BoggleGUI::setStatusMessage("That word can't be formed on this board.");
            }
        } else {
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
            BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
        }
        cout << currentBoggle;
    }
}

void checkWinner(Boggle& currentBoggle) {
    if (currentBoggle.getScoreComputer() > currentBoggle.humanScore()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else {
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
    }
}
