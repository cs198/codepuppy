// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "lexicon.h"
#include "shuffle.h"
#include "Boggle.h"
#include "simpio.h"
#include "grid.h"
#include "hashset.h"
#include <string.h>
//#include "strlib.h"

string getCustomBoardInput();

void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(4, 4);
    Set<string> wordsEntered; //keeps track of words user has entered successfully.
    cout << endl;
    bool randomBoard = getYesOrNo("Do you want to generate a random board?");
    string boardInput = "";
    if (randomBoard == false) {
        boardInput = getCustomBoardInput();
    }
    Boggle boggleBoard(dictionary, boardInput); //initializes boggleBoard
    clearConsole();
    cout << "It's your turn!" << endl;
    BoggleGUI::setStatusMessage("It's your turn!");
    cout << boggleBoard;
    while (true) {
        cout << endl << "Your words (" << wordsEntered.size() << "): " << wordsEntered.toString() << endl;
        cout << "Your score: " << boggleBoard.humanScore() << endl;
        string line = getLine("Type a word (or Enter to stop):");
        cout << endl;
        line = toUpperCase(line);
        if (line == "") {
            break;
        }
        if (boggleBoard.checkWord(line)) { //if word is long enough (greater than 4 letters), word is in dictionary, and word hasn't been entered before.
            bool wordFound = boggleBoard.humanWordSearch(line);
            clearConsole();
            if (wordFound) {
                wordsEntered.add(line);
                BoggleGUI::setStatusMessage("You found a new word! \"" + line + "\"");
                cout << "You found a new word!\"" << line << "\"" << endl;
                BoggleGUI::setScore(boggleBoard.humanScore(), BoggleGUI::HUMAN);
            }
            if (!wordFound) {
                BoggleGUI::setStatusMessage("That word can't be formed on this board.");
                cout << "That word can't be formed on this board." << endl;
            }
            cout << boggleBoard;
        }
        else {
            clearConsole();
            BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
            cout << boggleBoard;
        }
    }
    //Computer's turn
    cout << "It's my turn!" << endl;
    Set<string> computerWords = boggleBoard.computerWordSearch();
    BoggleGUI::setScore(boggleBoard.getScoreComputer(), BoggleGUI::COMPUTER);
    cout << "My words (" << computerWords.size() << "):" << computerWords << endl;
    cout << "My score: " << boggleBoard.getScoreComputer() << endl;
    if (boggleBoard.getScoreComputer() > boggleBoard.humanScore()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    }
    if (boggleBoard.getScoreComputer() < boggleBoard.humanScore()) {
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");

    }
}

//When user prompts to create own gameboard, this method allows user to input 16 characters to configure the grid.
string getCustomBoardInput() {
    while(true) {
        string boardInput = getLine("Type the 16 letters to appear on the board: ");
        if (boardInput.size() == 16) {
            boardInput = toUpperCase(boardInput);
            return boardInput;
        }
        else cout << "That is not a valid 16-letter board string. Try again." << endl;
    }
}
