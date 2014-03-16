/*
 * Recursion Problems
 * Author: Katie Hahm
 * Section Leader: Jimmy Lee
 * 1. converts a given string to an integer
 * 2. checks if a string is balanced in terms of braces, curly braces, and square brackets
 * 3. draws a Sierpinski triangle based on given values
 * 4. fills the clicked shape with the given color
 * 5. generates a number of expressions with the given file of grammar code.
 * the main method contains the console instructions to ask the user for such values
 * and display its results.
 */

// Please feel free to add any other #includes you need!
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "gwindow.h"
#include "hashmap.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "recursionproblems.h"
#include "math.h"
#include "filelib.h"
#include "error.h"
using namespace std;

// decided to skip this problem!
int countKarelPaths(int street, int avenue) {
    return 0;
}

/*
 * converts a given string into an integer
 * parameter: string exp to be converted into an integer
 * returns: integer interpretation of the string
 * it prompts the user for a string of integers,
 * and converts it recursively.
 */
int convertStringToInteger(string exp) {
    if (exp[0] == '-') {
        return -1*(convertStringToInteger(exp.substr(1)));
    }
    else {
        if (exp.size() == 1) {
            return exp[0] - '0';
        }
        else {
            return (exp[0] - '0') * pow(10.0, double(exp.size()-1)) + convertStringToInteger(exp.substr(1));
        }
    }
}

/*
 * parameter: string exp to be considered
 * return: true if the given string is balanced,
 * return: false if the given string is not balanced.
 * the string is balanced if the brackets, curly braces, and square brackets
 * appear in pairs, but they do not need to be always next to each other.
 */
bool isBalanced(string exp) {
    if (exp == "") {
        return true;
    }
    else if (exp.find("()") != string::npos) {
        exp.erase(exp.find("()"), 2);
    }
    else if (exp.find("{}") != string::npos) {
        exp.erase(exp.find("{}"), 2);
    }
    else if (exp.find("[]") != string::npos) {
        exp.erase(exp.find("[]"), 2);
    }
    else {
        return false;
    }
    return isBalanced(exp);
}

/*
 * draws the Sierpinski triangle given certain values
 * parameter: GWindow, x and y coordinate, length of the triangle, order of the triangle
 * if order is 1, it will draw one triangle,
 * if order is 2, it will draw four triangles inside a large triangle,
 * and so on with the pattern.
 */
void drawSierpinskiTriangle(GWindow& gw, int x1, int y1, int size, int order) {
    double x = x1;
    double y = y1;
    if (order == 1) {
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, x + size / 2.0, y + size * sqrt(3.0) / 2.0);
        gw.drawLine(x + size, y, x + size / 2.0, y + size * sqrt(3.0) / 2.0);
    }
    else if (order > 0) {
        double newSize = size / 2.0;
        drawSierpinskiTriangle(gw, x, y, newSize, order-1);
        drawSierpinskiTriangle(gw, x + newSize, y, newSize, order-1);
        drawSierpinskiTriangle(gw, x + (newSize / 2.0), y + (newSize * sqrt(3.0) / 2.0), newSize, order-1);
    }
}

/*
 * Given a window of random shapes with random colors,
 * the user can choose a color and color each shape with that color.
 * parameter: selected x and y location, width and height of the window, color to be colored with
 */
void floodFill(int x, int y, int width, int height, int color) {
    int currentColor = getPixelColor(x, y);
    if (getPixelColor(x,y) != color && x > 0 && y > 0 && x < width && y < height) {
        setPixelColor(x, y, color);
        if (getPixelColor(x-1, y) == currentColor) {
            floodFill(x-1, y, width, height, color);
        }
        if (getPixelColor(x+1, y) == currentColor) {
            floodFill(x+1, y, width, height, color);
        }
        if (getPixelColor(x, y-1) == currentColor) {
            floodFill(x, y-1, width, height, color);
        }
        if (getPixelColor(x, y+1) == currentColor) {
            floodFill(x, y+1, width, height, color);
        }
    }
}

/*
 * translates the file to a hash map of the non-terminals to its values
 * parameter: input stream
 * returns: hashmap of strings to vectors of strings
 */
HashMap<string, Vector<string> > translateFile(istream& input) {
    HashMap<string, Vector<string> > fileMap;
    string line;
    while (getline(input,line)) {
        int index = line.find("::=");
        string values = trim(line.substr(index + 3));
        Vector<string> valueVector = stringSplit(values, "|");
        if (fileMap.containsKey(line.substr(0,index))) {
            throw "There's more than one line for the same non-terminal!";
        }
        else {
            fileMap.put(line.substr(0,index), valueVector);
        }
    }
    return fileMap;
}

/*
 * gets the phrase given the hashmap of nonterminals to its values and the starting symbol
 * parameter: hashmap of nonterminals to its values, starting symbol in string
 * returns: string of expression that was produced from the string
 */
string getPhrase(HashMap<string, Vector<string> >& fileMap, string symbol) {
    if (!fileMap.containsKey(symbol)) {
        return symbol + " ";
    }
    string s;
    Vector<string> currentValue = fileMap.get(symbol);
    int randomIndex = randomInteger(0,currentValue.size() - 1);
    string newSymbol = currentValue.get(randomIndex);
    Vector<string> values = stringSplit(newSymbol, " ");
    for (string x : values) {
        s += getPhrase(fileMap, x);
    }
    return s;
}

/*
 * generates the number of expression indicated by the user
 * parameter: input stream, string of symbol to begin at, integer number of expressions to produce
 * returns: vector of strings to pass to the main method
 * to display on the console to the user
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if (symbol == "") {
        throw "symbol is an empty string";
    }
    else {
        HashMap<string, Vector<string> > grammarFile = translateFile(input);
        Vector<string> v;
        for (int i = 0; i < times; i++) {
            v.add(getPhrase(grammarFile, symbol));
        }
        return v;
    }
}
