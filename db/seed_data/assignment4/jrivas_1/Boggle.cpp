// This is the .cpp file. There are a couple of helper
// functions to help with the recursion. I got some
// "help" from myself and old code
// from when I previously took 106b, but most
// of the code is revised/original,
// and all of the work has been done by me for this class.

#include "Boggle.h"
#include "shuffle.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

/*
 * Creates the board according to whether or not boardText
 * contains letters. If it does not have letters, it shuffles
 * the cubes and chooses a random letter from the cube to display
 * and store.
 */
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    shuffle(CUBES, 16);
    dict = dictionary;
    board.resize(4,4);
    if (boardText.size() == 0) {
        for (int i = 0; i < 16; i++) {
            string s = CUBES[i];
            char c = s.at(randomInteger(0, s.size() - 1));
            board[i/4][i%4] = c;
            BoggleGUI::labelCube(i/4,i%4,c,false);
        }
    }
    else {
        BoggleGUI::labelAllCubes(boardText);
        for (int i = 0; i < 16; i++) {
            board[i/4][i%4] = boardText.at(i);
        }
    }
}

/*
 * Returns the selected letter.
 */
char Boggle::getLetter(int row, int col) {
    char c = board[row][col];
    return c;
}

/*
 * Checks the word for basic problems such as
 * not being long enough, not being in the dictionary,
 * and already used.
 */
bool Boggle::checkWord(string word) {
    if (word.length() > 4) return false;
    if (!dict.contains(word)) return false;
    word = toUpperCase(word);
    if (usedWords.contains(word)) return false;
    return true;
}

/*
 * Returns true or false depending on whether or not the
 * word can be found in the boggle cubes. It first makes
 * a grid to mimic the cube grid, setting it all to false,
 * and clears the board from any highlighting. It then goes
 * through each cube to see if it begins with the appropriate letter
 * and, if it does, sends it through a recursive function to see
 * if the cube can make the letter.
 */
bool Boggle::humanWordSearch(string word) {
    int onBoard = 0;
    Grid<bool> grid(4,4);
    for (int row = 0; row < grid.numRows(); row++) {
        for (int col = 0; col < grid.numCols(); col++) {
            grid.set(row,col,false);
            BoggleGUI::labelCube(row,col,board.get(row,col),false);
        }
    }
    for (int row = 0; row < grid.numRows(); row++) {
        for (int col = 0; col < grid.numCols(); col++) {
            BoggleGUI::labelCube(row,col,board.get(row,col),true);
            if (board[row][col] == word.at(0) && onBoard == 0) {
                if (Boggle::isOnBoard(word,row,col,board,grid)) {
                    BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                    usedWords.add(word);
                    BoggleGUI::labelCube(row,col,board.get(row,col),true);
                    return true;
                } else {
                    BoggleGUI::labelCube(row,col,board.get(row,col),false);
                    grid.set(row,col,false);
                }
            }
            BoggleGUI::labelCube(row,col,board.get(row,col),false);
        }
    }
    usedWords.add(word);
    return false;
}

/*
 * Using the true/false grid, this recursive function goes through and checks all the
 * neighboring tiles to see if it can form the word. It goes through letter by letter and
 * returns false if there is no hope in making the word.
 */

bool Boggle::isOnBoard(string word, int row, int col, Grid<char>& board, Grid<bool> grid) {
    if (word.length() == 0) {
        return true;
    } else {
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {
                if (i >= 0 && i < 4 && j >= 0 && j < 4) {
                    if (word.at(0) == board.get(i,j) && !grid.get(i,j)) {
                        string subword = word.substr(1);
                        grid.set(i,j,true);
                        if(Boggle::isOnBoard(subword,i,j,board,grid)) {
                            BoggleGUI::setAnimationDelay(50);
                            BoggleGUI::labelCube(i,j,board.get(i,j),true);
                            return true;
                        } else {
                            BoggleGUI::setAnimationDelay(50);
                            BoggleGUI::labelCube(i,j,board.get(i,j),false);
                            grid.set(i,j,false);
                        }
                    }
                }
            }
        }
    } return false;
}

/*
 * Uses the usedWords set to tally up the points.
 */
int Boggle::humanScore() {
    int score = 0;
    for (string s: usedWords) {
        score += s.length() - 3;
    }
    BoggleGUI::setScore(score,BoggleGUI::HUMAN);
    return score;
}

/*
 * Checks the words for the computer. It goes through all of the cubes
 * and sends that cube into a recursive function to see if it can make
 * a word with it. It then removes words found by the user.
 */

Set<string> Boggle::computerWordSearch() {
    result = usedWords;
    Grid<bool> grid(4, 4);
    for (int row = 0; row < grid.numRows(); row++) {
        for (int col = 0; col < grid.numCols(); col++) {
            grid.set(row,col,false);
        }
    } for (int row = 0; row < board.numRows(); row++) {
        for (int col = 0; col < board.numCols(); col++) {
            char ch = board[row][col];
            string word = charToString(ch);
            grid.set(row,col,true);
            if (!isOnBoardForComputer(word,row,col,board,grid,dict,result)) {
                grid.set(row,col,false);
            }
        }
    }
    for (string z : usedWords) {
        result.remove(z);
    }
    for (string z : result) {
        BoggleGUI::recordWord(z, BoggleGUI::COMPUTER);
    }
    return result;
}

/*
 * Checks to see if the word can be continued, and then ends it by checking
 * to see if it's a word itself, returning true if it can be a word. In this
 * way, the word can be continued if there are longer words possible.
 */
bool Boggle::isOnBoardForComputer(string word, int row, int col, Grid<char>& board, Grid<bool> grid, Lexicon & dict, Set<string> & result) {
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = (col - 1); j <= (col + 1); j++) {
            if (i >= 0 && i < 4 && j >= 0 && j < 4) {
                string word2 = word + board.get(i,j);
                if ((dict.containsPrefix(word2) || dict.contains(word2)) && !grid.get(i,j)) {
                    grid.set(i,j,true);
                    if (!isOnBoardForComputer(word2, i, j, board, grid, dict, result)) {
                        grid.set(i,j,false);
                    }
                }
            }
        }
    }
    if (dict.contains(word) && word.length() >= 4) {
        if (!result.contains(word)) {
            result.add(word);
            return true;
        }
    }
    return false;
}

/*
 * Computes the score using the result set.
 */
int Boggle::getScoreComputer() {
    int score = 0;
    for (string s: result) {
        score += s.length() - 3;
    }
    BoggleGUI::setScore(score,BoggleGUI::COMPUTER);
    return score;
}

/*
 * Outputs the usedWords of boggle.
 */
ostream& operator<<(ostream& out, Boggle& boggle) {
    cout << "Your words (" << boggle.usedWords.size() << "): {";
    for (string s : boggle.usedWords) {
        cout << "\"" << s << "\"";
    }
    cout << "}" << endl;
    return out;
}
