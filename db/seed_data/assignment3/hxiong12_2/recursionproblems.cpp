// Assignment 3: Recursion Problems
// CS106B Winter 2014
// Coded by Helen Xiong
// SL: Jimmy Lee
// Sources used: Assignment spec, YEAH powerpoint
// Comments on "recursionproblems.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "gwindow.h"
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "queue.h"
#include "foreach.h"
#include "recursionproblems.h"
using namespace std;

//Optional Problem 1
int countKarelPaths(int street, int avenue) {
    return 0;
}

//Problem 2
int convertStringToInteger(string exp) {
    if (exp == "") { //base case, empty string
        return 0;
    } else if (exp.length() == 1) { //base case, single digit
        return char(exp[0]) - '0';
    } else if (exp[0] == '-') { //for negatives
        return -convertStringToInteger(exp.substr(1));
    } else { //converts ones place into int, then adds remainder of string * 10
        return convertStringToInteger(exp.substr(exp.size() - 1, 1)) + 10*convertStringToInteger(exp.substr(0, exp.size() - 1));
    }
}

//Problem 3
bool isBalanced(string exp) {
    if (exp == "") return true; //base case
    else if (exp.find("()") != string::npos) {
        exp = exp.erase(exp.find("()"), 2);
        return isBalanced(exp);
    } else if (exp.find("[]") != string::npos) {
        exp = exp.erase(exp.find("[]"), 2);
        return isBalanced(exp);
    } else if (exp.find("{}") != string::npos) {
        exp = exp.erase(exp.find("{}"), 2);
        return isBalanced(exp);
    }
    else if (exp[0] == '[' && exp[exp.size() - 1] == ']') return isBalanced(exp.substr(1, exp.size() - 2));
    else if (exp[0] == '{' && exp[exp.size() - 1] == '}') return isBalanced(exp.substr(1, exp.size() - 2));
    else if (exp[0] == '(' && exp[exp.size() - 1] == ')') return isBalanced(exp.substr(1, exp.size() - 2));
    else return false;
}

//Problem 4
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order > 0) {
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, x + size/2, y + size * sqrt(3) / 2);
        gw.drawLine(x + size/2, y + size * sqrt(3) / 2, x + size, y);
        drawSierpinskiTriangle(gw, x, y, size/2, order - 1);
        drawSierpinskiTriangle(gw, x + size/2, y, size/2, order - 1);
        drawSierpinskiTriangle(gw, x + size/4, y + size * sqrt(3) / 4, size/2, order - 1);
    }
}

//Problem 5
/*Helper for floodFill method.
 * Returns true if a given pixel, denoted by (x, y), is in the range of the
 * graphics window, defined by (width, height).*/
bool pixelInBound(int x, int y, int width, int height) {
    if (x > -1 && x < width && y > -1 && y < height) return true;
    else return false;
}


void floodFill(int x, int y, int width, int height, int color) {
    int oldColor = getPixelColor(x, y);
    if (pixelInBound(x, y, width, height)) setPixelColor(x, y, color);
    if (pixelInBound(x + 1, y, width, height) && getPixelColor(x + 1, y) == oldColor) {
        floodFill(x + 1, y, width, height, color);
    }
    if (pixelInBound(x - 1, y, width, height) && getPixelColor(x - 1, y) == oldColor) {
        floodFill(x - 1, y, width, height, color);
    }
    if (pixelInBound(x, y - 1, width, height) && getPixelColor(x, y - 1) == oldColor) {
        floodFill(x, y - 1, width, height, color);
    }
    if (pixelInBound(x, y + 1, width, height) && getPixelColor(x, y + 1) == oldColor) {
        floodFill(x, y + 1, width, height, color);
    }
}

//Problem 6
/*Helper for grammarGenerate.
 * Reads the input file of grammar, and parses the lines into symbols and terminal/non-terminal symbols.
 * Stores these into a map, which is called by the other methods.*/
void readFile(istream& input, Map<string, Vector<string> >& map) {
    string line;
    while (!input.fail()) {
        while (getline(input, line)) {
            //stores the keys in the map
            string key = line.substr(0, line.find("::"));
            if (key == "") throw "Blank key in grammar file."; //checks for an empty key in the grammar file

            //separates the string into rules
            string valueString = line.substr(line.find("=") + 1);
            if (char(valueString[valueString.size() - 1]) == '\r') valueString.erase(valueString.size() - 1, 1); //removes end-of-line character from string

            //parses the remainder of the string
            Vector<string> value = stringSplit(valueString, "|");
            if (map.containsKey(key)) { //checks if the file contains a duplicate entry for a symbol, in which case it throws an exception
                throw "Grammar contains more than one line for the same element.";
            } else {
                map.put(key, value);
            }
        }
    }
}

/*Helper for grammarGenerate.
 * Generates a sentence using the rules of the grammar.
 * Uses pseudocode from assignment spec*/
void generateSentence(string symbol, int times, string& sentence, Vector<string>& v, Map<string, Vector<string> >& map) {
    if (symbol == "") throw "Symbol parameter passed to function is empty."; //crashes the program if there is a blank symbol in the grammar.
    if (!map.containsKey(symbol)) { //if symbol is not contained in the grammar file, the program will print whatever string the user has input
        sentence += symbol + " ";
    } else {
        //chooses terminal/non-terminal from the entry for the symbol, and splits into components delimited by " "
        int random = randomInteger(0, map[symbol].size() - 1);
        Vector<string> sentencePart = stringSplit(map[symbol][random], " ");
        for (string key : sentencePart) {
            if (!map.containsKey(key)) { //a terminal element has been reached
                sentence += key + " ";
            } else {
                generateSentence(key, times, sentence, v, map);
            }
        }
    }
}

//Generates random sentence
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Vector<string> v;
    Map<string, Vector<string> > map;

    readFile(input, map);
    while (v.size() < times) { //fills in vector with the user input number of symbols
        string sentence = "";
        generateSentence(symbol, times, sentence, v, map);
        v.add(sentence);
    }
    return v;
}


