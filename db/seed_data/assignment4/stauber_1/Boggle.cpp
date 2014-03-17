//Filename: Boggle.cpp
//Author: Mark Stauber
//Date: 2/13/14
//Description: Contains the function definitions for
//the Boggle class that represents a single game of boggle.
#include "strlib.h"
#include "shuffle.h"
#include "lexicon.h"

#include "Boggle.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    //If boardText is empty then create random board.
    if(boardText.length() == 0){
        shuffle(CUBES, 16);
        for(int i = 0; i < 16; i++){
            string shuffled = shuffle(CUBES[i]);
            board += shuffled[0];
        }
    }else{
        //Fill out board with user's boardText
        for(int i = 0; i < 16; i++){
            board += toupper(boardText[i]);
        }
    }

    //Set up Boggle data fields and GUI.
    gameState = true;
    computerPoints = 0;
    humanPoints = 0;
    this->dictionary = dictionary;
    BoggleGUI::labelAllCubes(board);
}

char Boggle::getLetter(int row, int col) {
    if(row < 0 || row > 3 || col < 0 || col > 3){
        cout<<"Illegal "<<row<<" "<<col<<endl;
        throw(1);
    }
    return board[row * 4 + col];
}

bool Boggle::checkWord(string word) {
    if(word.length() < 4) return false;
    if(humanWords.contains(toUpperCase(word))) return false;
    if(!dictionary.contains(word)) return false;
    return true;
}


bool Boggle::humanWordSearch(string word) {
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            Set<int> markedSpots;
            //If word is found on board.
            if(humanSearchHelper(toUpperCase(word), i, j, markedSpots)){
                humanPoints += word.length() - 3;
                humanWords.add(toUpperCase(word));
                BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                BoggleGUI::setScore(humanPoints, BoggleGUI::HUMAN);
                return true;
            }
        }
    }
    return false;
}

int Boggle::humanScore() {
    return humanPoints;
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            string word = charToString(getLetter(i, j));
            Set<int> markedSpots;
            computerSearchHelper(word, i, j, markedSpots, result);
        }
    }
    return result;
}

int Boggle::getScoreComputer() {
    return computerPoints;
}

bool Boggle::gameOn(){
    return gameState;
}

void Boggle::endGame(){
    gameState = false;
}

void Boggle::printBoard(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout<<getLetter(i, j);
        }
        cout<<endl;
    }
    cout<<endl;
}

Set<string> Boggle::getHumanWords(){
    return humanWords;
}

string Boggle::gameResult(){
    if(computerPoints > humanPoints)
        return "Ha ha ha, I destroyed you. Better luck next time, puny human!";
    else if(computerPoints < humanPoints)
        return "WOW, you defeated me! Congratulations!";
    return "You tied!? Are you my motherboard?";
}

bool Boggle::humanSearchHelper(string word, int row, int col, Set<int>& markedSpots){
    //Add row, col to marked spots and set highlight
    markedSpots.add(row * 4 + col);
    BoggleGUI::labelCube(row, col, getLetter(row, col), true);

    //Check if row, col is next letter in word
    string thisLetter = charToString(getLetter(row, col));
    if(equalsIgnoreCase(thisLetter, charToString(word[0]))){

        //If word.length is 1 then we found the word on board
        if(word.length() == 1){
            BoggleGUI::labelCube(row, col, getLetter(row, col), false); //Reset highlight
            return true;
        }

        //For each of row, col neighbors
        for(int i = (row - 1); i < (row + 2); i++)
            for(int j = (col - 1); j < (col + 2); j++)
                //If i,j is on board call humanSearchHelper on it using the rest of word
                if(!markedSpots.contains(i * 4 + j) &&
                        !(i < 0 || i > 3 || j < 0 || j > 3) &&
                        humanSearchHelper(word.substr(1), i, j, markedSpots)){
                            //If word was found, unwind recursion
                            BoggleGUI::labelCube(row, col, getLetter(row, col), false);
                            return true;
                       }
    }
    //If word was not found on this path, unwind recursion
    markedSpots.remove(row * 4 + col);
    BoggleGUI::labelCube(row, col, getLetter(row, col), false);
    return false;
}

void Boggle::computerSearchHelper(string word, int row, int col,
                                  Set<int>& markedSpots, Set<string>& result){
    //If dictionary doesnt contain anything that starts with word then bad path.
    if(!dictionary.containsPrefix(word)) return;
    //Else check if word is a valid, unaccounted for word.
    if(word.length() > 3 && !humanWords.contains(word) && !result.contains(word)
            && dictionary.contains(word)){
        computerPoints += word.length() - 3;
        result.add(word);
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
        BoggleGUI::setScore(computerPoints, BoggleGUI::COMPUTER);
    }

    //Add row, col to this path
    markedSpots.add(row * 4 + col);
    //For each of row, col neighbors
    for(int i = (row - 1); i < (row + 2); i++)
        for(int j = (col - 1); j < (col + 2); j++)
            //If i, j is on board and unchecked call computerSearchHelper on it.
            if(!markedSpots.contains(i * 4 + j) &&
                    !(i < 0 || i > 3 || j < 0 || j > 3)){
                    computerSearchHelper(word + getLetter(i, j), i, j, markedSpots, result);
            }
    //Unwind path.
    markedSpots.remove(row * 4 + col);
    return;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    //If gameOn then it is user's turn
    if(boggle.gameOn()){
        //Print boggle board.
        boggle.printBoard();

        //print out human words.
        Set<string> words = boggle.getHumanWords();
        out<<"Your words ("<<words.size()<<"): {";
        bool first = true; //tracks special case of first word.
        for(auto i : words){
            //If it is the first word don't print with , beforehand.
            if(first){
                out<<"\""<<i<<"\"";
                first = false;
            }else cout<<", \""<<i<<"\"";
        }
        out<<"} "<<endl;

        //Print out human score
        out<<"Your score: "<<boggle.humanScore()<<endl;
    }else{
        cout<<endl<<endl<<"It's my turn!"<<endl;

        //Run a computerWordSearch, and print out words.
        Set<string> words = boggle.computerWordSearch();
        out<<"My words ("<<words.size()<<"): {";
        bool first = true; //tracks special case of first word.
        for(auto i : words){
            //If it is the first word don't print with , beforehand.
            if(first){
                out<<"\""<<i<<"\"";
                first = false;
            }else cout<<", \""<<i<<"\"";
        }
        out<<"} "<<endl;

        //Print out computer score.
        out<<"My score: "<<boggle.getScoreComputer()<<endl;
        out<<boggle.gameResult()<<endl;
    }
    return out;
}

