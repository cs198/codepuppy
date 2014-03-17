// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "Boggle.h"
#include "shuffle.h"
#include "random.h"
#include "bogglegui.h"

//constants:
const int ROW = 4;
const int COL =4;
const int WORD_MIN_LENGTH = 4;
const int DELAY = 150;

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    lexicon = dictionary;
    if(boardText=="") boardText = randomBoard();
    board.resize(ROW,COL);
    for(int i=0; i<ROW;i++) {
        for(int j=0; j<COL;j++){
            board[i][j] = boardText.at(i*ROW+j);
        }
    }
    used =  {};
    score = 0;
    computerResult = {};
}

string Boggle::randomBoard() {
    string text = "";
    shuffle(CUBES,16);
    for(int i=0; i<16; i++) {
        text += CUBES[i].at(randomInteger(0,CUBES[i].length()-1));
    }
    return text;
}

char Boggle::getLetter(int row, int col) const {
    return board[row][col];
}

string Boggle::getBoardText() const {
    string boardText="";
    for(int i=0; i<board.numRows();i++) {
        for(int j=0; j<board.numCols();j++){
            boardText += board[i][j];
        }
    }
    return boardText;
}

Grid<char> Boggle:: getBoard() const {
    return board;
}

bool Boggle::checkWord(string word) {
    return word.length()>= WORD_MIN_LENGTH && lexicon.contains(word) && !used.contains(word);
}

bool Boggle::humanWordSearch(string word) {
    Grid<char> copyBoard = board;
    BoggleGUI::labelAllCubes(this ->getBoardText());
    //Search through the board to find whether there is the first letter of the human word
    for(int i=0; i<copyBoard.numRows();i++) {
        for(int j=0; j<copyBoard.numCols();j++){
            BoggleGUI::labelCube(i,j,copyBoard[i][j],true);
            BoggleGUI::setAnimationDelay(DELAY);
            if(copyBoard[i][j] == word[0]) {
                BoggleGUI::labelCube(i,j,copyBoard[i][j],true);
                copyBoard[i][j] = '?'; //choose
                if(humanWordSearchHelper(copyBoard, word.substr(1), i,j)) return true; //explore
                copyBoard[i][j] = word[0]; // unchoose
                BoggleGUI::labelCube(i,j,copyBoard[i][j],false);
            }
            BoggleGUI::labelCube(i,j,copyBoard[i][j],false);
        }
    }
    return false;
}

bool Boggle::humanWordSearchHelper(Grid<char>& copyBoard, string word, int x, int y) {
    if(word == "" ) {
        return true;
    } else {
        //explore the neighbors of given i and j, to find whether any neighbor matches a the first letter of substring of the word
        for (int i= x-1; i <= x+1; i++) {
            for(int j = y-1; j <= y+1; j++) {
                if(copyBoard.inBounds(i,j) && copyBoard[i][j] == word[0]) {
                   BoggleGUI::labelCube(i,j,copyBoard[i][j],true);
                   BoggleGUI::setAnimationDelay(DELAY);
                   copyBoard[i][j] = '?';
                   if (humanWordSearchHelper(copyBoard, word.substr(1), i, j)) return true;
                   copyBoard[i][j] = word[0];
                   BoggleGUI::labelCube(i,j,copyBoard[i][j],false);
                }
            }
        }
    }
    return false;

}

int Boggle::humanScore() const {
    return score;
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    Grid<char> copyBoard = this->getBoard();
    //explore every letter on the board as the first letter of a word
    for(int i=0; i<copyBoard.numRows();i++) {
        for(int j=0; j<copyBoard.numCols();j++){
            string word = "";
            word += copyBoard[i][j];
            copyBoard[i][j] = '?';
            computerWordSearchHelper(copyBoard,i, j,word,result);
            copyBoard[i][j] = word[0];
         }
    }
    result -= used; // The exhaused search result excludes the human found words. The remainder is the computer result.
    computerResult = result;
    return result;
}

void Boggle::computerWordSearchHelper(Grid<char> & copyBoard, int row, int col, string word, Set<string> & result) {
    if(checkWord(word) && lexicon.contains(word)) result.add(word);
    //Explore a given letter's neighbors. If the chosen letters so far don't form a ligit prefix, this exploration path ends.
    for (int i= row-1; i <= row+1; i++) {
        for(int j = col-1; j <= col+1; j++) {
            if(copyBoard.inBounds(i,j) && isalpha(copyBoard[i][j]) && lexicon.containsPrefix(word)) {
                word += copyBoard[i][j];//choose
                copyBoard[i][j] = '?'; //choose
                computerWordSearchHelper(copyBoard, i, j, word, result); //explore
                copyBoard[i][j] = word[word.length()-1]; //unchoose
                word = word.substr(0,word.length()-1); //unchoose
            }
        }
    }

}

int Boggle::getScoreComputer() const {
    int computerScore = 0;
    for (string word: computerResult){
        computerScore += (word.length()-WORD_MIN_LENGTH+1);
    }
    return computerScore;
}

Set<string> Boggle::getScoredWords() const {
    return used;
}

void Boggle::addScoredWord(string word) {
    used.add(word);
}

void Boggle:: addHumanScore(int gain) {
    score += gain;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int i=0; i<boggle.getBoard().numRows();i++) {
        string text = "";
        for(int j=0; j<boggle.getBoard().numCols();j++){
            text += boggle.getBoard()[i][j];
        }
        out << text << endl;
    }
    return out;
}


