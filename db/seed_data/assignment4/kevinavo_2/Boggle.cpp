/* Name: Kevin Vo
 * TA: Jimmy Lee
 * This the cpp file that handles the bulk of the program. It implements
 * the functions that will be used in the boggleplay file. For further description
 * read the comments on top of each function.
*/

#include "Boggle.h"
#include "shuffle.h"
#include "string.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "bogglegui.h"

static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

/* This is the constructer of a Boggle object. It constructs a board
 * for both the random case and user choice case.
*/
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    Grid <char> b;
    b.resize(4,4);
    boardText = toUpperCase(boardText);
    BoggleGUI::initialize(4,4);
    this->dictionary = dictionary;
    this->boardText = boardText;

    //randomcase
    if (boardText == ""){
        int row = 0;
        for (int i = 0; i < 16; i++){
            string die = CUBES[i];
            int rand = randomInteger(0,5);
            if (i%4 == 0 && i != 0) row++;
            b.set(row,i%4,die[rand]);
            BoggleGUI::labelCube(row,i%4,die[rand],false);
        }
        board = b;

    //nonrandom case
    }else{
        int row = 0;
        for (int i = 0; i < 16; i++){
            if (i%4 == 0 && i != 0) row++;
            b.set(row,i%4,boardText[i]);
        }
        board = b;
        BoggleGUI::labelAllCubes(boardText);
    }
}

/*This getter function returns the board*/
Grid <char> Boggle::getBoard(){
    return board;
}

/*Gets the letter at the specified point on the board*/
char Boggle::getLetter(int row, int col) {
    return board[row][col];
}

/* This is boolean helper function that verifies if the word entered by the
 * user is actually a valid word. It must satisfy the three conditions: be a word
 * have at least four letters and it a new word.
*/
bool Boggle::checkWord(string word) {
    if (!dictionary.contains(word) || word.length() <= 3  || usedWords.contains(word)){
        return false;
    }else{
        return true;
    }
}
/* This is the function that implements a recursive function. It also initializes
 * a grid of booleans which will act as template to mark cubes that have been selected.
 * It also clears the GUI if there are highlighted cubes.
*/
bool Boggle::humanWordSearch(string word) {
    word = toUpperCase(word);
    Grid <bool> chosen;
    chosen.resize(4,4);
    for (int r = 0; r < board.numRows(); r++){
           for (int c = 0; c < board.numCols(); c++){
                  chosen[r][c] = true;
           }
    }
    //this clears the highlighted cubes from the previous turn
    for (int r = 0; r < board.numRows(); r++){
           for (int c = 0; c < board.numCols(); c++){
                  BoggleGUI::labelCube(r,c,board[r][c]);
           }
    }
    //iterates through the grid and recursively finds the word
    if (!dictionary.contains(word)){
        return false;
    }else{
        for (int r = 0; r < board.numRows(); r++){
            for (int c = 0; c < board.numCols(); c++){
                if (board[r][c] == word[0]){
                    bool found =  humanWordSearchHelper(word, chosen, r, c);
                    if (found){
                        return true;
                    }else{
                        chosen[r][c] = true;
                    }
                }
            }
        }
    }
}
/* This is the recursive helper function. It looks at the letters around a selected
 * cube and sees if any of the letters match the next letter in the word. If a letter
 * matches that it recurses again to see if the next letter in the word matches. It keeps
 * recursing until the last letter is found. Progression: start with word "here". Loops across
 * grid to find "h" now the word you are looking for is "ere" then "re" then "e".
*/
bool Boggle::humanWordSearchHelper(string word, Grid<bool>& chosen, int row, int col){
    chosen[row][col] = false;
    BoggleGUI::labelCube(row,col,board[row][col],true);
    BoggleGUI::setAnimationDelay(40);
    word = word.substr(1);
    for (int r = row - 1; r <= row + 1; r++){
        for (int c = col - 1; c <= col + 1; c++){
            if (board.inBounds(r,c) && chosen[r][c] && board[r][c] == word[0]){
                if (word.length() != 1){
                    BoggleGUI::labelCube(r,c,board[r][c],true);
                    return humanWordSearchHelper(word, chosen, r, c);
                }
                else {
                    BoggleGUI::labelCube(r,c,board[r][c],true);
                    BoggleGUI::setStatusMessage("You have found a word");
                    return true;
                }

            }
        }
    }
    return false;
}
/*A getter function that returns the human score*/
int Boggle::getHumanScore() {
    return score;
}
/*A setter method to set the member variable that represents human score to the desired value.*/
void Boggle::setHumanScore(int score){
    this->score = score;
}
/*This is the recursive function that searches the entire board for all possible words that exist
It utilizes a grid of booleans to mark and unmark steps during the recursive process.*/
Set<string> Boggle::computerWordSearch() {
      Grid <bool> chosen;
      chosen.resize(4,4);
      for (int r = 0; r < board.numRows(); r++){
             for (int c = 0; c < board.numCols(); c++){
                    chosen[r][c] = true;
             }
      }
      Set <string> allWords;
      for (int row = 0; row < board.numRows(); row++){
          for (int col = 0; col < board.numCols(); col++){
                computerWordSearchHelper(chosen, row, col, "", allWords);
          }
      }
      return allWords;
}
/* This is recursive helper function. Like the humanword search function, it looks at the adjacent letters and
 * see if it forms a prefix that is found in the dictionary. If it is then it keeps recursin. If the prefix is
 * not found it returns back to the previous step. This is search that utilizes a pruning approach.
*/
void Boggle::computerWordSearchHelper(Grid <bool>& chosen, int row, int col, string word, Set<string>& allWords){
    for (int r = row - 1; r <= row + 1; r++){
       for (int c = col - 1; c <= col + 1; c++){
            if (!dictionary.containsPrefix(word)){
                return;
            } else{
               if (board.inBounds(r,c) && chosen[r][c]){
                   char chosenLetter = board[r][c];
                   chosen.set(r,c,false);
                   std:string str (1,chosenLetter);
                   word += str;
                   if (dictionary.contains(word) && word.length() > 3 && !usedWords.contains(word)){
                       allWords.add(word);
                       usedWords.add(word);
                       computerScore += word.length()-3;
                       BoggleGUI::recordWord(word,BoggleGUI::COMPUTER);
                       BoggleGUI::setScore(computerScore,BoggleGUI::COMPUTER);
                   }
                   computerWordSearchHelper(chosen,r,c,word,allWords);
                   chosen.set(r,c,true);
                   word = word.substr(0,word.size()-1);
               }
           }
        }
    }
}
/*The following two functions are getter and setter functions for
the ocmputer score.*/
int Boggle::getScoreComputer() {
    return computerScore;
}

void Boggle::setScoreComputer(int computerScore){
    this->computerScore = computerScore;
}

/*Adds all the used words to a lexicon to make sure that words are not repeated*/
void Boggle::setUsedWords(string word){
    usedWords.add(word);
}

/*This displays the boggle board on the console window.*/
string Boggle::displayBoard(Grid<char>& grid, int index){
    string result;
    for (int r = index; r < index + 1; r++){
        for (int c = 0; c < grid.numCols(); c++){
            result += grid[r][c];
        }
        return result;
    }
}
/*The operator was overloaded to print out a boggle board when it
sees a boggle board as an object*/
ostream& operator<<(ostream& out, Boggle& boggle) {
    Grid<char> b = boggle.getBoard();
    out << boggle.displayBoard(b,0) << endl << boggle.displayBoard(b,1) << endl
           << boggle.displayBoard(b,2) << endl << boggle.displayBoard(b,3);
    return out;
}
