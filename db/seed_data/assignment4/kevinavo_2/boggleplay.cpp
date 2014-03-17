/* Name: Kevin Vo
 * TA: Jimmy Lee
 * This is the cpp file that implements the turn based system
 *of the Boggle game. There are two major components, the human
 * turn and the computer turn.
*/

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "strlib.h"
#include "console.h"
#include "bogglegui.h"

/* This function is in charge of executing the human turn of the game.
 * The strategy is to use a while true that exits when the user inputs
 * a blank string. I keep track of the used words, the score and print
 * them to the console screen.
*/
void humanTurn(Boggle& boggle){
    string userInput;
    Vector <string> successWords;
    string header = "It's your turn!";
    int score = 0;
    while (true){
        clearConsole();
        cout << header << endl;
        cout << boggle << endl;
        cout << endl;
        cout << endl;
        cout << "Your words " << "(" << successWords.size() << "): " << successWords << endl;
        cout << "Your score: " << boggle.getHumanScore() << endl;
        BoggleGUI::setScore(boggle.getHumanScore(),BoggleGUI::HUMAN);
        userInput = getLine("Type a word (or Enter to stop): ");
        userInput = toUpperCase(userInput);
        if (boggle.checkWord(userInput) == false){
            header = "You must enter an unfound 4+ letter word from the dictionary.";
        }else if (boggle.humanWordSearch((userInput))){
            BoggleGUI::recordWord(userInput,BoggleGUI::HUMAN);
            boggle.setUsedWords(userInput);
            successWords.add(userInput);
            score += (userInput.length() - 3);
            boggle.setHumanScore(score);
            header = "You found a new word! " + userInput;
        }else{
            header = "word cannot be created on this board.";
        }
        if (userInput == "") break;
    }
    return;
}
/* This function is responsible for the computer turn, which uses recursion to
 * find all the possible words on the board. I use a set to collect all the
 * words that the computer finds. It also handles the case where the user
 * actually beats the computer
*/
void computerTurn(Boggle& boggle){
    Set<string> computerAnswers;
    cout << "It's my turn!" << endl;
    computerAnswers = boggle.computerWordSearch();
    cout << "My words " << "(" << computerAnswers.size() << ") : ";
    cout << computerAnswers << endl;
    cout << "My score: " << boggle.getScoreComputer() << endl;
    if (boggle.getHumanScore() > boggle.getScoreComputer()){
        cout << "Wow you defeated me congratulations" << endl;
    }else{
        cout << "Ha ha ha, I destoryed you. Better luck nest time, puny human!" << endl;
    }
}
/* This is a boolean function that iterates through a string that the user inputs
 * for generating a non-random board. If the string is not all comprised of letters
 * then it will return false.
*/
bool allLetters (string sequence){
    for (int i = 0; i < sequence.length(); i++){
        if (!isalpha(sequence[i])){
            return false;
        }
    }
    return true;
}
/* This the "main" function that handles one turn of play. It gives the user two
 * options, a random board or to generate a new board, which is represented by
 * the condition == 'y' and == 'n'.
*/
void playOneGame(Lexicon& dictionary) {
    string randomOption;
    string boardText;
    while (true){
        cout << endl;
        randomOption = getLine("Do you want to generate a random board? (Y/N): ");
        randomOption = toLowerCase(randomOption);
        if (randomOption[0] == 'y' || randomOption[0] == 'n') break;
        cout << "Please type a word that begins with 'y' or 'n'." << endl;
    }
    if (randomOption[0] == 'y'){
        Boggle boggle(dictionary, boardText);
        humanTurn(boggle);
        computerTurn(boggle);
    }else if (randomOption[0] == 'n'){
        while (true){
            boardText = getLine("Type the 16 letters to appear on the board: ");
            if (boardText.length() == 16 && allLetters(boardText)) break;
            cout << "Not a valid 16 letter sequence. Please try again." << endl;
        }
        Boggle boggle(dictionary, boardText);
        humanTurn(boggle);
        computerTurn(boggle);
    }
}


