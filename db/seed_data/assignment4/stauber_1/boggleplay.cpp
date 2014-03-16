//Filename: boggleplay.cpp
//Author: Mark Stauber
//Date: 2/13/14
//Description: Plays a single game of boggle. This file
//handles the initialization of a new boggle board as well
//as most of the I/O for the game.
#include "lexicon.h"
#include "simpio.h"
#include "console.h"
#include "strlib.h"

#include "Boggle.h"
#include "bogglegui.h"

using namespace std;

//Function to check if user's boardText is valid
bool checkInitLetters(string initLetters);

void playOneGame(Lexicon& dictionary) {
    //If GUI is note yet initialize, start it.
    if(!BoggleGUI::isInitialized()){
        BoggleGUI::initialize(4, 4);
        BoggleGUI::setAnimationDelay(50);
    }
    //Otherwise reset it.
    else BoggleGUI::reset();

    cout<<"Do you want to generate a random board? ";
    bool yesRand = getYesOrNo();

    string initLetters = ""; //empty string for initLetters
    if(!yesRand){
        while(true){
            cout<<"Type the 16 letters to appear on the board: ";
            initLetters = getLine();
            if(checkInitLetters(initLetters)){
                break;
            }
            cout<<"That is not a valid 16-letter board string. "
                <<"Try again."<<endl;
        }
    }
    Boggle thisGame(dictionary, initLetters);

    clearConsole();
    cout<<"It's your turn!"<<endl;
    BoggleGUI::setStatusMessage("It's your turn!");
    cout<<thisGame; //Prints board status

    //It is user's turn until they give up.
    while(true){
        cout<<"Type a word (or Enter to stop): ";
        string word = getLine();
        //If user tried another word
        if(word.length() > 0){
            //If word is valid && is found on board.
            if(thisGame.checkWord(word)){
                    if(thisGame.humanWordSearch(word)){

                    clearConsole();
                    cout<<"You found a new word! \""
                        <<toUpperCase(word)<<"\""<<endl;
                    BoggleGUI::setStatusMessage("You found a new word! \"");
                    }else{
                        clearConsole();
                        cout<<"That word can't be formed on this board."<<endl;
                        BoggleGUI::setStatusMessage(
                                    "That word can't be formed on this board.");
                    }

            }else{
                clearConsole();
                cout<<"You must enter an unfound 4+ letter "
                    <<"word from the dictionary."<<endl;
                BoggleGUI::setStatusMessage(
                            "You must enter an unfound 4+ letter word from the dictionary.");
            }
            cout<<thisGame; //Update game status output
        }else{
            //Set game to end stage. It is now computer's turn.
            thisGame.endGame();
            break;
        }
    }

    BoggleGUI::setStatusMessage("It's my turn!");
    cout<<thisGame; //Update game status output
    BoggleGUI::setStatusMessage(thisGame.gameResult());
}

//Function to check if user's initLetters contains 16 letters.
bool checkInitLetters(string initLetters){
    if(initLetters.size() != 16) return false;
    for(auto i : initLetters)
        if(!isalpha(i)) return false;
    return true;
}

