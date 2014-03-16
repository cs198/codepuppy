/* Connie Zeng
 * CS 106B, Winter 2014
 * ---------------------
 * Assignment 4: Boggle
 * This program lets the user play a game of Boggle. This file handles the user
 * interaction through the console, including displaying messages and prompting
 * for input. The game is also animated in a separate GUI window.
 *
 * (1 late day)
 * incomplete submission, will submit final project by Monday
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

// Plays one round of Boggle
void playOneGame(Lexicon& dictionary) {
    if(BoggleGUI::isInitialized()){
       BoggleGUI::reset();
       BoggleGUI::setStatusMessage("");
    }
    else{
        BoggleGUI::initialize(4, 4);
        BoggleGUI::setAnimationDelay(50);
    }

    string boardText = "";
    if(!getYesOrNo("Do you want to generate a random board? ")){
        boardText = promptUserForString();
    }
    Boggle boggle(dictionary, boardText);

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

string promptUserForString(){
    while(true){
        string input = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
        if(isValid(input)) return input;
        cout << "That is not a valid 16-letter board string. Try again." << endl;
    }
}

bool isValid(string input){
    if(input.length() != 16) return false;
    for(char ch : input){
        if(!isalpha(ch)) return false;
    }
    return true;
}

void human(Boggle& boggle){
    clearConsole();
    cout << "It's your turn!" << endl;
    BoggleGUI::setStatusMessage("It's your turn!");
    while(true){
        Set<string> words = boggle.humanWords();
        cout << boggle << endl;
        cout << "Your words (" << words.size() << "): " << words << endl;
        cout << "Your score: " << boggle.humanScore() << endl;

        string turn = toUpperCase(getLine("Type a word (or Enter to stop): "));
        if(turn == "") break;
        clearConsole();
        BoggleGUI::clearHighlighting();
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

void computer(Boggle& boggle){
    cout << "It's my turn!" << endl;
    BoggleGUI::setStatusMessage("It's my turn!");
    BoggleGUI::clearHighlighting();
    Set<string> words = boggle.computerWordSearch();
    cout << "My words (" << words.size() << "): " << words << endl;
    cout << "My score: " << boggle.getScoreComputer() << endl;
}
