/* Connie Zeng
 * CS 106B, Winter 2014
 * ---------------------
 * Assignment 4: Boggle
 * This program lets the user play a game of Boggle. This file handles the user
 * interaction through the console, including displaying messages and prompting
 * for input. The game is also animated in a separate GUI window.
 *
 * (1 late day)
 */

#include <cctype>
#include <string>
#include <iostream>
#include "lexicon.h"
#include "console.h"
#include "simpio.h"
#include "Boggle.h"
#include "bogglegui.h"

/* Function prototypes */
string promptUserForString();
bool isValid(string input);
void human(Boggle& boggle);
void computer(Boggle& boggle);

/* Function: playOneGame
 * Usage: playOneGame(dictionary);
 * --------------------------------
 * This function plays one round of Boggle. The user can choose to generate
 * a random board or enter a string of letters to fill in the board with. The
 * user then finds as many words as possible from the board, after which the
 * computer finds all remaining words.
 */

void playOneGame(Lexicon& dictionary) {
    // Starts GUI window with a blank 4x4 boggle board
    if(BoggleGUI::isInitialized()){
       BoggleGUI::reset();
       BoggleGUI::setStatusMessage("");
    }
    else{
        BoggleGUI::initialize(4, 4);
        BoggleGUI::setAnimationDelay(50);
    }

    // Creates new Boggle object
    string boardText = "";
    if(!getYesOrNo("Do you want to generate a random board? ")){
        boardText = promptUserForString();
    }
    Boggle boggle(dictionary, boardText);

    // Game play and result
    human(boggle);
    cout << endl;
    computer(boggle);
    if(boggle.humanScore() >= boggle.getScoreComputer()){
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
    }
    else{
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    }
}

/* Function: promptUserForString
 * Usage: string input = promptUserForString();
 * ---------------------------------------------
 * This function prompts the user for a string of letters to fill in the Boggle
 * board with, if a random board is not selected. If the string is not
 * appropriate for a Boggle board, the user is given additional chances to
 * enter a valid string.
 */

string promptUserForString(){
    while(true){
        string input = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
        if(isValid(input)) return input;
        cout << "That is not a valid 16-letter board string. Try again." << endl;
    }
}

/* Function: isValid
 * Usage: bool valid = isValid(input);
 * ------------------------------------
 * This function returns whether the passed string is a valid Boggle board
 * string. The string must contain exactly 16 letters (no numerical digits or
 * whitespace).
 */

bool isValid(string input){
    if(input.length() != 16) return false;
    for(char ch : input){
        if(!isalpha(ch)) return false;
    }
    return true;
}

/* Function: human
 * Usage: human(boggle);
 * ----------------------
 * This function runs the human's turn in the game. It prompts the user to
 * enter a word and checks that the word is valid, then updates the human's
 * score. The user presses enter to signal the end of the turn.
 */

void human(Boggle& boggle){
    clearConsole();
    cout << "It's your turn!" << endl;
    BoggleGUI::setStatusMessage("It's your turn!");

    // Asks user for words until enter is pressed
    while(true){
        // Displays current found words and score
        Set<string> words = boggle.humanWords();
        cout << boggle << endl;
        cout << "Your words (" << words.size() << "): " << words << endl;
        cout << "Your score: " << boggle.humanScore() << endl;
        string turn = toUpperCase(getLine("Type a word (or Enter to stop): "));
        if(turn == "") break;

        // Clears previous entry to provide cleaner interface
        clearConsole();
        BoggleGUI::clearHighlighting();

        // Checks that word is valid
        if(!boggle.checkWord(turn)){
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        }
        else if(!boggle.humanWordSearch(turn)){
            cout << "That word can't be formed on this board." << endl;
        }
        else{
            cout << "You found a new word! \"" << turn << "\"" << endl;
            BoggleGUI::setStatusMessage("You found a new word! \"" + turn + "\"");
        }
    }
}

/* Function: computer
 * Usage: computer(boggle);
 * -------------------------
 * This function runs the computer's turn in the game. It finds all the words
 * that the user missed and calculates the computer's score.
 */

void computer(Boggle& boggle){
    cout << "It's my turn!" << endl;
    BoggleGUI::setStatusMessage("It's my turn!");
    BoggleGUI::clearHighlighting();
    Set<string> words = boggle.computerWordSearch();
    cout << "My words (" << words.size() << "): " << words << endl;
    cout << "My score: " << boggle.getScoreComputer() << endl;
}
