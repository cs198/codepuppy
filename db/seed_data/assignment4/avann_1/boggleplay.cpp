// This is the Boggle Game Play cpp file which is responsible for
// communicating with the Boggle class to allow a user to play the
// Boggle Game.
// For this assignment, I used the powerpoints and lecture notes online
// and received help from the LAIR.

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "console.h"
#include "bogglegui.h"
#include <cctype>

Boggle createBoggleBoard(Lexicon& dictionary);
void printBoggleBoard(Grid<char>& boggleBoard);
void checkingWord(string word, Boggle& boggleGame);
void computerOutcome(Boggle& boggleGame, int& computerScore);
void determineWinner(int humanScore, int computerScore);

/* playOneGame initializes the Boggle board GUI and creates a Boggle Board
 * object. It is responsible for the execution of the entire Boggle game,
 * including listing the words the user has guessed correctly, outputting
 * the score, getting valid words from the user, having the computer find
 * all the possible words, and determining the winner.
 */
void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(4, 4);

    Boggle boggleGame = createBoggleBoard(dictionary);

    cout << boggleGame << endl;

    clearConsole();

    cout << "It's your turn!" << endl;
    cout << boggleGame << endl;
    cout << endl;

    int humanScore;
    int computerScore;

    while(true){

        cout << "Your words: " << "(" << (boggleGame.getHumanWords()).size() << "): " << boggleGame.getHumanWords() << endl;
        humanScore = boggleGame.humanScore();
        BoggleGUI::setScore(humanScore, BoggleGUI::HUMAN);
        cout << "Your score: " << humanScore << endl;

        string word = getLine("Type a word (or Enter to stop): ");
        if(word=="") break;
        word = toUpperCase(word);

        clearConsole();

        checkingWord(word, boggleGame);

        cout << boggleGame << endl;
        cout << endl;
    }

    computerOutcome(boggleGame, computerScore);

    determineWinner(humanScore, computerScore);
}

/* createBoggleBoard asks the user if they would like to generate a random
 * board. If they decide to not input a random board, then it asks the user
 * for 16 characters, and ensures that the characters the user inputs forms
 * a valid Boggle board.
 */
Boggle createBoggleBoard(Lexicon& dictionary){
    bool generate = getYesOrNo("Do you want to generate a random board? ");
    string boardText="";

    if(!generate) {
        boardText = getLine("Type the 16 letters to appear on the board: ");
        if(boardText != ""){
            bool valid = false;
            while(valid==false){
                valid = true;

                if(boardText.length()!=16){
                    valid=false;
                }

                for(int i=0; i<boardText.length(); i++){
                    if(isalpha(boardText[i])==false){
                        valid = false;
                    }
                }

                if(valid) break;

                cout << "That is not a valid 16-letter board string. Try again." << endl;
                boardText = getLine("Type the 16 letters to appear on the board: ");
            }
        }
    }

    Boggle boggleGame(dictionary, toUpperCase(boardText));

    return boggleGame;
}

/* checkingWord ensures the user inputs a valid Boggle word for the current game.
 * When the word is valid, it adds it the list of words found. If not, it alerts
 * the user that it is not valid.
 */
void checkingWord(string word, Boggle& boggleGame){
    if(boggleGame.checkWord(word)){
        bool present = boggleGame.humanWordSearch(word);
        if(present){
            boggleGame.updateHumanWords(word);
            cout << "You found a new word: "<< "\"" << word << "\"" << endl;
            BoggleGUI::setStatusMessage("You found a new word");
            BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
        }else{
            cout << "That word can't be formed on this board." << endl;
            BoggleGUI::setStatusMessage("That word can't be formed on this board.");
        }
    }else{
        cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
    }
}

/* computerOutcome has the computer find all possible valid words on the Boggle
 * board and adds these to result to be printed to the console and to the GUI.
 * It also calculated the computer's score.
 */
void computerOutcome(Boggle& boggleGame, int& computerScore){
    cout << "It's my turn!" << endl;
    Set<string> result = boggleGame.computerWordSearch();
    cout << "My words: " << "(" << result.size() << "): " <<  result << endl;
    computerScore = boggleGame.getScoreComputer();
    BoggleGUI::setScore(computerScore, BoggleGUI::COMPUTER);
    cout << "My score: " << computerScore << endl;
}

/* determineWinner takes the users score and the computer's score and
 * determines who has won the game. If they tie, the user wins.
 */
void determineWinner(int humanScore, int computerScore){
    if(computerScore > humanScore){
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    }else{
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");

    }
}
