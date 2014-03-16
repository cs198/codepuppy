/* Name: Lindsey Txakeeyang
 * Class: 106B, Wednesday 4:15 section
 * Assignment: Boggle
 * This program replicates the game of Boggle with one human player and one computer player.
 */

#include "lexicon.h"
#include "Boggle.h"
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "bogglegui.h"


string generateBoardText();
bool isAllLetters(string boardConfiguration);
void printHumanStateOfGame(Boggle& boggle);
void humanTurn(Boggle& boggle, Lexicon& dictionary);
void clearBoard();
void computerTurn(Boggle& boggle, Lexicon& dictionary);

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(4, 4);
    string boardText = generateBoardText();
    Boggle boggle(dictionary, boardText);
    clearBoard();
    cout << "It's your turn!" << endl;
    printHumanStateOfGame(boggle);
    humanTurn(boggle, dictionary);
    computerTurn(boggle, dictionary);
    boggle.resetSets();
    BoggleGUI::shutdown();
}

/*
 * Generates the gameboard through either random configuration or user-input.
 */
string generateBoardText() {
    string boardText = "";
    string answer = "";
    while (true) {
        if (getYesOrNo("Do you want to generate a random board? ")) {
            return boardText;
        } else {
            while (true) {
                boardText = getLine("Type the 16 letters to appear on the board: ");
                if (boardText.length() == 16) {
                    if (isAllLetters(boardText)) {
                        boardText = toUpperCase(boardText);
                        return boardText;
                    }
                } else cout << "That is not a valid 16-letter board string. Try again." << endl;
            }
        }
    }
}

/*
 * Checks to make sure the user-input string is made of only letters.
 */
bool isAllLetters(string boardConfiguration) {
    for (int i = 0; i < boardConfiguration.length(); i++) {
        if (!isalpha(boardConfiguration[i])) return false;
    }
    return true;
}

/*
 * Prints the current state of the game without computing a new score.
 * Passes by reference the gameboard, the word input by the user and the score.
 */
void printHumanStateOfGame(Boggle& boggle) {
    boggle.printBoard();
    cout << endl;
    cout << "Your words (" << boggle.numberOfHumanWords() << "): " << boggle.printHumanUsedWords() << endl;
    cout << "Your score: " << boggle.humanScore() << endl;
}

/*
 * Clears the board.
 */
void clearBoard() {
    clearConsole();
}

/*
 * Plays one turn for the human player. Runs through all the conditions necessary
 * for a word input by a user.
 * Passes by reference the gameboard, the user-input word, and the score.
 */
void humanTurn(Boggle& boggle, Lexicon& dictionary) {
    while (true) {
        string word = getLine("Type a word (or press Enter to end your turn): ");
        if (word == "") break;
        if (!boggle.isRightLength(word)) {
            clearBoard();
            cout << "That word is not long enough." << endl;
            printHumanStateOfGame(boggle);
        }
        if (boggle.hasBeenUsed(word)) {
            clearBoard();
            cout << "You have already guessed that word." << endl;
            printHumanStateOfGame(boggle);
        }
        else if (!boggle.inDictionary(word, dictionary)) {
            clearBoard();
            cout << "That word is not in the dictionary." << endl;
            printHumanStateOfGame(boggle);
        }
        else if (!boggle.humanWordSearch(word)) {
            clearBoard();
            cout << "That word can't be formed on the board." << endl;
            printHumanStateOfGame(boggle);
        }
        else if (boggle.checkWord(word, dictionary)) {
            clearBoard();
            cout << "You found a new word!";
            word = toUpperCase(word);
            cout << " " << word << endl;
            boggle.addHumanUsedWord(word);
            printHumanStateOfGame(boggle);
        }
        BoggleGUI::clearHighlighting();
    }
}

/*
 * Plays the computer's turn.
 * Passes by reference the gameboard, and the score.
 */
void computerTurn(Boggle& boggle, Lexicon& dictionary) {
    cout << endl;
    cout << "It's my turn." << endl;
    boggle.computerWordSearch(dictionary);
    cout << "My words (" << boggle.numberOfComputerWords() << "): " << boggle.printComputerWords() << endl;
    cout << "My score: " << boggle.computerScore() << endl;
    if (boggle.humanScore() >= boggle.computerScore()) {
        cout << "WOW, you defeated me! Congratulations!" << endl;
    } else {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    }
}
