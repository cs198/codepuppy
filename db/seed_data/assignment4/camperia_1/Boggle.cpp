/*
 * Boggleplay program
 * there's one small bug with the gui: it always highlights one extra letter.
 * I tried messing with things here and there but can't seem to fix that
 * sorry...
 */


#include "Boggle.h"
#include "shuffle.h"
#include "bogglegui.h"

void createBoard(Grid<char>& , string);
bool humanWordSearchHelper(string , string , int , int , Grid<bool>& , Grid<char>&);
bool isValid(int , int , Grid<char>& , Grid<bool>&);
bool areInBounds(int , int , Grid<char>&);
void computerWordSearchHelper(string , Set<string>& , int , int , Grid<bool>& , Grid<char>& , Lexicon& , Set<string>&);


// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& dictionary) {
    //initializes the board and the gui
    Dictionary = dictionary;
    boggleBoard.resize(4 , 4); //resizes the board grid
    BoggleGUI::initialize(4 , 4); //initializes the gui
    checkedLetters.resize(4 , 4); //resizes the bool grid
}

void Boggle::createBoard(string boardText){
    //creates the board with a given string

    if(boardText == ""){
        //if the user wants a random board
        int counter = 0;
        for (int i = 0 ; i < boggleBoard.nRows ; i++){
            for (int j = 0 ; j < boggleBoard.nCols ; j++){
                int randomPos = randomInteger(0 , 5);
                char current = CUBES[counter][randomPos]; //gets a random letter form each "cube"
                boggleBoard.set(i , j , current); //sets it to the baord
                counter++;
            }
        }
        shuffle(boggleBoard); //shuffles the board

        //sets the letters to the gui
        for (int i = 0 ; i < boggleBoard.nRows ; i++){
            for (int j = 0 ; j < boggleBoard.nCols ; j++){
                BoggleGUI::labelCube(i, j, boggleBoard.get(i , j), false);
            }
        }
    } else {
        //if the user forced a text
        int counter = 0;
        for (int i = 0 ; i < boggleBoard.nRows ; i++){
            for (int j = 0 ; j < boggleBoard.nCols ; j++){
                char current = boardText[counter]; //gets the letter from the string
                boggleBoard.set(i , j , current); //sets the grid spot with the letter
                BoggleGUI::labelCube(i, j, current, false); //sets up the gui
                counter++;
            }
        }
    }
}

void Boggle::setCheckedLetters(){
    //sets all bools in the bool grid to false
    for (int i = 0 ; i < boggleBoard.nRows ; i++){
        for (int j = 0 ; j < boggleBoard.nCols ; j++){
            checkedLetters.set(i , j , false);
        }
    }
}

void Boggle::setHighlightedLetters(){
    //sets all the letters in the gui to un-highlighted
    for (int i = 0 ; i < 4 ; i++){
        for (int j = 0 ; j < 4 ; j++){
            BoggleGUI::labelCube(i , j , boggleBoard.get(i , j) , false);
        }
    }
}



char Boggle::getLetter(int row, int col) {
    //gets a specific letter from the board given a row and column
    return boggleBoard.get(row , col);
}

bool Boggle::checkWord(string word) {
    //checks if a given word is valid
    if (Dictionary.contains(word)){ //if it's in the dictionary
        if (word.length() >= 4){ //if it's more than 3 letters
            if (!humanWords.contains(toUpperCase(word))){ //if the user hasn't already inpputted it
                return true;
            }
        }
    }
    return false;
}

bool Boggle::humanWordSearch(string word) {
    //searches for a specific word on the board
    string newWord = toUpperCase(word); //sets it to uppercase
    string current = ""; //initializes a "current" string

    //iterates over each letter in the board
    for (int i = 0 ; i < boggleBoard.nRows ; i++){
        for (int j = 0 ; j < boggleBoard.nCols ; j++){
            BoggleGUI::labelCube(i, j, boggleBoard.get(i , j), true); //highlights the cube
            checkedLetters.set(i , j , true); //sets it to "used" in the bool grid
            if (humanWordSearchHelper(newWord , current , i , j , checkedLetters , boggleBoard)){
                //gets the helper word
                humanWords.add(toUpperCase(word)); //if it return true adds the word to the set
                return true;
            }
            setCheckedLetters(); //resets all letters to false
            setHighlightedLetters(); //unghighlights all letters
        }
    }
    return false;
}

bool humanWordSearchHelper(string word , string current , int row , int col , Grid<bool>& checkedLetters ,Grid<char>& boggleBoard){
    //helper function for the human word

    if (current == word) {
        //if we found the word
        return true;

    } else if (current == word.substr(0 , current.size()) && current.size() < word.size()){ //these statements prune the search tree
        //if the beggining woth the word we're forming matches the beginning of the word we're trying to form
        //and we haven't exceeded its size
        //(to prune the search tree)

        //iterates over all the neighbors of the letter
        for (int i = -1 ; i <= 1 ; i++){
            for (int j = -1 ; j <= 1 ; j++){
                if (isValid(row+i , col+j , boggleBoard , checkedLetters)){
                    //if the letter is in bounds and hasn't been checked yet
                    current += boggleBoard.get(row+i , col+j); //adds it to the word
                    checkedLetters.set(row+i , col+j , true); //sets it to checked (used)
                    BoggleGUI::labelCube(row+i, col+j, boggleBoard.get(row+i , col+j), true); //highlights it on the gui
                    if (humanWordSearchHelper(word , current , row+i , col+j , checkedLetters , boggleBoard)) return true; //explores its neighbors
                    BoggleGUI::labelCube(row+i, col+j, boggleBoard.get(row+i , col+j), false); //unhighlights it on the gui
                    current.erase(current.size()-1 , 1); //erases the letter from current
                    checkedLetters.set(row+i , col+j , false); //sets it to unused
                }
            }
        }
    }
    return false;
}

bool isValid(int row , int col , Grid<char>& boggleBoard , Grid<bool>& checkedLetters){
    //checks if the letter is valid (is in bounds and hasn't been used yet)
    if (areInBounds(row , col , boggleBoard) && checkedLetters.get(row , col) == false){
        return true;
    } else{
        return false;
    }
}

bool areInBounds(int row , int col , Grid<char>& boggleBoard){
    //checks if teh letter is in bounds
    if (row < boggleBoard.nRows && row >= 0 && col < boggleBoard.nCols && col >= 0){
        return true;
    } else{
        return false;
    }
}


int Boggle::humanScore() {
    //gets the human's score
    int score = 0;
    for (string word : humanWords){
        score += word.size()-3; //adds the length of each word -3 to the score
    }
    return score;
}

Set<string> Boggle::getHumanWords() {
    //returns the set of human words found
    return humanWords;
}

Set<string> Boggle::computerWordSearch() {
    //recursive algorithm for the computer words

    setCheckedLetters(); //sets all letters on the board to false

    //iterates over all the letters on the board
    for (int i = 0 ; i < boggleBoard.nRows ; i++){
        for (int j = 0 ; j < boggleBoard.nCols ; j++){
            string current; //for each initializes a string
            computerWordSearchHelper(current , result , i , j , checkedLetters , boggleBoard , Dictionary , humanWords); //recurses
        }
    }

    //returns the resulting set of strings
    return result;

}

void computerWordSearchHelper(string current , Set<string>& result , int row , int col , Grid<bool>& checkedLetters , Grid<char>& boggleBoard, Lexicon& dictionary , Set<string>& humanWords){
    //helper function for the computer's recursion

    if (dictionary.contains(current) && current.size() > 3 && !humanWords.contains(current)) {
        //if we found a dictionary word longer than three letters that the human hasn't found
        result.add(current); //add it to the string set result

    } else if (dictionary.containsPrefix(current)){ //using the "contains profix" to prunt the search tree

        //iterates over all the neighbors of the current letter
        for (int i = -1 ; i <= 1 ; i++){
            for (int j = -1 ; j <= 1 ; j++){
                if (isValid(row+i , col+j , boggleBoard , checkedLetters)){
                    //if the letter is valid
                    current += boggleBoard.get(row+i , col+j); //add it to the "current" string
                    checkedLetters.set(row+i , col+j , true); //set it to checked
                    computerWordSearchHelper(current , result , row+i , col+j , checkedLetters , boggleBoard , dictionary , humanWords); //recurse
                    current.erase(current.size()-1 , 1); //erase
                    checkedLetters.set(row+i , col+j , false); //uncheck
                }
            }
        }

    }
}


int Boggle::getScoreComputer() {
    //gets the computer's score
    int score = 0;
    for (string word : result){
        score += word.size()-3;
    }
    return score;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    //prints our the boggle object to the console
    for (int i = 0 ; i < 4 ; i++){
        for (int j = 0 ; j < 4 ; j++){
            out << boggle.getLetter(i , j);
        }
        out << endl;
    }
    return out;
}
