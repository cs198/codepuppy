// Min Cheol Kim
// Section Leader Jimmy Lee
// Section Wed 5:15
/* This assignment simulated the word game Boggle, in which players try to form words on a 4x4 grid
 * using letters that are immediately adjacent to each other (diagonally, horizontally, and vertically).
 * This assignment involved a computer player who can find all the words recursively, and essentially wins
 * every single time.
 *
 * This specfic file is the Boggle class file. */

#include "point.h"
#include "Boggle.h"
#include "vector.h"
#include "random.h"
#include "shuffle.h"
#include "bogglegui.h"
// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
// Constant declared for the size of the board.
const int BOARD_SIZE = 4;

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    initialSetup(dictionary);
    gameBoard.resize(BOARD_SIZE,BOARD_SIZE);
    if (boardText.length() != 0)
        manualBoardSetup(boardText);
    else
        randomBoardSetup();
}

char Boggle::getLetter(int row, int col) const {
    if (row >= 0 && col >= 0)
        return gameBoard[row][col];
    throw "Not a valid coordinate";
}

bool Boggle::checkWord(string word) const {
    if ( (word.length() >= 4) && (dict.contains(word)) && !(alreadyUsed.contains(toUpperCase(word))) )
        return true;
    else
        return false;
}

bool Boggle::humanWordSearch(string word) {
    for (int n = 0; n < BOARD_SIZE; n++)
        for (int m = 0; m < BOARD_SIZE; m++) {
            resetGameBoard();  
            string current = "";
            Point pos(n,m);
            if (humanWordSearchHelper(current, word, pos))
                return true;
        }
    return false;
}

bool Boggle::humanWordSearchHelper(string current, string word, Point pos) {
    if(isOutside(pos)) return false;
    if(isMarked(pos)) return false;
    current = current + gameBoard[pos.getX()][pos.getY()];
    BoggleGUI::labelCube(pos.getX(), pos.getY(), gameBoard[pos.getX()][pos.getY()], true);
    if(current == word) return true;
    if(current != word.substr(0, current.length() )) {
        BoggleGUI::labelCube(pos.getX(), pos.getY(), gameBoard[pos.getX()][pos.getY()], false);
        return false;
    }
    markSquare(pos);
    for (int n = 1; n <= 8; n++)
        if (humanWordSearchHelper(current, word, adjacentCube(pos, n) ) )
            return true;
    unmarkSquare(pos);
    BoggleGUI::labelCube
            (pos.getX(), pos.getY(), gameBoard[pos.getX()][pos.getY()], false);
    return false;
}

int Boggle::getHumanScore() const {
    return humanScore;
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    for (int n = 0; n < BOARD_SIZE; n++)
        for (int m = 0; m < BOARD_SIZE; m++) {
            resetGameBoard();
            string current = "";
            Point pos(n,m);
            computerWordSearchHelper(current, pos, result);
        }
    return result;
}

void Boggle::computerWordSearchHelper(string current, Point pos, Set<string>& result) {
    if(isOutside(pos)) return;
    if(isMarked(pos)) return;
    if( !(dict.containsPrefix(current)) ) return;
    current = current + gameBoard[pos.getX()][pos.getY()];
    if(checkWord(current) ) {
        result.add(current);
        alreadyUsed.add(current);
        increaseComputerScore(current);
    }
    markSquare(pos);
    for (int n = 1; n <= 8; n++)
        computerWordSearchHelper(current,adjacentCube(pos, n), result );
    unmarkSquare(pos);
}

int Boggle::getScoreComputer() const {
    return computerScore;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int n = 0; n < BOARD_SIZE; n++) {
        for (int m = 0; m <BOARD_SIZE; m++)
            out << boggle.getGameBoard()[n][m];
        out << endl;
        }
    return out;
}

Grid<char> Boggle::getGameBoard() const {
    return gameBoard;
}

Set<string> Boggle::getAlreadyUsed() const {
    return alreadyUsed;
}

bool Boggle::isOutside(Point p) const {
    double x = p.getX();
    double y = p.getY();
    if (x < 0 || x > BOARD_SIZE - 1 || y < 0 || y > BOARD_SIZE - 1)
        return true;
    else
        return false;
}

bool Boggle::isMarked(Point p) const {
    return markedCubes[p.getX()][p.getY()];
}

void Boggle::markSquare(Point p) {
    markedCubes[p.getX()][p.getY()] = true;
}

void Boggle::unmarkSquare(Point p) {
    markedCubes[p.getX()][p.getY()] = false;
}

void Boggle::addToAlreadyUsed(string word) {
    alreadyUsed.add(word);
}

void Boggle::increaseHumanScore(string word) {
    humanScore = humanScore + (word.length() - 3);
}

void Boggle::increaseComputerScore(string word) {
    computerScore = computerScore + (word.length() - 3);
}

Point Boggle::adjacentCube(Point current, int dir) const {
    switch (dir) {
    case 1: return Point(current.getX()-1,current.getY()-1);
    case 2: return Point(current.getX()-1,current.getY());
    case 3: return Point(current.getX()-1,current.getY()+1);
    case 4: return Point(current.getX(),current.getY()-1);
    case 5: return Point(current.getX(),current.getY()+1);
    case 6: return Point(current.getX()+1,current.getY()-1);
    case 7: return Point(current.getX()+1,current.getY());
    case 8: return Point(current.getX()+1,current.getY()+1);
    }
}

void Boggle::resetGameBoard() {
    for (int n = 0; n < BOARD_SIZE; n++)
        for (int m = 0; m < 4; m++)
            markedCubes[n][m] = false;
}

void Boggle::manualBoardSetup(string scheme) {
    for (int n = 0; n < BOARD_SIZE; n++)
        for (int m = 0; m < BOARD_SIZE; m++)
            gameBoard[n][m] = scheme[BOARD_SIZE*n+m];
}

void Boggle::randomBoardSetup() {
    Vector<int> randomPos;
    for (int n = 0; n < 16; n++)
        randomPos.add(randomInteger(0,5)); //Chooses which letter of each cube will be shown.
    shuffle(CUBES, 16); // Randomly shuffles the cubes to different positions.
    for (int n = 0; n < BOARD_SIZE; n++)
        for (int m = 0; m < BOARD_SIZE; m++) {
            int index = BOARD_SIZE*n+m;
            string cube = CUBES[index];
            gameBoard[n][m] = cube[randomPos[index]];
        }
}

void Boggle::initialSetup(Lexicon& dictionary) {
    dict = dictionary;
    humanScore = 0;
    computerScore = 0;
    markedCubes.resize(BOARD_SIZE,BOARD_SIZE);
    resetGameBoard();
}
