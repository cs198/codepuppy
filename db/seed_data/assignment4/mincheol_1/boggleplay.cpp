// Min Cheol Kim
// Section Leader Jimmy Lee
// Section Wed 5:15
/* This assignment simulated the word game Boggle, in which players try to form words on a 4x4 grid
 * using letters that are immediately adjacent to each other (diagonally, horizontally, and vertically).
 * This assignment involved a computer player who can find all the words recursively, and essentially wins
 * every single time.
 *
 * This specfic file is the Boggleplay file. */
#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "strlib.h"
#include "hashset.h"
#include "set.h"
#include "bogglegui.h"
#include "console.h"

bool isValidInput(string input);
string chooseGameStart();
void humanTurn(Boggle& game);
void computerTurn(Boggle& game);
void initializeGUI(Boggle& test);
void printGameStatus(Boggle& game);
void validWordFound(string word, Boggle& game);
string getBoardSetupScheme();

// Constant to set the board size
const int BOARD_SIZE = 4;

/* This function takes in a dictionary and allows for one round of play between the human and the computer. The human
 * first attempts to find all the words he can, and the computer finds all of the remaining words. */
void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(BOARD_SIZE,BOARD_SIZE);
    Boggle test(dictionary, chooseGameStart());
    BoggleGUI::setAnimationDelay(100);
    initializeGUI(test);
    humanTurn(test);
    computerTurn(test);
}

/* This function initializes the gui with appropriate letters in each of the squares, given by the game board of the
 * Boggle classes whose reference was passed in. */
void initializeGUI(Boggle& test) {
    string cubes = "";
    for (int n = 0; n < BOARD_SIZE; n++)
        for (int m = 0; m < BOARD_SIZE; m++) {
            cubes += test.getLetter(n,m);
        }
    BoggleGUI::labelAllCubes(cubes);
}

/* Checks if the player-inputted scheme for setting up the board is valid - checks if the length is 16 letters and
 * only contains the alphabet. */
bool isValidInput(string input) {
    bool flag = true;
    if (input.length() != 16)
        flag = false;
    for (int n = 0; n < input.length(); n++) {
        char ch = input[n];
        if ( !((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A')) )
            flag = false;
    }
    return flag;
}

/* Asks the player if they want to use a randomly generated board or a custom made board. A word starting with a "y"
 * is taken as a yes and a word starting with a "n" is taken as a no. The function returns a 16 character string for
 * manually set up command and an empty string for a randomly set up command. */
string chooseGameStart() {
    string input;
    string ifRandom;
    if (getYesOrNo("Do you want to generate a random board?",
                   "Please type a word that begins with 'y' or 'n'."))
        input = "";
    else
        input = getBoardSetupScheme();
    return input;
}

/* Plays the human turn of the game. Takes in the Boggle class representing the state
 * of the game. Checks if the human-attempted words are valid and can really be found on the grid; if yes, then the
 * word is added to the list of words the player found and the score is increased. Pressing enter ends the human's
 * turn. */
void humanTurn(Boggle& game) {
    cout << "It's your turn!" << endl;
    while (true) {
        printGameStatus(game);
        string possibleWord = getLine("Type a word (or Enter to stop): ");
        if (possibleWord == "")
            break;
        else if ( !game.checkWord(possibleWord)  ) {
            clearConsole();
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        }
        else {
            BoggleGUI::clearHighlighting();
            string word = toUpperCase(possibleWord);
            if (game.humanWordSearch(word)) {
                clearConsole();
                validWordFound(word, game);
            }
            else {
                clearConsole();
                cout << "That word can't be formed on this board." << endl;
            }
        }
    }
}

/* Plays the computer's turn of the game. The computer simply finds all of the words over four leters that can be
 * found on the word, prints them out, and declares victory. */
void computerTurn(Boggle& game) {
    BoggleGUI::clearHighlighting();
    cout << "It's my turn!" << endl;
    Set<string> computerWords = game.computerWordSearch();
    for (string s : computerWords)
        BoggleGUI::recordWord(s, BoggleGUI::COMPUTER);
    BoggleGUI::setScore(game.getScoreComputer(), BoggleGUI::COMPUTER);
    cout << "My words (" << computerWords.size() << "): " << computerWords << endl;
    cout << "My score: " << game.getScoreComputer() << endl;
    if (game.getScoreComputer() > game.getHumanScore())
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    else
        cout << "WOW, you defeated me! Congratulations!" << endl;
}

/* Prints the state of the game - the board itself, the words the human found so far, and the score of the human. */
void printGameStatus(Boggle& game) {
    cout << game << endl;
    cout << "Your words (" << game.getAlreadyUsed().size() << "): " << game.getAlreadyUsed().toString() << endl;
    cout << "Your score: " << game.getHumanScore() << endl;
}

/* Actions for when a human finds a valid word - add to the already found list, increase score, and carry out
   appropriate graphic changes. */
void validWordFound(string word, Boggle& game) {
    cout << "You found a new word! " << "\"" << word << "\"" << endl;
    game.addToAlreadyUsed(word);
    game.increaseHumanScore(word);
    BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
    BoggleGUI::setScore(game.getHumanScore(), BoggleGUI::HUMAN);
}

/* Repeated asks for a valid 16 letter string that could be used to set up the board. */
string getBoardSetupScheme() {
    string possibleInput;
    string input;
    while (true) {
        possibleInput = getLine("Type the 16 letters to appear on the board: ");
        if (isValidInput(possibleInput)) {
             input = toUpperCase(possibleInput);
            break;
        }
        cout << "That is not a valid 16-letter board string. Try again." << endl;
    }
    return input;
}
