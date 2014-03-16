/* Name: Austin Poore
 *
 * This file solves a variety of recursion problems, including converting strings to integers,
 * checking to see whether a given string has balanced bracket operators, drawing
 * Sierpinski Triangle fractals, creating a flood-fill tool, and reading a grammar file
 * and then using it to generate random text.
 *
 * I used lecture slides on the assignment.
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

void convertStringToIntegerHelper(string exp, int& result);
void floodFillHelper(int x, int y, int width, int height, int color, int originalColor);
void storeFile(istream& input, HashMap<string, Vector<string>>& rules);
void generateOutput(HashMap<string, Vector<string>>& rules, string& symbol, int times, Vector<string>& v);
void generatePhrase(string& symbol, HashMap<string, Vector<string>>& rules, string& result);

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

/*
 * This method gets passed a string and returns the integer representation of
 * that string. It checks the first character in the string to see if the integer
 * should be negative, and then removes the minus sign (if present). It then uses
 * a recursive helper method to build up the integer result and, if necessary,
 * multiplies the result by -1 before returning the value.
 */

int convertStringToInteger(string exp) {
    bool negative = false;
    int result = 0;
    if (exp[0] == '-') {
        negative = true;
        exp = exp.substr(1);
    }
    convertStringToIntegerHelper(exp, result);
    if (negative) {
        result *= -1;
    }
    return result;
}

/*
 * This method is a recursive helper method used in converting strings to integers.
 * It takes a string by value and an integer by reference to store the result. It
 * checks to see whether the string is empty, and provided that it is not, the
 * current result gets multiplied by 10 and the next value gets added (and becomes
 * the value in the ones place). Then, the same method gets called on the substring
 * of exp where the first character has been dropped.
 *
 * Base case = empty string
 */

void convertStringToIntegerHelper(string exp, int& result) {
    if (exp.length() != 0) {
        int last = exp[0] - '0';
        result *= 10;
        result += last;
        convertStringToIntegerHelper(exp.substr(1), result);
    }
}

/*
 * This method gets passed a string and checks to make sure that the bracketing
 * operators are balanced. It works by searching for substrings of operator pairs
 * and removing them, and then calling itself on the remaining string, which should
 * still be balanced.
 *
 * Base case = empty string
 */

bool isBalanced(string exp) {
    if (exp.length() == 0) {
        return true;
    }
    if (exp.find("()") != string::npos) {
        exp.erase(exp.find("()"), 2);
    } else if (exp.find("[]")!= string::npos) {
        exp.erase(exp.find("[]"), 2);
    } else if (exp.find("{}")!= string::npos) {
        exp.erase(exp.find("{}"), 2);
    } else {
        return false;
    }
    if (isBalanced(exp)) {
        return true;
    }
}

/*
 * This method gets passed a graphical window, a starting position, a side length,
 * and an integer representing the order of the fractal. The recursive function
 * works by ensuring that the order is not zero, then drawing a triangle and calling
 * itself three times to draw the subsequent triangles of (order - 1) in the proper locations.
 *
 * Base case: order = 0
 */

void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order != 0) {
        gw.drawLine(x, y, x + size/2.0, y + size*sqrt(3)/2.0);
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x + size, y, x + size/2.0, y + size*sqrt(3)/2.0);
        drawSierpinskiTriangle(gw, x, y, size/2.0, order - 1);
        drawSierpinskiTriangle(gw, x + size/2.0, y, size/2.0, order - 1);
        drawSierpinskiTriangle(gw, x + size/4.0, y + size*sqrt(3)/4.0, size/2.0, order - 1);
    }
}

/*
 * This method stores the pixel color of the mouse click and calls a recursive
 * helper method (assuming the user is not trying to change the color to the color
 * it already is.
 */

void floodFill(int x, int y, int width, int height, int color) {
    int firstColor = getPixelColor(x, y);
    if (color != firstColor) { // No need to fill if they are the same.
        floodFillHelper(x, y, width, height, color, firstColor);
    }
}

/*
 * This recursive helper method checks to make sure the pixel being examined is the same
 * color as the color of the pixel where the mouse was first clicked. It then resets
 * the color to the new color and then calls itself on the four surrounding pixels.
 */

void floodFillHelper(int x, int y, int width, int height, int color, int firstColor) {
    int colorNew = getPixelColor(x, y);
    if (colorNew == firstColor) {
        setPixelColor(x, y, color);
        if (x - 1 >= 0) {
            floodFillHelper(x - 1, y, width, height, color, firstColor);
        }
        if (x + 1 < width) {
            floodFillHelper(x + 1, y, width, height, color, firstColor);
        }
        if (y - 1 >= 0) {
            floodFillHelper(x, y - 1, width, height, color, firstColor);
        }
        if (y + 1 < height) {
            floodFillHelper(x, y + 1, width, height, color, firstColor);
        }
    }
}

/*
 * This method takes in an istream which reads the desired file of grammar rules,
 * a symbol to generate, and the number of times to generate the symbol. It throws
 * an error if it is not passed a symbol. It calls helper functions to store the contents
 * of the file and generate the output. It returns a vector with the instances of output.
 */

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if (symbol == "") {
        throw "Please pass a valid symbol.";
    }
    HashMap<string, Vector<string>> rules;
    storeFile(input, rules);
    Vector<string> v;
    generateOutput(rules, symbol, times, v);
    return v;
}

/*
 * This function is a helper which reads the contents of the grammar file and
 * stores the rules in a HashMap. The non-terminals make up the keys, and vectors containing
 * the possibilities for expansion make up the values. This function makes use of the
 * stringSplit function to first split lines of the file into the two sides of the "::="
 * symbol and then to split the right side into its expansion possibilities. It throws
 * an error if the file contains multiple lines for the same key.
 */

void storeFile(istream& input, HashMap<string, Vector<string>>& rules) {
    while(!input.fail()) {
        string line;
        getline(input, line);
        line = trim (line);
        if (line.length() != 0) {
            Vector<string> firstSplit = stringSplit(line, "::=");
            Vector<string> values = stringSplit(firstSplit[1], "|");
            if (rules.containsKey(firstSplit[0])) {
                throw "Invalid input file.";
            }
            rules.put(firstSplit[0], values);
        }
    }
}

/*
 * This function generates the requested output by using a for loop for the desired number
 * of times to call the generatePhrase function. GeneratePhrase is the recursive function
 * that generates the individual instances of the desired symbol by calling itself.
 */

void generateOutput(HashMap<string, Vector<string>>& rules, string& symbol, int times, Vector<string>& v) {
    string result;
    for(int i = 0; i < times; i++) {
        result = "";
        generatePhrase(symbol, rules, result);
        v.add(result);
    }
}

/*
 * This function is the recursive portion of the grammar problem. It calls itself to expand
 * the expressions into terminal components. The base case is when the symbol is a terminal,
 * and the recursive case is everything else. Terminals are identified by the fact that they
 * are not keys in the HashMap which stores the data.
 */

void generatePhrase(string& symbol, HashMap<string, Vector<string>>& rules, string& result) {
    if (!rules.containsKey(symbol)) {
        result += symbol;
        result += " ";
    } else {
        Vector<string> possValues = rules[symbol];
        int choice = randomInteger(0, possValues.size() - 1);
        string chosen = possValues[choice];
        Vector<string> tokens = stringSplit(chosen, " "); // Split expansion rules into individual components
        for (string token: tokens) {
            generatePhrase(token, rules, result);
        }
    }
}
