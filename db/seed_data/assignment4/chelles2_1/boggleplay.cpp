/* This is the client to perform console UI and work with the Boggle class to play a game.
 */

#include "lexicon.h"
#include "strlib.h"
#include "bogglegui.h"
#include "Boggle.h"
#include "simpio.h"

/* This function sets up the board. It gets user input to determine whether or not to generate a random board.
 * If no, it prompts the user for 16 letters to form the board and re-prompts if input is invalid.
 * If yes, it calls the function Boggle to create a randomized board. */
Boggle setUpBoard(Lexicon& dictionary, Boggle& b) {
    if (!getYesOrNo("Do you want to generate a random board?")) {
            string boardText = getLine("Type the 16 letters to appear on the board: ");
            while (boardText.length() != 16) {
                cout << "That is not a valid 16-letter board string. Try again." << endl;
                boardText = getLine("Type the 16 letters to appear on the board: ");
            }
            boardText = toUpperCase(boardText);
            return Boggle(dictionary, boardText);
        } else {
            return Boggle(dictionary, "");
        }
}

/* Prints the Boggle board onto the console */
void printBoard(Boggle& b) {
    for (int i = 0; i < b.getBoardSize(); i++) {
        for (int j = 0; j < b.getBoardSize(); j++) {
            cout << b.getLetter(i, j) << " ";
        }
        cout << endl;
    }
}

/* The user plays Boggle by inputting word found on the board. If it is an invalid word, reprompt.
 If it is valid, update the score, highlight the word, and print the words found so far.*/
void userPlays(Boggle& b) {
    while (true) {
        string word = getLine("Type a word (or Enter to stop): ");
        if (word == "") break;
        if (!b.checkWord(word)) {
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        }
        if (b.humanWordSearch(word)) {
            cout << "You found a new word! " << word << endl;
            b.printFoundWords(word);
            cout << "Your words (" << b.foundWords.size() << "): " << b.foundWords << endl;
            cout << "Your score: " << b.humanScore() << endl;
        }
        BoggleGUI::clearHighlighting();
    }
}

/* Works with the Boggle class to play one game between the user and the computer. */
void playOneGame(Lexicon& dictionary) {

    // Initialize the board
    Boggle b = setUpBoard(dictionary, b);
    b.displayGrid();
    printBoard(b);

    userPlays(b); //User's search

    cout << "It's my turn!" << endl;

    //Computer's search
    Set<string> compFoundWords = b.computerWordSearch();
    b.printCompFoundWords(compFoundWords);
    cout << "My words (" << compFoundWords.size() << "): " << compFoundWords << endl;
    cout << "My score: " << b.getScoreComputer(compFoundWords) << endl;

    //Display results
    if (b.humanScore() < b.getScoreComputer(compFoundWords)) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    } else if (b.humanScore() == b.getScoreComputer(compFoundWords)) {
        cout << "We tied!" << endl;
    }
}
