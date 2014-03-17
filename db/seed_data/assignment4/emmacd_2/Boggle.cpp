/*
 * CS106B Assignment 4: Boggle
 * Emma Daugherty
 * Section Leader: Jimmy Lee
 *
 * This is the Boggle class file. It implements all the methods of the Boggle class.
 */

#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "set.h"
#include "strlib.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

//constants
const int CUBES_SIZE = 16;
const int BOARD_SIZE = 4;
const int WORD_LENGTH = 4;

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    this -> dictionary = dictionary;
    this -> boardText = boardText;
    layout = Grid<char>(BOARD_SIZE, BOARD_SIZE);
    //random set-up
    if (this -> boardText == "") {
        Vector<string> pieces;
        for (int i = 0; i < CUBES_SIZE; i ++) {
            pieces.add(CUBES[i]);
        }
        shuffle(pieces);
        for (int k = 0; k < pieces.size(); k++) {
            int rand = randomInteger(0, pieces[k].length() - 1);
            boardText += pieces[k][rand];
        }
    }
    //reads board text into grid form
    int count = 0;
    for (int a = 0; a < layout.numRows(); a++) {
        for (int b = 0; b < layout.numCols(); b++) {
            layout[a][b] = boardText[count];
            count++;
        }
    }

    BoggleGUI::labelAllCubes(boardText);
}

char Boggle::getLetter(int row, int col) {
    char letter = layout.get(row, col);
    return letter;
}

bool Boggle::checkWord(string word) {
    if (word.length() < WORD_LENGTH) {
        return false;
    } else if (!dictionary.contains(word)) {
        return false;
    } else if (correctEntries.contains(word)) {
        return false;
    } else {
        correctEntries.add(word);
        return true;
    }
}

bool Boggle::humanWordSearch(string word) {
    if (!checkWord(word)) {
        return false;
    }
    else{
        for (int i = 0; i < layout.numRows(); i++) {
            for (int j = 0; j < layout.numCols(); j++) {
                //choose
                unhighlightBoard();
                BoggleGUI::labelCube(i, j, getLetter(i, j), true);
                if (getLetter(i, j) == word[0]) {
                    char temp = layout[i][j];
                    layout[i][j] = '#';
                    if (humanWordSearchHelper(i, j, word.substr(1))) { //explore
                        //unchoose
                        layout[i][j] = temp;
                        chosenWords.add(word);
                        recordWord(word, BoggleGUI::HUMAN);
                        return true;
                    }
                    else {
                        //unchoose if explore fails
                        layout[i][j] = temp;
                        BoggleGUI::labelCube(i, j, getLetter(i, j), false);
                    }
                }
            }
        }
    }
}

bool Boggle::humanWordSearchHelper(int row, int col, string remaining) {
    // if the last letter can be found, word is on the board
    if (remaining.length() == 1) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <=1; j++) {
                if (layout.inBounds(row + i, col + j) && getLetter(row + i, col + j) == remaining[0] &&
                        getLetter(row + i, col + j) != '#'){
                    BoggleGUI::labelCube(row + i, col + j, getLetter(row + i, col + j), true);
                    return true;
                }
            }
        }
        return false; //word not on board
    }
    else {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <=1; j++) {
                if (layout.inBounds(row + i, col + j) && getLetter(row + i, col + j) == remaining[0] &&
                        getLetter(row + i, col + j) != '#'){
                    //choose
                    char temp = layout[row + i][col + j];
                    BoggleGUI::labelCube(row + i, col + j, temp, true);
                    layout[row + i][col + j] = '#';
                    if (humanWordSearchHelper(row + i, col + j, remaining.substr(1))) { //explore
                        //unchoose
                        layout[row + i][col + j] = temp;
                        return true;
                    }
                    else {
                        //unchoose if explore fails
                        layout[row + i][col + j] = temp;
                        BoggleGUI::labelCube(row + i, col + j, temp, false);
                    }
                }
            }
        }
        return false;
    }
}

void Boggle::unhighlightBoard() {
    for (int i = 0; i < layout.numRows(); i++) {
        for (int j = 0; j < layout.numCols(); j++) {
            BoggleGUI::labelCube(i, j, layout[i][j], false);
        }
    }
}

int Boggle::humanScore() {
    int score = 0;
    for (string word : chosenWords) {
        score += word.length() - 3;
    }
    BoggleGUI::setScore(score, BoggleGUI::HUMAN);
    return score;
}

Set<string> Boggle::computerWordSearch() {

    for (int i = 0; i < layout.numRows(); i++) {
        for (int j = 0; j < layout.numCols(); j++) {
            char temp = layout[i][j];
            string word = charToString(temp);
            layout[i][j] = '#'; //choose
            computerWordSearchHelper(i, j, word); //explore
            layout[i][j] = temp; //unchoose

        }
    }

    return computerWords;
}

void Boggle::computerWordSearchHelper(int row, int col, string word) {
    if (dictionary.contains(word) && word.length() >= WORD_LENGTH && !chosenWords.contains(word) &&
            !computerWords.contains(word)) {
        computerWords.add(word);
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (layout.inBounds(row + i, col + j) && getLetter(row + i, col + j) != '#') {
                string newWord = word + getLetter(row + i, col + j);
                if (dictionary.containsPrefix(newWord)) {
                    char temp = layout[row + i][col + j];
                    layout[row + i][col + j] = '#'; //choose
                    computerWordSearchHelper(row + i, col + j, newWord); //explore
                    layout[row + i][col + j] = temp; //unchoose
                }
            }
        }
    }
}

int Boggle::getScoreComputer() {
    int score = 0;
    for (string word : computerWords) {
        score += word.length() - 3;
    }
    BoggleGUI::setScore(score, BoggleGUI::COMPUTER);
    return score;
}

Set<string> Boggle::getWords() {
    return chosenWords;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            out << boggle.getLetter(i, j);
        }
        out << endl;
    }
    return out;
}
