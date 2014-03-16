/* Connie Zeng
 * CS 106B, Winter 2014
 * ---------------------
 * Assignment 4: Boggle
 * This program lets the user play a game of Boggle. This file implements the
 * Boggle class described in Boggle.h.
 *
 * (1 late day)
 */

#include <string>
#include <iostream>
#include "shuffle.h"
#include "strlib.h"
#include "Boggle.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    boggleDictionary = dictionary;
    score = 0;
    remScore = 0;

    board.resize(4, 4);
    if(boardText == ""){
        shuffle(CUBES, 16);
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                board[i][j] = shuffle(CUBES[4 * i + j])[0];
                BoggleGUI::labelCube(i, j, board[i][j]);
            }
        }
    }
    else if(boardText.length() != 16){
        throw "Invalid string length.";
    }
    else{
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                board[i][j] = boardText[4 * i + j];
                BoggleGUI::labelCube(i, j, board[i][j]);
            }
        }
    }
}

char Boggle::getLetter(int row, int col) {
    if(row < 0 || row >= 4) throw row;
    if(col < 0 || col >= 4) throw col;
    return board[row][col];
}

bool Boggle::checkWord(string word) {
    return(boggleDictionary.contains(word) && !words.contains(word) && word.length() >= 4);
}

bool Boggle::humanWordSearch(string word) {
    Set<string> chosen;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(humanWordSearch(word, i, j, chosen)){
                words.add(word);
                score += word.length() - 3;
                BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                BoggleGUI::setScore(score, BoggleGUI::HUMAN);
                return true;
            }
        }
    }
    return false;
}

bool Boggle::humanWordSearch(string word, int x, int y, Set<string>& chosen) {
    string choice = integerToString(x) + integerToString(y);
    if(x < 0 || x >= 4 || y < 0 || y >= 4) return false;
    if(chosen.contains(choice)) return false;
    if(board[x][y] == word[0]){
        chosen.add(choice);
        BoggleGUI::setHighlighted(x, y, true);
        if(word.length() == 1) return true;
        else{
            for(int i = x - 1; i <= x + 1; i++){
                for(int j = y - 1; j <= y + 1; j++){
                    if(humanWordSearch(word.substr(1), i, j, chosen)) return true;
                }
            }
            chosen.remove(choice);
            BoggleGUI::setHighlighted(x, y, false);
        }
    }
    return false;
}

int Boggle::humanScore() {
    return score;
}

Set<string> Boggle::humanWords() {
    return words;
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    Set<string> chosen;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            findWords(i, j, "", chosen, result);
        }
    }
    return result;
}

void Boggle::findWords(int x, int y, string wordSoFar, Set<string>& chosen, Set<string>& result) {
    string choice = integerToString(x) + integerToString(y);
    if(x >= 0 && x < 4 && y >= 0 && y < 4 && !chosen.contains(choice)){
        wordSoFar += board[x][y];
        if(boggleDictionary.containsPrefix(wordSoFar)){
            chosen.add(choice);
            if(boggleDictionary.contains(wordSoFar) && !words.contains(wordSoFar) && wordSoFar.length() >= 4){
                result.add(wordSoFar);
                remScore += wordSoFar.length() - 3;
                BoggleGUI::recordWord(wordSoFar, BoggleGUI::COMPUTER);
                BoggleGUI::setScore(remScore, BoggleGUI::COMPUTER);
            }
            for(int i = x - 1; i <= x + 1; i++){
                for(int j = y - 1; j <= y + 1; j++){
                    findWords(i, j, wordSoFar, chosen, result);
                }
            }
            chosen.remove(choice);
        }
    }
}

int Boggle::getScoreComputer() {
    return remScore;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            out << boggle.getLetter(i, j);
        }
        out << endl;
    }
    return out;
}
