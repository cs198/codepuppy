// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "console.h"
#include "simpio.h"
#include "Boggle.h"
using namespace std;

//These are the prototype methods
string getBoardText();
bool isOnlyLetters(string str);
void playHumanTurn(Boggle& boggler);
void printHumanRoundInfo(Boggle& boggler);
void printCompRoundInfo(Boggle& boggler, Set<string>& result);

/*
 * This method plays one game of Boggle. First, the program determines if the user wants to input a string
 * of characters to be used for the game board or not. After this is determined, a boggle object is created.
 * After this, the human's round is played. After that, the computer searches the board to find all the
 * remaining words on the board and stores it in a string. Then, the computer's round info is printed.
 */
void playOneGame(Lexicon& dictionary) {
    string boardText = "";
    if(!getYesOrNo("Do you want to generate a random board? ",
                   "Please type a word that begins with 'y' or 'n'.")) {
        boardText = getBoardText();
    }
    Boggle boggler(dictionary, boardText);
    playHumanTurn(boggler);
    cout << endl;
    cout << "It's my turn!" << endl;
    Set<string> result = boggler.computerWordSearch();
    printCompRoundInfo(boggler, result);
}

/*
 * This method handles the round of the human player. It prints the dice board in the console and prompts
 * the user for a guess word. If this guess is an empty string, the round ends. If not, the method checks
 * if the guessed word is a playable word and if it is contained in the board. If these are both true,
 * then the word is added to the GUI, the player's score is incrimented, and a message is printed to the
 * user. If not, the method prompts the user to enter a valid, playable word.
 */
void playHumanTurn(Boggle& boggler) {
    clearConsole();
    cout << "It's your turn!" << endl;
    while(true) {
        cout << boggler << endl;
        printHumanRoundInfo(boggler);
        cout << "Type a word (or Enter to Stop): ";
        string userGuess;
        getLine(cin, userGuess);
        if(userGuess == "") break;
        userGuess = toUpperCase(userGuess);
        if(boggler.checkWord(userGuess) && boggler.humanWordSearch(userGuess)) {
            clearConsole();
            boggler.addFoundWord(userGuess);
            cout << "You found a new word! \"" << userGuess << "\"" << endl;
        } else {
            clearConsole();
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        }
    }
}

/*
 * This method prints the human player's word count, found words, and score.
 */
void printHumanRoundInfo(Boggle& boggler) {
    cout << "Your words(" << boggler.getHumanFoundWords().size() << "): "
         << boggler.getHumanFoundWords().toString() << endl;
    cout << "Your score: " << boggler.humanScore() << endl;
}

/*
 * This method prints the word count and score of the computer player's round. If the computer player's
 * score is greater than the human's score, then a message telling the user that the computer has won.
 * If this is not true, then a message declaring that the user has won is printed.
 */
void printCompRoundInfo(Boggle& boggler, Set<string>& result) {
    cout << "My words (" << result.size() << "): " << result.toString() << endl;
    cout << "My score :" << boggler.getScoreComputer() << endl;
    if (boggler.getScoreComputer() > boggler.humanScore()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    } else {
        cout << "WOW, you defeated me! Congratulations!" << endl;
    }
}

/*
 * This method prompts the user to input a string of characters that will be used to fill the diceBoard
 * grid and the GUI dice board. The method recieves the user's input, changes it to uppercase, and checks
 * if it is a valid string of characters. Once a valid string is inputed, it is returned.
 */
string getBoardText() {
    string letters;
    while(true) {
        cout << "Type the 16 letters to appear on the board: ";
        getLine(cin, letters);
        letters = toUpperCase(letters);
        if(letters.size() == 16 && isOnlyLetters(letters)) {
            break;
        } else {
            cout << "That is not a valid 16-letter board string. Try again." << endl;
        }
    }
    return letters;
}

/*
 * This method checks if a string passed to the method only contains uppercase letters and returns true
 * if it is.
 */
bool isOnlyLetters(string str) {
    for(int i = 0; i < str.size(); i++) {
        if (str[i] < 'A' || str[i] > 'Z') return false;
    }
    return true;
}
