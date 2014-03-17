/*
 * Name: Lindsey Txakeeyang
 * Class: 106B, Wednesday 4:15 Section
 * Assignment: Recursion Problems
 */

// Please feel free to add any other #includes you need!
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
#include "recursionproblems.h"
using namespace std;

bool isBalancedHelper(string& exp, string characters);
int floodFillHelper(int x, int y, int width, int height, int color, int originalColor);
HashMap<string, Vector<string> > mappingText(istream& input);
string generate(HashMap<string, Vector<string> >& textMap, string symbol);

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

/*
 * Intakes a string of digits and converts it to an integer.
 */
int convertStringToInteger(string exp) {
    if (exp.length() == 1) {
        return exp[0] - '0';
    } else {
        return convertStringToInteger(exp.substr(exp.length() - 1, 1)) + 10 * (convertStringToInteger(exp.substr(0, exp.length() - 1)));
    }
}

/*
 * Intakes a string consisting of only parentheses, brackets, and
 * curly braces and returns true or false according to whether the
 * expression has properly balanced operators.
 */
bool isBalanced(string exp) {
    if (exp == "") {
        return true; // base
    } else if (exp.length() > 0) {
        if (!isBalancedHelper(exp, "()")) return false;
        if (!isBalancedHelper(exp, "{}")) return false;
        if (!isBalancedHelper(exp, "[]")) return false;
    } else return false;
    return isBalanced(exp);
}

/*
 * Helper function that returns true if the characters can be found and
 * then erases them. Returns false if the characters can't be found.
 */
bool isBalancedHelper(string& exp, string characters) {
    int pair = exp.find(characters);
    if (pair >= 0) {
        exp.erase(pair, 2);
        return true;
    } else return false;
}

/*
 * Draws the fractal design Sierpinski Triangle given a GWindow, coordinates for the
 * upper top/left corner of the triangle, length of each side, and the order of the
 * figure to be drawn.
 * Draws nothing if order 0 is passed or less.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order > 0) {
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, (x + size/2), ((y + ((sqrt(3)/2)*size))));
        gw.drawLine(x + size, y, (x + size/2), ((y + ((sqrt(3)/2)*size))));
        drawSierpinskiTriangle(gw, x, y, size/2, order - 1);
        drawSierpinskiTriangle(gw, x + size/2, y, size/2, order - 1);
        drawSierpinskiTriangle(gw, (x + size/4), ((y + ((sqrt(3)/2*size/2)))), size/2, order - 1);
       }
    // base case is order 0, when nothing is performed
}

/*
 * Fills a region with a solid color selected by the user.
 */
void floodFill(int x, int y, int width, int height, int color) {
    int originalColor = getPixelColor(x, y);
    floodFillHelper(x, y, width, height, color, originalColor);
}

/*
 * Performs the recursive action of filling a region with a solid color selected by the user.
 */
int floodFillHelper(int x, int y, int width, int height, int color, int originalColor) {
    setPixelColor(x, y, color);
    if (originalColor == getPixelColor(x, y + 1)) {
        floodFillHelper(x, y + 1, width - 1, height - 1, color, originalColor);
    }
    if (originalColor == getPixelColor(x, y - 1)) {
        floodFillHelper(x, y - 1, width - 1, height - 1, color, originalColor);
    }
    if (originalColor == getPixelColor(x + 1, y)) {
        floodFillHelper(x + 1, y, width - 1, height - 1, color, originalColor);
    }
    if (originalColor == getPixelColor(x - 1, y)) {
        floodFillHelper(x - 1, y, width - 1, height - 1, color, originalColor);
    }
}

/*
 * This method intakes a text file with grammar in Backus-Naur Form
 * and then allows the user to randomly generate elements of the grammar.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if (symbol == "") {
        string msg = "Symbol is empty.";
        throw msg;
    }
    Vector<string> v;
    HashMap<string, Vector<string> > textMap = mappingText(input);
    for (int i = 0; i < times; i++) {
        string generatedText = generate(textMap, symbol);
        v.add(generatedText);
    }
    return v;
}

/*
 * Constructs a new Grammar Solver that intakes the given
 * BNF grammar input file and stores it into a map.
 * Assumes the input file exists, is non-empty and is in
 * a proper valid format.
 */
HashMap<string, Vector<string> > mappingText(istream& input) {
    HashMap<string, Vector<string>> textMap;
    string rule;
    string phrase;
    while (getline(input, rule)) {
        Vector<string> splitRule = stringSplit(rule, "::=");
        rule = splitRule.get(0);
        phrase = splitRule.get(1);
        Vector<string> values = stringSplit(phrase, "|");
        if (!textMap.containsKey(rule)) {
            textMap.put(rule, values);
        } else {
            string msg = "Grammar contains more than one line for the same terminal.";
            throw msg;
        }
    }
    return textMap;
}

/*
 * Generates a random occurrence of the given symbol and returns it as a string.
 * Throws a string exception if the given symbol has a length of 0.
 */
string generate(HashMap<string, Vector<string> >& textMap, string symbol) {
    string generatedText = "";
    if (!textMap.containsKey(symbol)) {
        return symbol;
    }
    if (symbol.length() > 0) {
        Vector<string> values = textMap.get(symbol);
        int vectorSize = values.size();
        int random = randomInteger(0, vectorSize - 1);
        string occurrence = values.get(random);
        Vector<string> splitValues = stringSplit(occurrence, " ");
        for (int i = 0; i < splitValues.size(); i++) {
            generatedText = generatedText + " " + generate(textMap, splitValues[i]);
            generatedText = trim(generatedText);
        }
        return generatedText;
    } else if (symbol.length() == 0) {
        string msg = "Symbol length is 0.";
        throw msg;
    }
    return generatedText;
}
