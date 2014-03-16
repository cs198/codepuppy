/**
 * Name: Milan Doshi
 * Assignment: boggleplay.cpp
 * TA: Jimmy Lee
 *
 * This file assist in the I/O with the user and initializes a Boggle class object and then calls upon the methods in the class to allow the user to play
 * one game of Boggle. This file allows the user to create their own board, a random board, and then goes through the process of playing
 * one game between the user and the computer.
 */

#include "lexicon.h"
#include "simpio.h"
#include "console.h"
#include "Boggle.h"
#include "bogglegui.h"

void humanGameState(Boggle& game, Vector<string> humanWords, string output);
void computerGameState(Boggle& game);
void outputWinner(Boggle& game);
bool checkBoardText(string boardText);

/**
 * method: playOneGame()
 * return: void
 * This method allows the user to play one game against the computer in its entirety.
 */
void playOneGame(Lexicon& dictionary) {
    if (BoggleGUI::isInitialized()) {
        BoggleGUI::reset();
    } else {
        BoggleGUI::initialize(4, 4);
        BoggleGUI::setAnimationDelay(200);
    }
    string boardText = "";
    string output = "";
    cout << "Do you want to generate a random board? ";
    bool whatBoard = getYesOrNo();
    if(whatBoard == false) {
        boardText = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
        boardText = trim(boardText);
        while(!checkBoardText(boardText)) {
            boardText = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
            boardText = trim(boardText);
        }
    }
    Boggle game = Boggle(dictionary, boardText);
    clearConsole();
    output = "It's your turn!";
    humanGameState(game,game.humanWords,output);
    string word = toUpperCase(getLine("Type a word (or Enter to stop): "));
    word = trim(word);
    while(word != "") {
        if(game.checkWord(word)) {
            BoggleGUI::labelAllCubes(game.getBoardText());
            if(game.humanWordSearch(word)) {
                game.humanWords.add(word);
                game.removeWord(word);
                output = "You found a new word! ";
                output += "\"" + word + "\"";
                BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                humanGameState(game,game.humanWords, output);
            } else {
                output = "That word can't be formed on this board.";
                BoggleGUI::setStatusMessage(output);
                humanGameState(game, game.humanWords, output);
            }

       } else {
           output = "You must enter an unfound 4+ letter word from the dictionary.";
           BoggleGUI::setStatusMessage(output);
           humanGameState(game, game.humanWords, output);
        }
        word = toUpperCase(getLine("Type a word (or Enter to stop): "));
        word = trim(word);
    }

    cout << "It's my turn!" << endl;
    computerGameState(game);
    outputWinner(game);
}

/**
 * method: humanGameState(Boggle& game, Vector<string> humanWords, string output)
 * return: void
 * This method prints the game state of the human, which includes any messages, their words, and their score.
 */
void humanGameState(Boggle& game, Vector<string> humanWords, string output) {
    clearConsole();
    cout << output << endl;
    cout << game << endl;
    cout << "Your words(" << humanWords.size() << "): " << humanWords.toString() << endl;
    cout << "Your score: " << game.humanScore() << endl;
}

/**
 * method: computerGameState(Boggle& game)
 * return: void
 * This method prints the game state of the computer, which their words, and their score.
 */
void computerGameState(Boggle& game) {
    Set<string> result = game.computerWordSearch();
    cout << "My words(" << result.size() << "): " << result.toString() << endl;
    cout << "My score: " << game.getScoreComputer() << endl;
}

/**
 * method: outputWinner(Boggle& game)
 * return: void
 * This method prints out who the winner is based on calling both the human and the computer score.
 */
void outputWinner(Boggle& game) {
    string result = "";
    if(game.humanScore() > game.getScoreComputer()) {
        result = "Beginners luck. I guess you win, human. Don't get used to this.";
        BoggleGUI::setStatusMessage(result);
        cout << result << endl;
    } else if (game.humanScore() < game.getScoreComputer()) {
        result = "Ha ha ha, I destroyed you. Better luck next time, puny human!";
        BoggleGUI::setStatusMessage(result);
        cout << result << endl;
    } else if (game.humanScore() == game.getScoreComputer()) {
        result = "I guess neither one of us was meant to win.";
        BoggleGUI::setStatusMessage(result);
        cout << result << endl;
    }
}

/**
 * method: checkBoardText(string boardText)
 * return: bool
 * This method checks whether boardText text entered by the user is only 16 characters and only alphabetical letters.
 */
bool checkBoardText(string boardText) {
    for(int i = 0; i < boardText.size(); i++)
        if(!isalpha(boardText[i])) {
             cout << "That is not a valid 16-letter board string. Try again." << endl;
             return false;
        }
    if(boardText.length() != 16) {
        cout << "That is not a valid 16-letter board string. Try again." << endl;
        return false;
    } else {
        return true;
    }
}
