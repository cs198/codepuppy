// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "grid.h"
#include "bogglegui.h"

string getBoardChoice();
string boardText(string choice);
string userBoardText();
bool checkAlpha(string userString);
void humanTurn(Boggle boggle);
void computerTurn(Boggle boggle);
string askPlayAgain();

void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(4, 4);
    BoggleGUI::setAnimationDelay(50);
    while (true) {
        string choice=getBoardChoice();
        string text=boardText(choice);
        Boggle boggle=Boggle(dictionary, text);
        cout << "It's your turn!" << endl;
        BoggleGUI::setStatusMessage("It's your turn!");
        cout << boggle << endl;
        humanTurn(boggle);
        computerTurn(boggle);
        string playAgain=askPlayAgain();
        if(playAgain=="N") {
            cout << "Have a nice day." << endl;
            BoggleGUI::setStatusMessage("Have a nice day.");
            BoggleGUI::shutdown();
            break;
        } else {
            BoggleGUI::reset();
        }
    }
}

string askPlayAgain() {
    while (true) {
        BoggleGUI::setStatusMessage("Play again (Y/N)? ");
        string playAgain=getLine("Play again (Y/N)? ");
        if(playAgain!="y" && playAgain!="Y" && playAgain!="n" && playAgain!="N") {
            BoggleGUI::setStatusMessage("That is not a valid choice, please try again.");
            cout << "That is not a valid choice, please try again." << endl;
        } else {
            playAgain=toUpperCase(playAgain);
            return playAgain;
        }
    }
}

void humanTurn(Boggle boggle) {
    while(true) {
        int score=boggle.humanScore();
        cout << "Your words: (" << boggle.usedWords.size() << "): {";
        string printWords;
        for(string word: boggle.usedWords) {
            cout << "\"" << word << "\", ";
        }
        /*if(printWords.length()>0) {
            printWords=printWords.substr(0, printWords.length()-2);
        }
        cout << printWords;*/
        cout << "}" << endl;
        cout << "Your score: " << score << endl;
        //BoggleGUI::setStatusMessage("Type a word (or Enter to stop): ");
        string word=getLine("Type a word (or Enter to stop): ");
        if(word=="") {
            break;
        } else {
            word=toUpperCase(word);
            if(boggle.checkWord(word)) {
                bool found=boggle.humanWordSearch(word);
                if(found) {
                    cout << "You found a new word! \""<< word << "\"" << endl;
                    BoggleGUI::setStatusMessage("You found a new word!");
                    score++;
                    boggle.usedWords.add(word);
                    cout << boggle << endl;
                } else {
                    cout << "That word can't be formed on this board." << endl;
                    BoggleGUI::setStatusMessage("That wod can't be formed on this board.");
                    cout << boggle << endl;
                }

            } else {
                cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
                BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
                cout << boggle << endl;
            }
        }
    }
}

string getBoardChoice() {
    while(true) {
        cout << endl;
        string choice=getLine("Do you want to generate a random board? ");
        BoggleGUI::setStatusMessage("Do you want to generate a random board? ");
        choice=toLowerCase(choice);
        if(choice!="y" && choice!="n") {
            cout <<"That is not a valid choice, please try again." << endl;
            BoggleGUI::setStatusMessage("That is not a valid choice, please try again.");
        } else {
            return choice;
        }
    }

}

string boardText(string choice) {
    string boardText;
    if(choice=="y") {
        boardText="";
    }
    if(choice=="n") {
        boardText=userBoardText();
        BoggleGUI::labelAllCubes(boardText);
    }
    return boardText;
}

string userBoardText() {
    while(true) {
        string userString=getLine("Type the 16 letters to appear on the board: ");
        if(userString.length()!=16) {
            cout << "That is not a valid 16-letter board string. Try again." << endl;
        } else if(checkAlpha(userString)==false) {
            cout << "That is not a valid 16-letter board string. Try again." << endl;
        } else {
            for(int i=0; i<userString.length(); i++) {
                userString=toUpperCase(userString);
            }
            return userString;
        }
    }
}

bool checkAlpha(string userString) {
    for(int i=0; i<userString.length(); i++) {
        if(isalpha(userString[i])==false) {
            return false;
        }
    }
    return true;
}

void computerTurn(Boggle boggle) {
    Set<string> foundWords=boggle.computerWordSearch();
    cout << endl;
    cout << "It's my turn!" << endl;
    cout << "My words (" << foundWords.size() << "): ";
    for(string word: foundWords) {
        cout << word << " ";
    }
    cout << endl;
    cout << "My score: " << boggle.getScoreComputer() << endl;
    cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
}
