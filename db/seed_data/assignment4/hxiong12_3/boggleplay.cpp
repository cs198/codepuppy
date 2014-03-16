/*
 * CS 106B Boggle
 * This file controls the user-console interaction needed to play one game of Boggle.
 * Calls upon the Boggle class to create and update a Boggle object.
 */

#include "lexicon.h"
#include "foreach.h"
#include "Boggle.h"
#include "bogglegui.h"
#include "simpio.h"
#include "console.h"

/*Returns true if an input string consists entirely of letters. Used in the method getBoardText.*/
bool isAllLetters(string str) {
    for (char ch : str) {
        if (!isalpha(ch)) return false;
    }
    return true;
}

/*Asks user if he/she would like to create a custom board, and if so,
 *asks user to input a string of 16-letters that correspond to their custom board.*/
string getBoardText(int boggle_size) {
    string boardText;
    if (!getYesOrNo("Do you want to generate a random board? ")) { //generate a custom board
        boardText = getLine("Type the " + integerToString(boggle_size*boggle_size) + " letters to appear on the board: ");
        while ((boardText.size() != boggle_size * boggle_size) || !isAllLetters(boardText)) { //board is 16 letters long and contains only alphabet
            cout << "That is not a valid " + integerToString(boggle_size*boggle_size) + "-letter board string. Try again." << endl;
            boardText = getLine("Type the " + integerToString(boggle_size*boggle_size) + " letters to appear on the board: ");
        }
        boardText = toUpperCase(boardText);
    } else {boardText = "";} //user wishes to generate a random board; i.e. boardText is empty
    return boardText;
}

/*Plays one round for a human player. Prints feedback regarding the last word entered, a text copy of the game board,
 * a list of words found and the score of the human player.*/
void humanTurn(Boggle& boggleBoard) {
    string word;
    clearConsole();
    cout << "It's your turn!" << endl;
    cout << boggleBoard; //prints the Boggle board
    cout << endl;
    while (true) {
        cout << "Your words (" << boggleBoard.getHumanWords().size() << "): " << boggleBoard.getHumanWords() << endl; //prints all the words found so far
        cout << "Your score: " << boggleBoard.humanScore() << endl; //prints the human score as calculated by the getScore() method
        word = getLine("Type a word (or Enter to stop): ");
        word = toUpperCase(word);
        if (word == "") break;
        clearConsole();
        //ERROR-CHECKING
        if (!boggleBoard.checkWord(word)) { //Word has been found already, is less than 4 letters, or isn't actually a word
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
            BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
        }  else if (!boggleBoard.humanWordSearch(word)) { //Word cannot be formed on this board
            cout << "That word can't be formed on this board." << endl;
            BoggleGUI::setStatusMessage("That word can't be formed on this board.");
        } else { //YAY! Word is found!
            cout << "You found a new word! \"" << word << "\"" << endl;
            BoggleGUI::setStatusMessage("You found a new word! \"" + toLowerCase(word) + "\"");
        }
        cout << boggleBoard;
        cout << endl;
    }
}

/*Plays one turn for the computer*/
void computerTurn(Boggle& boggleBoard) {
    cout << "It's my turn!" << endl;
    cout << "My words (" << boggleBoard.computerWordSearch().size() << "): " << boggleBoard.computerWordSearch() << endl; //Prints words found by computer that the human has missed
    int computerScore = boggleBoard.getScoreComputer();
    cout << "My score: " << computerScore << endl; //Prints total score
    if (computerScore > boggleBoard.humanScore()) { //Computer wins
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else { //Computer is a sore loser
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
    }
}

/*Asks user to input the size of the grid, from 2-6. Currently is very slow past 4.*/
int getSize() {
    int n = getInteger("Enter the size of the board: ");
    while (n < 2) {
        cout << "Board size must be 2 or greater." << endl;
        n = getInteger("Enter the size of the board: ");
    }
    return n;
}

/*Plays one game, consisting of a single human turn and computer turn, of Boggle*/
void playOneGame(Lexicon& dictionary) {
    int boggle_size = getSize(); //user sets grid size
    string boardText = getBoardText(boggle_size);
    Boggle boggleBoard(dictionary, boardText, boggle_size);

    humanTurn(boggleBoard);
    cout << endl;
    computerTurn(boggleBoard);
}
