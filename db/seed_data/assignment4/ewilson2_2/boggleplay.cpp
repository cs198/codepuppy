// This method plays the game of Boggle. It utilizes the methods defined in the Boggle class to create a Boggle object and interact with the user to play the game.

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "grid.h"
#include "bogglegui.h"

string getBoardChoice();
string boardText(string choice);
string userBoardText();
bool checkAlpha(string userString);
void humanTurn(Boggle& boggle, Set<string>& usedWords);
void computerTurn(Boggle& boggle);
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
        humanTurn(boggle, boggle.usedWords);
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

//This method asks the user if he or she wants to play another game of boggle, and returns a string with their choice.
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

//This method is called as long as it is the human's turn to play. It will prompt the user for a word, check if that word is on the board, and alert them of their progress.
void humanTurn(Boggle& boggle, Set<string>& usedWords) {
    while(true) {
        cout << "Your words: (" << usedWords.size() << "): {";
        string printWords;
        for(string word: usedWords) {
            //cout << "\"" << word << "\", ";
            if(printWords.length()>0) {
                printWords+=", \""+word+"\"";
            } else {
                printWords+="\""+word+"\"";
            }
        }
        cout << printWords;
        cout << "}" << endl;
        cout << "Your score: " << boggle.humanScore() << endl;
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
                    usedWords.add(word);
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

//This method determines whether or not the user wants to generate a random board, and returns a string with their choice
string getBoardChoice() {
    while(true) {
        cout << endl;
        string choice=getLine("Do you want to generate a random board? ");
        choice=toLowerCase(choice);
        if(choice!="y" && choice!="n") {
            cout <<"That is not a valid choice, please try again." << endl;
            BoggleGUI::setStatusMessage("That is not a valid choice, please try again.");
        } else {
            return choice;
        }
    }
}

//This method returns a string that represents the squares to be generated on the board.
//If the user wants a random board, it will return an empty string, alerting the Boggle class to randomly generate a board.
//If the user wants a user-generated board, it will return the string that they want on the board, alerting the Boggle class to incorporate that string.
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

//This method will be called if the user wants to generate the text on the board.
//It will prompt the user for a valid 16-character string, and return that string.
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

//This method will check to see if all of the characters in the string that is passed in are valid alphabetical characters
bool checkAlpha(string userString) {
    for(int i=0; i<userString.length(); i++) {
        if(isalpha(userString[i])==false) {
            return false;
        }
    }
    return true;
}

//This method plays the computer's turn. It will find all possible words on the board, update the computer score, and alert the user who the winner of the game is.
void computerTurn(Boggle& boggle) {
    Set<string> foundWords=boggle.computerWordSearch();
    cout << endl;
    cout << "It's my turn!" << endl;
    cout << "My words (" << foundWords.size() << "): {";
    string wordsComputer;
    for(string word: foundWords) {
        if(wordsComputer.length()>0) {
            wordsComputer+=", \""+word+"\"";
        } else {
            wordsComputer+="\""+word+"\"";
        }
    }
    cout << wordsComputer << "}" << endl;
    cout << "My score: " << boggle.getScoreComputer() << endl;
    cout << "human score: " << boggle.humanScore() << endl;
    cout << "computer score: " << boggle.getScoreComputer() << endl;
    if(boggle.humanScore()>=boggle.getScoreComputer()) {
        cout << "Congratulations, you win!" << endl;
        cout << endl;
    } else {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        cout<< endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    }
}
