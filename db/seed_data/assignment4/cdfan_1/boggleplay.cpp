// This implements one complete game of Boggle, including a new board, human turns, computer turn, and game results.


#include "lexicon.h"
#include "Boggle.h"
#include "bogglegui.h"
#include "simpio.h"
#include "bogglegui.h"
#include "strlib.h"

//Prototypes
string generateBoardText();
bool isLetter (string text);
void initializeGraphics();
void humanTurn(Boggle & oneBoggle);
void checkHumanWordValid(Boggle & oneBoggle, string newWord);
void computerTurn (Boggle & oneBoggle);
void gameResult(Boggle & oneBoggle);

//constants
const int ROW = 4;
const int COL =4;
const int WORD_MIN_LENGTH = 4;

//Implements one complete game of Boggle, including generation board, human turns, computer turn and report game results.
void playOneGame(Lexicon& dictionary) {
    initializeGraphics();
    string boardText = generateBoardText();
    Boggle oneBoggle(dictionary,boardText);    
    BoggleGUI::labelAllCubes(oneBoggle.getBoardText());
    humanTurn(oneBoggle);
    computerTurn(oneBoggle);
    gameResult(oneBoggle);
}

//It gies the user the choice to enter the board letter configuration. Return the ligit text back.
string generateBoardText() {
    string text = "";
    if(!getYesOrNo("Do you want to generate a random board?")) {
        while(true) {
            text = getLine("Type the 16 letters to appear on the board:");
            if(text.length() == 16 && isLetter(text)) break;
            cout << "That is not a valid 16-letter board string. Try again." << endl;
        }
    }
    text = toUpperCase(text);
    return text;
}

//Checks a given string contains any non-letter words. If so, return false. Otherwise return true.
bool isLetter (string text) {
    for(int i=0; i<text.length(); i++) {
        if(!isalpha(text[i])) return false;
    }
    return true;
}

//IF the Graphics is not initialized, do that. If so, reset.
void initializeGraphics() {
    if(!BoggleGUI::isInitialized()) {
        BoggleGUI::initialize(ROW,COL);
    } else{
        BoggleGUI::reset();
    }
}

//Human's tern includes asking human for a word until the user enters. The word given by humans is validated in each while loop
void humanTurn(Boggle & oneBoggle) {
    clearConsole();
    cout << "It's your turn!" << endl;
    BoggleGUI::setStatusMessage("It's your turn!");
    while(true) {
        cout << oneBoggle<< endl;
        cout << "Your words (" << oneBoggle.getScoredWords().size() << "): " << oneBoggle.getScoredWords() << endl;
        cout << "Your score: " << oneBoggle.humanScore() << endl;
        string newWord = getLine("Type a word (or Enter to stop): ");
        if (newWord == "") {
            cout << "" << endl;
            break;
        }
        newWord = toUpperCase(newWord);
        checkHumanWordValid(oneBoggle, newWord);
    }
}

//It checks whether whether a given human word is valid based on the board. First it checks whether it meets 4-letter lenght, unused
//or a ligit English word in lexicon. if so, it continues to do recursive search on the board. If it's valid, it update human score
// and the word list found and used by human. if not, infomr the user, the word can't be found.
void checkHumanWordValid(Boggle & oneBoggle, string newWord){
    if(!oneBoggle.checkWord(newWord)) {
        clearConsole();
        cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
    } else if(!oneBoggle.humanWordSearch(newWord)) {
        clearConsole();
        cout << "That word can't be formed on this board." << endl;
        BoggleGUI::setStatusMessage("That word can't be formed on this board.");
    } else {
        clearConsole();
        oneBoggle.addScoredWord(newWord);
        oneBoggle.addHumanScore(newWord.length()-WORD_MIN_LENGTH+1);
        cout << "You found a new word! "<<"\"" << newWord <<"\"" << endl;
        BoggleGUI::setStatusMessage("You found a new word! \"" + newWord + "\"");
        BoggleGUI::recordWord(newWord,BoggleGUI::HUMAN);;
        BoggleGUI::setScore(oneBoggle.humanScore(),BoggleGUI::HUMAN);
    }
}

//Computer's turn of exhauted search result of every possibel words found on the board. The computer's result is the whole set
//minus the human result. update the computer score.
void computerTurn (Boggle & oneBoggle) {
    cout << "It's my turn!" << endl;
    BoggleGUI::setStatusMessage("It's my turn!");
    Set<string> computerResult = oneBoggle.computerWordSearch();
    int computerScore = oneBoggle.getScoreComputer();
    cout << "My words (" << computerResult.size() << "): " << computerResult << endl;
    cout << "My score: " << computerScore << endl;

    //update the BoggleGUI for computer results.Record each word in computer result Set onto UI and update computer score
    for(string word: computerResult) BoggleGUI::recordWord(word,BoggleGUI::COMPUTER);
    BoggleGUI::setScore(computerScore,BoggleGUI::COMPUTER);
}

//based on the human and computers scores, it reports the win/loose result into console and graphics UI.
void gameResult(Boggle & oneBoggle) {
    int humanScore = oneBoggle.humanScore();
    int computerScore = oneBoggle.getScoreComputer();
    if(humanScore < computerScore) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else {
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");

    }
}

