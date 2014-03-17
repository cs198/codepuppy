/* Name: Lindsey Txakeeyang
 * Class: 106B, Wednesday 4:15 section
 * Assignment: Boggle
 * This program replicates the game of Boggle with one human player and one computer player.
 */

#include "Boggle.h"
#include "grid.h"
#include "bogglegui.h"


static const int NUM_CUBES = 16; // the number of cubes in the game
static const int CUBE_SIDES = 6; // the number of sides on each cube

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    gameboard.resize(BOARD_SIZE, BOARD_SIZE);
    visitedTiles.resize(BOARD_SIZE, BOARD_SIZE);
    compVisitedTiles.resize(BOARD_SIZE, BOARD_SIZE);
    if (boardText == "") {
        boardText = shuffleCubes();
        setBoard(boardText);
    } else {
        setBoard(boardText);
    }
}

string Boggle::shuffleCubes() {
    Grid<string> tempBoard;
    tempBoard.resize(BOARD_SIZE, BOARD_SIZE);
    string boardConfiguration = "";
    string temp;
    string randomLetter;

    int row = 0;
    for (int i = 0; i < NUM_CUBES; i++) {
        int column = 0;
        column = i % 4;
        tempBoard.set(row, column, CUBES[i]);
        if (i % 4 == 3) row++;
    }

    shuffle(tempBoard);
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            temp = tempBoard.get(r, c);
            int randomNum = randomInteger(0, CUBE_SIDES - 1);
            randomLetter = temp[randomNum];
            boardConfiguration.append(randomLetter);
        }
    }
    return boardConfiguration;
}

void Boggle::setBoard(string boardText) {
    int row = 0;
    char letter;
    for (int i = 0; i < boardText.length(); i++) {
        int column = 0;
        column = i % 4;
        letter = boardText[i];
        gameboard.set(row, column, letter);
        if (i % 4 == 3) row++;
    }
    BoggleGUI::labelAllCubes(boardText);
}

char Boggle::getLetter(int row, int col) {
    if (gameboard.inBounds(row, col)) {
        return gameboard.get(row, col);
    } else {
        int exception = 0;
        throw exception;
    }
}

void Boggle::printBoard() {
    for(int r = 0; r < BOARD_SIZE; r++) {
        for (int c=0; c < BOARD_SIZE; c++) {
            cout << gameboard.get(r,c);
        }
        cout << endl;
    }
}

void Boggle::addHumanUsedWord(string word) {
    BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
    humanWords.add(word);
}

int Boggle::numberOfHumanWords() {
    int number = humanWords.size();
    return number;
}

string Boggle::printHumanUsedWords() {
    string words = humanWords.toString();
    return words;
}

bool Boggle::checkWord(string word, Lexicon& dictionary) {
    if (isRightLength(word) && !hasBeenUsed(word) && inDictionary(word, dictionary)&& humanWordSearch(word)) return true;
    else return false;
}

bool Boggle::isRightLength(string word) {
    if (word.length() >= 4) {
        return true;
    }
    else return false;
}

bool Boggle::hasBeenUsed(string word) {
    word = toUpperCase(word);
    if (humanWords.contains(word)) {
        return true;
    }
    else return false;
}

bool Boggle::inDictionary(string word, Lexicon& dictionary) {
    word = toUpperCase(word);
    if (dictionary.contains(word)) {
        return true;
    }
    else return false;
}

bool Boggle::humanWordSearch(string word) {
    word = toUpperCase(word);
    string temp;
    if (!isRightLength(word)) return false;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            temp = gameboard.get(r, c);
            BoggleGUI::setAnimationDelay(200);
            BoggleGUI::labelCube(r, c, gameboard.get(r, c), true);
            if (word[0] == temp[0]) {
                if (word.length() == 1) {
                    return true;
                }
                if (word.length() > 1) {
                    word = word.substr(1);
                    if (neighborsCanSpellWord(word, r, c)) {
                        return true;
                    }
                }
            } else {
                BoggleGUI::labelCube(r, c, gameboard.get(r,c), false);
            }
        }
    }
    return false;
}

bool Boggle::neighborsCanSpellWord(string word, int r, int c) {
    for (int x = r - 1; x <= r + 1; x++) {
        for (int y = c - 1; y <= c + 1; y++) {
            if (isValidUnusedWord(word, x, y)) {
                visitedTiles.set(x, y, true);
                if (word.length() == 1) {
                    return true;
                } else if ((word.length() > 1)) {
                    BoggleGUI::setAnimationDelay(200);
                    BoggleGUI::labelCube(x, y, gameboard.get(x, y), false);
                    word = word.substr(1);
                    if (neighborsCanSpellWord(word, x, y)) {
                        return true;
                    }
                    BoggleGUI::labelCube(x, y, gameboard.get(x, y), false);
                    visitedTiles.set(x, y, false);
                } else {
                    BoggleGUI::labelCube(x, y, gameboard.get(x, y), false);
                    return false;
                }
            }
        }
    }
}

bool Boggle::isValidUnusedWord(string word, int x, int y) {
    if (gameboard.inBounds(x, y) && visitedTiles.get(x, y) == false) {
        BoggleGUI::setAnimationDelay(200);
        BoggleGUI::labelCube(x, y, gameboard.get(x,y), true);
        char temp = gameboard.get(x, y);
        char letter = word[0];
        if (letter == temp) {
            return true;
        }
    } else return false;
}

int Boggle::humanScore() {
    int score = 0;
    for (string word : humanWords) {
        int wordlength = word.length();
        score = score + (wordlength - 3);
    }
    BoggleGUI::setScore(score, BoggleGUI::HUMAN);
    return score;
}

Set<string> Boggle::computerWordSearch(Lexicon& dictionary) {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            compVisitedTiles.set(r, c, true);
            string word = "a";
            word[0] = gameboard.get(r, c);
            computerWordSearchHelper(word, r, c, dictionary);
            compVisitedTiles.set(r, c, false);
        }
    }
    return computerWords;
}

void Boggle::computerWordSearchHelper(string word, int r, int c, Lexicon& dictionary) {
    if (!dictionary.containsPrefix(word)) {
        return;
    }
    if (dictionary.contains(word) && !humanWords.contains(word) && isRightLength(word)) {
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
        computerWords.add(word);
    }
    for (int x = r - 1; x <= r + 1; x++) {
        for (int y = c - 1; y <= c + 1; y++) {
            if (gameboard.inBounds(x, y) && compVisitedTiles[x][y] == false) {
                compVisitedTiles.set(x, y, true);
                word += gameboard.get(x, y);
                computerWordSearchHelper(word, x, y, dictionary);
                compVisitedTiles.set(x, y, false);
                word.erase(word.length() - 1, 1);
            }
        }
    }
}

int Boggle::computerScore() {
    int score = 0;
    for (string word: computerWords) {
        int wordlength = word.length();
        score = score + (wordlength - 3);
    }
    BoggleGUI::setScore(score, BoggleGUI::COMPUTER);
    return score;
}

int Boggle::numberOfComputerWords() {
    int number = computerWords.size();
    return number;
}

string Boggle::printComputerWords() {
    string words = computerWords.toString();
    return words;
}

void Boggle::resetSets() {
    humanWords.clear();
    computerWords.clear();
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            cout << boggle.gameboard.get(r, c);
        }
    }
    return out;
}
