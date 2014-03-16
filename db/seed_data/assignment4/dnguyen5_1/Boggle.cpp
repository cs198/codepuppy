// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "Boggle.h"
#include "grid.h"
#include "shuffle.h"
#include "random.h"
#include "string.h"
#include "hashset.h"
#include "point.h"
#include <string>

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};


//initializes Boggle class using string boardText and lexicon dictionary. If boardText is an empty string, then dice are randomized.
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    playerScore = 0; //keeps track of player's score
    computerScore = 0; //keeps track of computer score
    boggleDictionary = dictionary;
    boggleGrid.resize(4, 4);
    //shuffles dice and initializes them
    if (boardText == "") {
        shuffle(CUBES, 16);
        int cubeCounter = 0;
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                string cubeString = CUBES[cubeCounter];
                boggleGrid[j][k] = cubeString[randomInteger(0, 5)];
                cubeCounter++;
                BoggleGUI::labelCube(j,k,boggleGrid[j][k], false);
            }
        }
    }
    //initializes dice to custom text
    if (boardText != "") {
        BoggleGUI::labelAllCubes(boardText);
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                boggleGrid[j][k] = boardText[0];
                boardText = boardText.substr(1);
            }
        }
        cout << boggleGrid << endl;
    }
}

//returns letter from boggle Grid at given row and column
char const Boggle::getLetter(int row, int col) {
    return boggleGrid[row][col];
}

//returns boggle grid
Grid<char> const Boggle::getGrid() {
    return boggleGrid;
}

//returns true if word (passed in as parameter) is 4+ letters, is in boggleDictionary, and the word hasn't been successfully entered by the user.
bool const Boggle::checkWord(string word) {
    if (word.length() >= 4 && boggleDictionary.contains(word) &&
            !wordsEntered.contains(word)) {
        return true;
    }
    else return false;
}

//returns computer's score
int const Boggle::getScoreComputer() {
    return computerScore;
}

//returns human player's score
int const Boggle::humanScore() {
    return playerScore;
}

//recursive human word search function. returns true if word can be found on the board.
bool Boggle::humanWordSearch(string word) {
    BoggleGUI::clearHighlighting();
    BoggleGUI::setAnimationDelay(100);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            BoggleGUI::setHighlighted(i, j, true);
            if (boggleGrid.get(i,j) == word[0]) {
                string currentWordFound;
                currentWordFound +=  boggleGrid.get(i,j);
                string wordCopy = word.substr(1);
                HashSet<Point> pointsChecked;
                pointsChecked.add(Point(i,j));
                bool wordFoundBool = humanWordSearchHelper(word, wordCopy, currentWordFound, i, j, pointsChecked);
                if (wordFoundBool == true) {
                    playerScore += word.length()-3;
                    wordsEntered += word;
                    BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                    return wordFoundBool;
                }
            }
            BoggleGUI::setHighlighted(i, j, false);
        }

    }
    return false;
}

//human word search helper. Takes in additional parameters: grid coordinates, copy of a word that is updated by one char at a time (ex: B -> BA -> BAL -> BALL), and a hashset of coordinates already checked
bool Boggle::humanWordSearchHelper(string word, string wordCopy, string currentWordFound, int gridX, int gridY, HashSet<Point> pointsChecked) {
    if (word == currentWordFound) {
        return true;
    }
    for (int i = gridX - 1; i < gridX + 2; i++) {
        for (int j = gridY - 1; j < gridY + 2; j++) {
            if (boggleGrid.inBounds(i, j) /*&& boggleGrid.get(i,j) == wordCopy[0] */&& (i != gridX || j != gridY) && !pointsChecked.contains(Point(i,j))) {
                BoggleGUI::setHighlighted(i, j, true);
                if (boggleGrid.get(i,j) == wordCopy[0]) {
                    string tempWordFound = currentWordFound;
                    tempWordFound += boggleGrid.get(i,j);//boggleGrid.get(i,j);
                    string tempWord = wordCopy.substr(1);
                    HashSet<Point> temp = pointsChecked;
                    temp += Point(i, j);
                    bool wordFoundBool = humanWordSearchHelper(word, tempWord, tempWordFound, i, j, temp);
                    if (wordFoundBool == true) return wordFoundBool;
                }
                BoggleGUI::setHighlighted(i, j, false);
            }
        }
    }
    return false;
}

//recursive computer word search
Set<string> Boggle::computerWordSearch() {
    Set<string> wordsFound;
    string currentWordFound = "";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            HashSet<Point> pointsChecked;
            currentWordFound = boggleGrid.get(i,j);
            pointsChecked.add(Point(i,j));
            computerWordSearchHelper(currentWordFound, wordsFound, i, j, pointsChecked);
        }
    }
    return wordsFound;
}

//Computer word search helper. Takes in additional parameters: grid coordinates, copy of a word that is updated by one char at a time (ex: B -> BA -> BAL -> BALL), and a hashset of coordinates already checked
void Boggle::computerWordSearchHelper(string currentWordFound, Set<string>& wordsFound, int gridX, int gridY, HashSet<Point> pointsChecked) {
    if (boggleDictionary.contains(currentWordFound) && currentWordFound.length() > 3 && !wordsFound.contains(currentWordFound) && !wordsEntered.contains(currentWordFound)) {
        wordsFound.add(currentWordFound);
        computerScore += currentWordFound.size()-3;
        BoggleGUI::recordWord(currentWordFound, BoggleGUI::COMPUTER);

    }
    if (boggleDictionary.containsPrefix(currentWordFound)) {
        for (int i = gridX - 1; i < gridX + 2; i++) {
            for (int j = gridY - 1; j < gridY + 2; j++) {
                if (boggleGrid.inBounds(i, j) && (i != gridX || j != gridY) && !pointsChecked.contains(Point(i,j))) {
                    string temp = currentWordFound + boggleGrid.get(i,j);
                    HashSet<Point> tempPointsChecked = pointsChecked;
                    tempPointsChecked.add(Point(i,j));
                    computerWordSearchHelper(temp, wordsFound, i, j, tempPointsChecked);
                }
            }
        }
    }
}


//overloads operator to print grid
ostream& operator<<(ostream& out, Boggle& boggle) {
    Grid <char> temp = boggle.getGrid();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out << temp[i][j];
        }
        out << endl;
    }
    return out;
}
