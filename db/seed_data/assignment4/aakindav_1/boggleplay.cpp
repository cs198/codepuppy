/* Charles Akin-David
* CS106B
* SL: Jimmy Lee
* Sources used: Lecture Slides, LaIR
* This program plays the game of Boggle using the
* created Bogle class.
* Decomposition style chosen was to use inner comments rather than
* modular decomposition since the only thing here is loops or checks
* depending on what the user wants to do. It flows better all under
* one play game function.
*/

#include "lexicon.h"
#include "simpio.h"
#include "Boggle.h"
#include "strlib.h"
#include "bogglegui.h"

void playOneGame(Lexicon& dictionary) {
    // initializes the GUI gameboard and sets the highlight delay
    BoggleGUI::initialize(4,4);
    BoggleGUI::setAnimationDelay(150);
    cout<<endl;
    string randomBoard = toLowerCase(getLine("Do you want to generate a random board? "));
    Boggle game (dictionary);
    // checks whether the user wants to enter text for the boggle board or not
    while (true) {
        // questions the user until they enter yes, y, no, or n
        if (randomBoard != "yes" && randomBoard != "y" && randomBoard != "n" && randomBoard != "no") {
            cout << "Please enter yes/y or no/n!" << endl;
            randomBoard = toLowerCase(getLine("Do you want to generate a random board? "));
        }
        else break;
    }
    // if the choose to enter the words in the boggle board will be reset with text
    if (randomBoard == "n" || randomBoard == "no") {
        string boardLetters = "";
        // questions the user for text until it is 16 characters long
        while (true) {
            boardLetters = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
            if (boardLetters.size() != 16) cout << "That is not a valid 16-letter board string. Try again." << endl;
            else break;
        }
        game.resetWithWords(boardLetters);
    }
    // loops through the board, getting every character and storing it in a string
    string ranText = "";
    for (int r=0; r<4; r++) {
        for (int c=0; c<4; c++) {
            ranText += game.getLetter(r,c);
        }
    }
    // labels all the cubes, clears console and allows user to pick words
    // clears console to start the game
    BoggleGUI::labelAllCubes(ranText);
    clearConsole();
    BoggleGUI::setStatusMessage("It's your turn!");
    cout << "It's your turn!" << endl;
    cout << game << endl;
    // pulls out the set of words the user has guess correctly
    Set<string> humanWords = game.getSet();
    // shows the users words and score
    cout << "Your words (" << humanWords.size() << "): " << humanWords.toString() << endl;
    cout << "Your score: " << game.getHumanScore() << endl;
     // continuously prompts user to enter a word
    while (true) {
        string word = toUpperCase(getLine("Type a word (or Enter to stop): "));
        // if the user enters nothing then they are done and the computer starts it's turn
        if (word == "") {
            cout << endl;
            BoggleGUI::setStatusMessage("It's my turn!");
            cout << "It's my turn!" << endl;
            // pulls out all the valid words the computer finds
            Set<string> compSearch = game.computerWordSearch();
            // updates the GUI with the computers words and score
            BoggleGUI::setScore(game.getScoreComputer(), BoggleGUI::COMPUTER);
            for (string words:compSearch) {
                BoggleGUI::recordWord(words,BoggleGUI::COMPUTER);
            }
            // shows the computers found words and score
            cout << "My words (" << compSearch.size() << "): "<< compSearch.toString() << endl;
            cout << "My score: " << game.getScoreComputer() << endl;
            // checks to see who got the higher score then prints the game result message
            // also puts the game result message on the GUI
            if (game.getScoreComputer() > game.getHumanScore()) {
                BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
                cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
            } else {
                BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
                cout << "WOW, you defeated me! Congratulations!" << endl;
            }
            break;
        }
        // clears console
        // if the user enters an invalid word a error message is shown
        // this word is either less than 4 characters, not an english word, or an already used word
        // also prints error message in the GUI
        if (!game.checkWord(word)) {
            clearConsole();
            BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        }
        // clears console
        // if the user enters a valid word that can't be found in the word an error message is displayed
        // error message is also displayed in the GUI
        // also the GUI shows the process of looking for the word
        else if (!game.humanWordSearch(word)) {
            clearConsole();
            BoggleGUI::labelAllCubes(ranText);
            BoggleGUI::setStatusMessage("That word can't be formed on this board.");
            cout << "That word can't be formed on this board." << endl;
        }
        // clears console
        // here the user enters a valid word that can be fund on the board
        // the process of finding the word is shown on the GUI
        // shows the user's new found word in the GUI
        // messgae telling the user that they found a new word is shown in the console and GUI
        else {
            clearConsole();
            BoggleGUI::labelAllCubes(ranText);
            BoggleGUI::setStatusMessage("You found a new word! \"" + word + "\"");
            BoggleGUI::recordWord(word,BoggleGUI::HUMAN);
            cout<<"You found a new word! \"" << word << "\"" << endl;
        }
        // pulls out human word set again
        // sets the User score in the console and in the GUI
        // this will always be done is the user enters a valid word
        cout << game << endl;
        Set<string> humanWords = game.getSet();
        BoggleGUI::setScore(game.getHumanScore(), BoggleGUI::HUMAN);
        cout << "Your words (" << humanWords.size() << "): " << humanWords.toString() << endl;
        cout << "Your score: " << game.getHumanScore() << endl;
    }
}
