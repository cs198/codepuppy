/*
 * CS106B Assignment 4: Boggle
 * Emma Daugherty
 * Section Leader: Jimmy Lee
 *
 * This implements the playOneGame method. It simulates one game of boggle.
 */

#include "lexicon.h"
#include "simpio.h"
#include "Boggle.h"
#include "strlib.h"
#include "simpio.h"

//methods
string getBoardText();
void enterWords(Boggle& boggle);
void calculateScore(Boggle& boggle);
void displayStatus(Boggle& boggle, int numWords);

/*
 * Implements one game of Boggle
 */
void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(4, 4);
    BoggleGUI::setAnimationDelay(150);

    string bText = getBoardText();
    Boggle boggle(dictionary, bText);
    clearConsole();
    cout << "It's your turn!" << endl;
    displayStatus(boggle, 0);

    enterWords(boggle);
    cout << endl;
    cout << "It's my turn!" << endl;
    calculateScore(boggle);
}
/*
 * This method creates the board for boggle, allowing for the user to input their own letters if they choose
 */
string getBoardText() {
    bool board = getYesOrNo("Do you want to generate a random board? ");
    string bText;
    if (!board) {
        bText = getLine("Enter your text");
        while (bText.length() != 16) {
            bText = getLine("Please enter 16 letters");
        }
        bText = toUpperCase(bText);
    }
    else {
        bText = "";
    }
    return bText;
}
/*
 * This method allows the user to enter words and checks if they are valid
 */
void enterWords(Boggle& boggle) {
    int numWords = 0;
    while (true) {
        string entry = getLine("Type a word (or Enter to stop): ");
        entry = toUpperCase(entry);
        if (entry == "") {
            break;
        }
        else {
            clearConsole();
            if (boggle.humanWordSearch(entry)) {
                cout << "You found a new word! \"" << entry << "\"" << endl;
                numWords++;
            }
            else {
                cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
            }
            displayStatus(boggle, numWords);
        }
    }
}
/*
 * This method calculates the human and computer scores and determines the winner
 */
void calculateScore(Boggle& boggle) {
    int humanScore = boggle.humanScore();
    Set<string> words = boggle.computerWordSearch();
    cout << "My words(" << words.size() << "): " << words.toString() << endl;
    int computerScore = boggle.getScoreComputer();
    cout << "My score: " << computerScore << endl;

    if (humanScore > computerScore) {
        cout << "WOW, you defeated me! Congratulations!" << endl;
    }
    else {
        cout << "Ha ha ha, I destroyed you. Better luck next" << endl << "time, puny human!" << endl;
    }
}
/*
 * This method displays the current status of the player. It prints the board, players found words and the players
 * score.
 */
void displayStatus(Boggle& boggle, int numWords) {
    cout << boggle << endl;
    cout << "Your words (" << numWords << "): " << boggle.getWords() << endl;
    cout << "Your score: " << integerToString(boggle.humanScore()) << endl;
}
