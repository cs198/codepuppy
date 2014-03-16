// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "lexicon.h"
#include "simpio.h"
#include "Boggle.h"
#include "strlib.h"
#include "console.h"

//Function Prototypes
string determineInput();
void generateRandomBoard(Lexicon& dictionary, Boggle& board);
void generateUserInputBoard(Lexicon& dictionary, Boggle& board);
void humanTurn(Boggle& board, Stack<string> &humanWords);
void displayHumanIntroMessages(Boggle& board, Stack<string>& humanWords);
void computerTurn(Boggle& board);

// Plays one game of boggle.
void playOneGame(Lexicon& dictionary) {
    bool randomBoard = getYesOrNo("Do you want to generate a random board?");
    string boardInput = "";
    if (!randomBoard) {
        boardInput = determineInput();
    }
    Boggle board(dictionary,boardInput);
    Stack<string> humanWords;
    humanTurn (board, humanWords);
    computerTurn(board);
}

/* If the user has entered that they wish to enter a specific string instead of
 * using a randomized board, this function allows them to enter a 16 letter string
 * that contains only letters and returns the string that the user enters, if it is valid.
 */
string determineInput() {
    while (true) {
        string boardInput = getLine("Enter a string that contains 16 characters that will be on the Boggle board.");
        boardInput = toUpperCase(boardInput);
        bool validInput = true;
        if (boardInput.length() != 16) {
            validInput = false;
        }
        for (int i = 0 ; i < boardInput.length() ; i++) {
            if (boardInput[i] - 'A' < 0 || boardInput[i] - 'A' > 26) {
                validInput = false;
            }
        }
        if (validInput) {
            return boardInput;
        } else {
            cout << "That input string is not valid. Please try again." << endl;
        }
    }
}

/* Allows the player to play one turn as the human player. The human player continuously
 * enters words that they are able to find on the Boggle board and the Boggle object
 * will check whether or not the words are valid. The player is rewarded points for
 * each word they are able to find. When they cannot find anymore, they press enter
 * and the human turn is over.
 */
void humanTurn (Boggle& board,Stack<string>& humanWords) {
    while (true) {
        displayHumanIntroMessages(board, humanWords);
        string word = getLine("Type a word (or Enter to stop): ");
        if (word == "") {
            return;
        }
        word = toUpperCase(word);
        clearConsole();
        if (board.checkWord(word)) {
            if (board.humanWordSearch(word)) {
                humanWords.push(word);
                cout << "You found a new word! \"" << word << "\"" << endl;
            } else {
                cout << "That word can't be formed on this board." << endl;
            }
        } else {
            cout << "Please enter an unfound 4+ letter word from the dictionary." << endl;
        }

    }
}

// Displays the messages at the beginning of the human player's turn.
void displayHumanIntroMessages(Boggle& board, Stack<string>& humanWords) {
    if (humanWords.isEmpty()) {
        cout << "It's your turn!" << endl;
    }
    cout << board << endl;
    cout << "Your words: " << humanWords.toString() << endl;
    cout << "Your score: " << board.humanScore() << endl;
}

/*Plays a turn of Boggle as the computer. This involves the computer finding every
 * word that the human player was not able to find. The computer earns points for each
 * of these words and at the end of the computer's turn, the human and computer scores are
 * compared to determine a winner.
 */
void computerTurn(Boggle& board) {
    cout << endl << "It's my turn!" << endl;
    Set <string> result = board.computerWordSearch();
    cout << "My words (" << result.size() << ") : " << result.toString() << endl;
    cout << "My score: " << board.getScoreComputer() << endl;
    if (board.getScoreComputer() > board.humanScore()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    } else {
        cout << "WOW, you defeated me! Congratulations!" << endl;
    }
}

