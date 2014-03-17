//Name: Benzheng Zhu
//CS 106B Assignment 4
//Citation: lecture slides
//This is a Boggle Game program, which reads the initial state of grid from the given .txt file
//and models the life cycle of bacteria from one generation to another.

#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "Boggle.h"
#include "console.h"
#include "set.h"
#include "bogglegui.h"

void typeLetters(string& letters);
void clearHighlight();

void playOneGame(Lexicon& dictionary) {
    if(BoggleGUI::isInitialized()) {
        BoggleGUI::reset();
    } else {
        BoggleGUI::initialize(4, 4);
    }

    string letters = "";
    cout << endl;
    bool choice = getYesOrNo("Do you want to generate a random board? ");
    if (!choice) {
        typeLetters(letters);
    }
    Boggle boggle(dictionary,letters);
    letters = boggle.getText();

    clearConsole();
    string str = ""; //this string records the status message

    //user input words
    str = "It's your turn!";
    cout<< str<< endl;
    BoggleGUI::setStatusMessage(str);

    while(true) {
        boggle.printBoard();
        cout<<endl<<"Your words ("<<boggle.humanList().size()<<"): "<<boggle.humanList()<<endl;
        cout <<"Your score: "<<boggle.humanScore()<<endl;
        string word = toUpperCase(getLine("Type a word (or Enter to stop): "));

        BoggleGUI::labelAllCubes(letters);//All cubes are un-highlighted

        if (word=="") {
            break; //end of human's turn
        } else {
            clearConsole(); //clear console
            if (!boggle.checkWord(word)) {
                str =  "You must enter an unfound 4+ letter word from the dictionary.";
            } else if (!boggle.humanWordSearch(word)) {
                str = "That word can't be formed on this board.";
            } else {
                str = "You found a new word! ";
                str += "\""+word+"\"";
            }
            cout << str << endl;
            BoggleGUI::setStatusMessage(str);
        }
    }

    //conputer search for remaining words
    str = "It's my turn!";
    cout<< endl<<str<< endl;
    BoggleGUI::setStatusMessage(str);

    boggle.computerWordSearch();
    cout<<"My words (" <<boggle.computerList().size() <<"): "<<boggle.computerList()<<endl;
    cout<< "My score: "<<boggle.computerScore()<<endl;

    //compare scores
    if (boggle.computerScore() > boggle.humanScore()) {
        cout<<"Ha ha ha, I destroyed you. Better luck next time, puny human!"<<endl;
    } else {
        cout<<"WOW, you defeated me! Congratulations!"<<endl;
    }
}

void typeLetters(string& letters){
    //takes a string as input, checks the validity of typed letters, and stores them in the string
    //called when the user chooses to type the letters manually

    while (true) {
        letters = toUpperCase(getLine("Type the 16 letters to appear on the board: "));

        //check if any char is not a letter
        bool alpha = true;
        for (int i=0; i<letters.size(); i++) {
            alpha = isalpha(letters[i]);
            if (!alpha) {
                break;
            }
        }
        if (letters.size() != 16 || !alpha) {
            cout << "That is not a valid 16-letter board string. Try again."<<endl;
        } else {
            break;
        }
    }
}

