// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
#include "Boggle.h"
#include "strlib.h"
#include "grid.h"
#include "random.h"
#include "shuffle.h"
#include "vector.h"
#include "hashset.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    // TODO: implement
        dic = dictionary; // construct dic
        boardString = boardText;
        totalHumanWords = 0; // set an initial value to it
        humanWordScore = 0;
        computerWordScore = 0;
        totalComputerWords = 0;
        // build board grid
        boardGrid.resize(4,4);
        if(boardText == ""){
         shuffle(CUBES, 16); // rearrange cubes on the board
         int m = 0; // CUBE Num.
         for(int i = 0; i < 4; i++){ // boardGrid row Num.
           for(int j = 0; j < 4; j++){// boardGrid col Num
              int rdm = randomInteger(0, 5); // get random side for each cube
              string cubeText = CUBES[m];
              char c = cubeText[rdm];
              boardGrid[i][j] = c;
              boardString += charToString(c);
              m++;
           }
         }
        }else{ // put the string elements in the grid
            int n = 0;
            for(int i = 0;i < 4; i++){
                for (int j = 0; j < 4; j++){
                 boardGrid[i][j] = boardText[n];
                 n++;
                }
            }
        }
}

string Boggle::getString(){
   return boardString;
}

Grid<char> Boggle::getGrid(){
    return boardGrid;
}

char Boggle::getLetter(int row, int col) {
    // TODO: implement
    return boardGrid[row][col];
}

bool Boggle::checkWord(string word) {
 if(word.size() >=4 && dic.contains(word) && !humanWords.contains(word) ){
        return true;
    }else{
    return false;
    }
}

bool Boggle::humanWordSearch(string word) {
    // TODO: implement
        for(int m = 0; m < 4; m++){ // loop 4 rows and 4 cols
            for(int n = 0; n < 4; n++){
                BoggleGUI::labelCube(m,n,boardGrid[m][n], true);
                BoggleGUI::setAnimationDelay(100);
                if (boardGrid[m][n] == word[0]){ // find the first letter
                    // do the recursion
                    if(humanWordSearchHelper(word, m, n, 1)){
                        humanWordScore = humanWordScore + word.size() - 3;
                        totalHumanWords = totalHumanWords + 1; // num of human words
                        BoggleGUI::setScore(totalHumanWords, BoggleGUI::HUMAN);
                        humanWords.add(word);
                        return true;
                    }
                }
                BoggleGUI::labelCube(m,n,boardGrid[m][n], false);
            }
        }
        return false;
 }

bool Boggle::humanWordSearchHelper(string word, int row, int col, int num){ // num begins with 1
        // base case
    BoggleGUI::labelCube(row,col,boardGrid[row][col], true);
    BoggleGUI::setAnimationDelay(100);
    if(num == word.size()){
        BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
        return true;
    }else{
        char temp = boardGrid[row][col];
        boardGrid[row][col] = '?'; // mark it as used-- chose
        for(int i = row-1; i<=row+1; i++){ // check all the neighbours
            for(int j = col-1; j<=col+1; j++){
                if(boardGrid.inBounds(i,j) && boardGrid[i][j] != '?'){
                    BoggleGUI::labelCube(i,j,boardGrid[i][j], true); //highlight the neighbour
                    BoggleGUI::setAnimationDelay(100);
                if(boardGrid[i][j]==word[num]){ // if get the next letter
                    if(humanWordSearchHelper(word,i,j, num+1)){ // recursion
                        boardGrid[row][col] = temp; // unchose
                        return true;
                    }
                }
                BoggleGUI::labelCube(i,j,boardGrid[i][j], false); // unhighlight the neighbour
            }
          }
        }
        boardGrid[row][col] = temp; //unchose
        return false;
    }
}

int Boggle::getHumanWordsNum(){
    return totalHumanWords;
}

HashSet<string> Boggle::getHumanWords(){
    return humanWords;
}

int Boggle::humanScore() {
    // TODO: implement
    return humanWordScore;   // remove this
}

Set<string> Boggle::computerWordSearch() {
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            string computerWord = charToString(boardGrid[i][j]);
            char temp = boardGrid[i][j]; //mark the first letter
            boardGrid[i][j] = '?';
            // do recursion
            computerWordSearchHelper(computerWord,i,j);
            boardGrid[i][j] = temp; //unmark the first letter
        }
    }
    return computerWords;
}

void Boggle::computerWordSearchHelper(string cword, int row, int col){
    for(int i = row-1; i <= row+1; i++){
        for(int j = col-1; j <= col+1; j++){
            if(boardGrid.inBounds(i,j) && boardGrid[i][j]!= '?'){
                char temp = boardGrid[i][j]; // choose
                boardGrid[i][j] = '?'; // choose
                cword = cword + charToString(temp); // update the word
               if(dic.containsPrefix(cword)){ // if dic has such prefix
                computerWordSearchHelper(cword,i,j); // recursion
                if(checkWord(cword) && !computerWords.contains(cword)){ // if it's a word in dic
                  computerWords += cword;
                  BoggleGUI::recordWord(cword, BoggleGUI::COMPUTER);
                  computerWordScore = computerWordScore + cword.size() -3;
                  totalComputerWords = totalComputerWords + 1;
                  BoggleGUI::setScore(computerWordScore, BoggleGUI::COMPUTER);
               }
            }
               cword = cword.substr(0, cword.size()-1);
                boardGrid[i][j] = temp; //unchose
            }
        }
    }
}


int Boggle::getComputerWordsNum(){
    //cout << totalComputerWords << endl;
    return totalComputerWords;
}

int Boggle::computerScore() {
    // TODO: implement
    return computerWordScore;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    //print the grid;
      for (int r = 0; r < 4; r++){
          string s1 = "";
          for (int c = 0; c < 4; c++){
              s1 += boggle.getLetter(r,c);
          }
          out << s1 << endl;
      }
   return out;
}
