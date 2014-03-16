//Name: Benzheng Zhu
//CS 106B Assignment 4
//Citation: lecture slides
//This is the Boggle class .cpp file, where the member variables and memeber functions are defined

#include <string>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "set.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    if (boardText == "") {
        //for each cube, choose a side
        for (int i=0; i<16;i++) {
            int n = randomInteger(0,5);
            boardText += CUBES[i][n];
        }
        //shuffle cubes
        boardText=shuffle(boardText);
    }

    BoggleGUI::labelAllCubes(boardText);
    BoggleGUI::setAnimationDelay(20);
    board.resize(4,4);
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            board[i][j] = boardText[i*4+j];
        }
    }
    D = dictionary;//It takes time to copy the dictionary.
    yourList.clear();
    myList.clear();
    yourScore = 0;
    myScore = 0;
}

void Boggle::printBoard() const {
    for (int i=0; i<board.numRows(); i++) {
        for (int j=0; j<board.numCols(); j++) {
            cout << board[i][j];
        }
        cout<<endl;
    }
}

char Boggle::getLetter(int row, int col) const {
    return board[row][col];
}

bool Boggle::checkWord(string& word) const{
    if ((!D.contains(word)) || (word.size()<4) || (yourList.contains(word))) {
        return false;
    } else {
        return true;
    }
}

bool Boggle::humanHelper(Grid<char>& temp, string word, int row, int col) {
    if (word.size()<=0) {
        return true;
    } else {
        BoggleGUI::labelCube(row, col, ' ' ,true);
        if (word[0] == temp[row][col]) {
            temp[row][col]='-'; // choose
            //                        BoggleGUI::labelCube(row, col, ' ' ,true);
            //explore the cell itself and its neighbors
            //since the cell has been marked, it will not be selected again
            for (int i=max(0,row-1); i <=min(row+1,board.numRows()-1);i++) {
                for (int j=max(0, col-1); j<=min(col+1, board.numCols()-1);j++ ) {
                    bool found = humanHelper(temp,word.substr(1),i,j);
                    if (found) {
                        return true;
                    }
                }
            }
            temp[row][col]=board[row][col];   // un-choose
        }
        if (temp[row][col]!= '-') {
            BoggleGUI::labelCube(row,col,' ',false);
        }
        return false;
    }
}

bool Boggle::humanWordSearch(string word) {
    Grid<char> temp = board;
    for (int i=0; i<board.numRows();i++) {
        for (int j=0; j<board.numCols();j++) {
            bool found = humanHelper(temp,word, i, j);
            if (found) {
                yourList.add(word);
                yourScore += word.size()-3;
                BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                BoggleGUI::setScore(yourScore, BoggleGUI::HUMAN);
                return true;
            }
        }
    }
    return false;
}

void Boggle::computerHelper(Grid<char>& temp, string s, int row, int col) {
    if (checkWord(s)&& !myList.contains(s)) {
        myList.add(s);
        myScore += s.size()-3;
        BoggleGUI::recordWord(s, BoggleGUI::COMPUTER);
        BoggleGUI::setScore(myScore, BoggleGUI::COMPUTER);
    }
    if (!D.containsPrefix(s)) {
        return;
    } else {
        //    mark cube c as visited. // choose
        temp[row][col]='-';
        //    for each neighboring cube next to c:
        for (int i=max(0,row-1); i <=min(row+1,board.numRows()-1);i++) {
            for (int j=max(0, col-1); j<=min(col+1, board.numCols()-1);j++ ) {
                //    explore all words that could start with c's letter. // explore
                if (temp[i][j]!='-'){
                    s+=temp[i][j]; //choose
                    computerHelper(temp,s,i,j);//explore
                    s.erase(s.size()-1,1); //un-choose
                }
            }
        }
        //    un-mark cube c as visited. // un-choose
        temp[row][col]=board[row][col];
    }
    return;
}

void Boggle::computerWordSearch() {
    //    for each letter cube c:
    for (int row=0; row<board.numRows();row++){
        for (int col=0; col< board.numCols();col++) {
            Grid<char> temp = board;
            string s = "";
            s+=board[row][col];
            computerHelper(temp,s,row, col);
        }
    }
}

int Boggle::humanScore() const {
    return yourScore;
}

int Boggle::computerScore() const {
    return myScore;
}

Set<string> Boggle::humanList() const {
    return yourList;
}

Set<string> Boggle::computerList() const {
    return myList;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    // TODO: implement
    return out;
}

string Boggle::getText() const {
    string str = "";
    for (int i=0; i<board.numRows();i++) {
        for (int j=0; j<board.numCols();j++) {
            str+=board[i][j];
        }
    }
    return str;
}
