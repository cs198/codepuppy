    // This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "console.h"
#include "set.h"
#include "grid.h"
#include "bogglegui.h"

/*Function Prototypes*/
void getBoardStr(string& gameBoardStr);
void playHumanGame(Boggle& boggle);
void playComputerGame(Boggle& boggle);
void printInitialBoard(Boggle& boggle);
void printCurBoard(Boggle& boggle);
void checkWordType(bool isFound, Boggle& boggle, string userWord);
bool validString(string gameBoardStr);
void displayGUIComputerWords(Boggle& boggle);
void setComputerMessage(Boggle& boggle);
void printComputerStats(Boggle& boggle);

/*
 * Plays one game of boggle by creating the boggle board with a string of 16 characters.
 * Prints the initial board and the messages that correspond with it.
 * Then respectively allows the human and computer to play.
 */
void playOneGame(Lexicon& dictionary) {
    string gameBoardStr = "";
    getBoardStr(gameBoardStr);
    Boggle boggle (dictionary, toUpperCase(gameBoardStr));
    printInitialBoard((boggle));
    playHumanGame(boggle);
    playComputerGame(boggle);
}

/*
 * Prints the intitial display of the game on the console.
 */
void printInitialBoard(Boggle& boggle){
    cout << "It's your turn!" << endl;
    printCurBoard(boggle);
}

/*
 * Prints the boggle board along with the user's current words found and score.
 */
void printCurBoard(Boggle& boggle){
    cout << boggle << endl;
    cout << "Your words (" << boggle.foundWordsSet().size() << "): " << boggle.foundWordsSet().toString() << endl;
    cout << "Your score: " << boggle.humanScore() << endl;
}

/*
 * Simulates the computer portion of the game.
 * Prints the computer stats and messages to the GUI and the console after the computer has found all the words.
 */
void playComputerGame(Boggle& boggle) {
    printComputerStats(boggle);
    setComputerMessage(boggle);
    displayGUIComputerWords(boggle);
}

/*
 * Prints all of the words that the computer has found and the computer's final score to the console and the GUI.
 */
void printComputerStats(Boggle& boggle){
    cout << endl;
    cout << "It's my turn!" << endl;
    cout << "My words: (" << boggle.computerWordSearch().size() << "): " << boggle.computerWordSearch().toString() << endl;
    cout << "My score: " << boggle.computerScore() << endl;
    BoggleGUI::setScore(boggle.computerScore(), BoggleGUI::COMPUTER);
}

/*
 * Prints either a victory or defeat message based on the computer's score and the human's score.
 */
void setComputerMessage(Boggle& boggle){
    if(boggle.computerScore() > boggle.humanScore()){
        string victory = "Ha ha ha, I destroyed you. Better luck next time, puny human!";
        BoggleGUI::setStatusMessage(victory);
        cout << victory << endl;
    } else {
        string defeat = "WOW, you defeated me! Congratulations!";
        BoggleGUI::setStatusMessage(defeat);
        cout <<  defeat << endl;
    }
}

/*
 * Displays all of the words that the computer found onto the GUI.
 */
void displayGUIComputerWords(Boggle& boggle){
    for(string s: boggle.computerWordSearch()){
        BoggleGUI::recordWord(s, BoggleGUI::COMPUTER);
    }
}

/*
 * Simulates the human playing part of Boggle.
 * The user can repeatedly guess words until they enter an empty string (space), which exits the human portion of the game.
 */
void playHumanGame(Boggle& boggle){
    string userWord = toUpperCase(getLine("Type a word (or Enter to stop): "));
    while(userWord != "") {
        if(boggle.checkWord(userWord)){
            BoggleGUI::clearHighlighting();
        }
        bool isFound = boggle.humanWordSearch(userWord);
        clearConsole();
        checkWordType(isFound, boggle, userWord);
        printCurBoard(boggle);
        BoggleGUI::setScore(boggle.humanScore(), BoggleGUI::HUMAN);
        userWord = toUpperCase(getLine("Type a word (or Enter to stop): "));
    }
    BoggleGUI::clearHighlighting();
}

/*
 * Prints the respective messages to the GUI or the console based on whether the word has been found, and whether the word is 4 or more letters long.
 */
void checkWordType(bool isFound, Boggle& boggle, string userWord){
    if(isFound) {
        string foundWord = "You found a new word! \"";
        cout << foundWord << userWord << "\"" << endl;
        BoggleGUI::setStatusMessage(foundWord + toLowerCase(userWord) + "\"");
        BoggleGUI::recordWord(userWord, BoggleGUI::HUMAN);
    } else if(userWord.length() < 4 || !boggle.checkWord(userWord)) {
        cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
    } else {
        cout << "That word can't be formed on this board." << endl;
        BoggleGUI::clearHighlighting();
    }
}

/*
 * Returns true if the use enters a valid 16 character string with no numbers, punctuation or spaces.
 */
bool validString(string gameBoardStr){
    if(gameBoardStr.length() != 16) {
        return false;
    } else {
        for(int i = 0; i < gameBoardStr.size(); i++){
            char c = gameBoardStr[i];
            if(isdigit(c) || ispunct(c) || isspace(c)){
                return false;
            }
        }
    }
    return true;
}

/*
 * Reads in a string from the user that will be used to create the Boggle board.
 */
void getBoardStr(string& gameBoardStr) {
    if(!getYesOrNo("Do you want to generate a random board? ")) {
        gameBoardStr = getLine("Type the 16 letters to appear on the board: ");
        while(!validString(gameBoardStr)){
            cout << "That is not a valid 16-letter board string. Try again." << endl;
            gameBoardStr = getLine("Type the 16 letters to appear on the board: ");
        }
    }
    clearConsole();
}



