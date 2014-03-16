
#include <string>
#include "console.h"
#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "strlib.h"
#include "bogglegui.h"

string getText();
bool testForAllChars(string);
void humanTurn(Boggle&);
void computerTurn(Boggle&);

void playOneGame(Lexicon& dictionary) {
    //plays one whole game

    //BoggleGUI::reset();

    cout << endl; //skips a line at the beginning of a program (to match the output)

    Boggle board(dictionary); //initializes the boggle board
    string boardText = getText(); //gets the text for the board (empty or actual text)
    board.createBoard(boardText); //sets the text to the board (it it's empty generates a random board)

    humanTurn(board); //the human's turn

    computerTurn(board); //the computer's turn


}

string getText(){
    //gets the text from the user

    string boardText; //intializes the string
    if (getYesOrNo("Do you want to generate a random board? " , "Invalid input")){
        //returns an empty string to generate a random board
         boardText = "";
    } else {
        //gets a string and checks for validity
        boardText = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
        while (boardText.size() != 16 || !testForAllChars(boardText)){ //checks for validity (length and all chars)
            cout << "That is not a valid 16-letter board string. Try again." << endl;
            boardText = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
        }
    }
    return boardText; //returns the string
}

bool testForAllChars(string boardString){
    //tests that all the characters are alphabetic letters
    int counter = 0;
    for (int i = 0 ; i < boardString.size() ; i++){
        if (!isalpha(boardString[i])) counter++; //if it's not a letter increments a counter
    }
    if (counter == 0) {
        return true; //if the counter is still 0 all are letters
    } else{
        return false; //else at least 1 isn't a letter
    }
}

void humanTurn(Boggle& board){
    //initializes the human's turn

    cout << "It's your turn! " << endl;
    BoggleGUI::setStatusMessage("It's your turn! ");

    string word;
    do{
        //starts the turn loop
        cout << board << endl; //prints out the board
        cout << "Your words (" << board.getHumanWords().size() << "): " << board.getHumanWords() << endl; //prints out a message with the human's words
        cout << "Your score: " << board.humanScore() << endl; //prints out a message with the human's score
        BoggleGUI::setScore(board.humanScore() , BoggleGUI::HUMAN); //sets the human's score on the gui
        word = getLine("Type a word (or Enter to stop): ");
        if (word == "") { //if the user inputted an empty string
            cout << endl;
            break; //breaks out of the loop
        }
        //next part is for when the user actually put in a non-empty string
        if (board.checkWord(word)){ //if the word is valid (length, dictionary, non already been put in)

            board.setHighlightedLetters(); //resets all the letters to un-highlighted
            if (board.humanWordSearch(word)){ //if the word exists on the board

                cout << "You found a new word! \"" << toUpperCase(word) << "\"" << endl; //message saying the user found a word
                BoggleGUI::setStatusMessage("You found a new word!"); //same thing for the gui
                BoggleGUI::recordWord(word , BoggleGUI::HUMAN); //records the word on the gui
            } else{
                cout << "That word can't be formed on this board." << endl;
                BoggleGUI::setStatusMessage("That word can't be formed on this board.");
            }
        } else{
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
            BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
        }
    } while (word!="");
}

void computerTurn(Boggle& board){
    //the computer's turn

    cout << "It's my turn!" << endl;
    BoggleGUI::setStatusMessage("It's my turn!");

    //prints out the set with all the computer's word
    cout << "My words (" << board.computerWordSearch().size() << "): " << board.computerWordSearch() << endl;

    for (string word : board.computerWordSearch()){
        //records every one of the computer's word on the gui
        BoggleGUI::recordWord(word , BoggleGUI::COMPUTER);
    }

    //prints the computer's score
    cout << "My score: " << board.getScoreComputer() << endl;
    BoggleGUI::setScore(board.getScoreComputer() , BoggleGUI::COMPUTER);

    //prints out the various messages (for if the computer won or lost
    if (board.getScoreComputer() > board.humanScore()){
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else{
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");

    }

}

