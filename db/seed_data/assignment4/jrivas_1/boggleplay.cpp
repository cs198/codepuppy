// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "bogglegui.h"
#include "Boggle.h"

using namespace std;

string getUserString();
void userTurn(Lexicon& dictionary, Boggle b);
void computerTurn(Lexicon& dictionary, Boggle b);
void outputUsedWords(Set<string> words);
void outputBoard(Boggle b);
bool IsOnBoard(string word, int row, int col, Boggle b, Grid<bool> grid);

void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(4,4);
    string s = "";
    if (getYesOrNo("Do you want to generate a random board? ")) {
        s = "";
    }
    else {
        s = getUserString();
    }
    const Boggle b(dictionary,toUpperCase(s));
    clearConsole();
    userTurn(dictionary, b);
    computerTurn(dictionary,b);
}

/*
 * Gets the user string, and returns an empty string if they
 * choose to make a random board.
 */
string getUserString() {
    string s = "";
    while(true) {
        s = getLine("Type the 16 letters to appear on the board: ");
        if (s.size() == 16) {
            int good = 0;
            for (int i = 0; i < s.size(); i++) {
                if (s[i] >= 'a' && s[i] <= 'z') good++;
            } if (good == 16) break;
        }
        cout << "That is not a valid 16-letter board string. Try again." << endl;
    }
    return s;
}

/*
 * Checks the word and sends the word into boggle function to check
 * to see if the word works; the loop will continue as long as the
 * user does not hit enter.
 */
void userTurn(Lexicon& dictionary, Boggle b) {
    cout << "It's your turn!" << endl;
    outputBoard(b);
    cout << b;
    string prompt = "Type a word (or Enter to Stop): ";
    string word = getLine(toUpperCase(prompt));
    while (word.length() != 0) {
        if (b.checkWord(word)) {
            if (b.humanWordSearch(toUpperCase(word))) {
                clearConsole();
                cout << "You found a new word! " << "\"" << word << "\"" << endl;
            }
            else {
                clearConsole();
                cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
            }
        }
        else {
            clearConsole();
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        }
        cout << "Your score: " << b.humanScore() << endl;
        outputBoard(b);
        cout << b;
        word = getLine(toUpperCase(prompt));
    }
}

/*
 * Outputs the words.
 */

void outputUsedWords(Set<string> words) {
    cout << "Your words (" << words.size() << "): {";
    for (string s : words) {
        cout << "\"" << s << "\"";
    }
    cout << "}" << endl;
}

/*
 * Outputs the board.
 */

void outputBoard(Boggle b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            char c = b.getLetter(i,j);
            cout << c;
        } cout << endl;
    }
    cout << endl;
}

/*
 * Begins the process of going through all the cubes
 * that is done in boggle.cpp. It then outputs the
 * words in the set.
 */
void computerTurn(Lexicon &dictionary, Boggle b) {
    cout << endl << "It's my turn!" << endl;
    Set<string> set = b.computerWordSearch();
    cout << "My words (" << set.size() << "): {";
    for (string s : set) {
        cout << "\"" << s << "\",";
    }
    cout << "}" << endl;
    cout << "My score: " << b.getScoreComputer() << endl;
    cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl << endl;
}
