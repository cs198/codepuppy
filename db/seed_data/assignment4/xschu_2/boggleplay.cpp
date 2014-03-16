// This cpp file is used for playing boggle game for one time.

#include "lexicon.h"
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "Boggle.h"
#include "shuffle.h"
#include "bogglegui.h"

/*
 * This function check if the user input string valid.
 * It returns false if the string length does not consist 16 characters, and if there is a non-alphabetical character in the string.
 */
bool validText(string s) {
    if (s.size() != 16) return false;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] < 'A' || s[i] > 'z' || (s[i] > 'Z' && s[i] < 'a')) return false;
    }
    return true;
}

/*
 * This function asks the user if he wants to generate the random board or manual input a board.
 * It returns a blank string if the user chooses to automatically generate the board, and returns the string if users chooses to manually generate the board.
 */
string generateBoard(Lexicon& dictionary) {
    string text = "";
    if (!getYesOrNo("Do you want to generate a random board? ")) {
        text = getLine("Type the 16 letters to appear on the board: ");
        while (!validText(text)) {
            cout << "This is not a valid 16-letter board string. Try again." << endl;
            text = getLine("Type the 16 letters to appear on the board: ");
        }
        text = toUpperCase(text);
    }
    return text;
}

/*
 * This function is called when it is the human's turn to play boggle.
 * It calls the methods in Boggle class to search the word, and update the message.
 */
void humanTurn(Boggle& boggle, string word) {
    while (word != "") {
        clearConsole();
        if (boggle.humanWordSearch(word)) {
            cout << "You found a new word!" << toUpperCase(word) << endl;
            BoggleGUI::setStatusMessage("You found a new word! \"" + toLowerCase(word) + "\"");
        } else {
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
            BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
        }
        cout << boggle << endl;
        cout << endl;
        cout << "Your words (" << boggle.humanWords().size() << "): {" << boggle.humanWords().toString() << "}" << endl;
        cout << "Your score: " << boggle.humanScore() << endl;
        word = getLine("Type a word (or Enter to stop): ");
    }
}

/*
 * This function is called when it is the computer's turn to play boggle.
 * It calls functions in Boggle class to search all the valid boggle word, and update word list and message on the screen.
 */
void computerTurn(Boggle& boggle) {
    cout << "It's my turn!" << endl;
    cout << "My words (" << boggle.computerWordSearch().size() << "): " << boggle.computerWordSearch().toString() << endl;
    cout << "My score: " << boggle.getScoreComputer() << endl;
    BoggleGUI::setStatusMessage("It's my turn!");
}

/*
 * This is the main funcion in the cpp file.
 * It clears the console, and updates the message on console and graphic window.
 * It calls functions to perform human's and computer's turn to play boggle.
 */
void playOneGame(Lexicon& dictionary) {
    if (BoggleGUI::isInitialized()) {
        BoggleGUI::reset();
    } else {
        BoggleGUI::initialize(4, 4);
    }
    string text = generateBoard(dictionary);
    Boggle boggle(dictionary, text);
    clearConsole();
    cout << "It's your turn!" << endl;
    BoggleGUI::setStatusMessage("It's your turn!");
    cout << boggle << endl;
    cout << endl;

    cout << "Your words (" << boggle.humanWords().size() << "): " << boggle.humanWords().toString() << endl;
    cout << "Your score: " << boggle.humanScore() << endl;
    string word = getLine("Type a word (or Enter to stop): ");

    humanTurn(boggle, word);
    computerTurn(boggle);

    if (boggle.getScoreComputer() > boggle.humanScore()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else {
        cout << "Wow, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("Wow, you defeated me! Congratulations!");
    }
}






