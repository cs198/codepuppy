/* Connie Zeng
 * CS 106B, Winter 2014
 * ---------------------
 * Assignment 3: Recursion Problems
 * This program contains several functions that involve recursion,
 * including a Grammar Solver that generates random sentences from a grammar
 * provided by an input file.
 */

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

/* Function prototypes for helper functions */

void floodFill(int x, int y, int width, int height, int color, int oldColor);
void storeFile(istream& input, Map<string, Vector<string> >& grammar);
string genElement(Map<string, Vector<string> >& grammar, string symbol);


int countKarelPaths(int street, int avenue) {
    // Optional
    return 0;
}

/* Function: convertStringToInteger
 * Usage: int converted = convertStringToInteger(exp);
 * ----------------------------------------------------
 * This function returns the passed string as an integer. It handles
 * both positive and negative cases, but assumes the passed string is a
 * valid, non-empty number within the bounds of an int.
 */

int convertStringToInteger(string exp) {
    // negative case
    if(exp[0] == '-'){
        return -1 * convertStringToInteger(exp.substr(1));
    }
    // base case
    else if (exp.length() == 1){
        return exp[0] - '0';
    }
    // recursive case
    else{
        int index = exp.length() - 1;
        return 10 * convertStringToInteger(exp.substr(0, index)) + exp[index] - '0';
    }
}

/* Function: isBalanced
 * Usage: bool balanced = isBalanced(exp);
 * ----------------------------------------
 * This function returns whether the passed string has balanced parentheses,
 * brackets, and curly braces. It checks if the string can be reduced to the
 * empty string by removing one pair of operators at a time. It assumes that
 * the string contains no other types of characters, but may be empty.
 */

bool isBalanced(string exp) {
    // base case
    if(exp == "") return true;
    // recursive case
    else{
        int index = exp.find("()");
        if(index == string::npos) index = exp.find("[]");
        if(index == string::npos) index = exp.find("{}");
        if(index == string::npos) return false;
        else return isBalanced(exp.erase(index, 2));
    }
}

/* Function: drawSierpinskiTriangle
 * Usage: drawSierpinskiTriangle();
 * ---------------------------------
 * This function draws a Sierpinski triangle of the specified order and side
 * length in the passed graphical window, with its top left vertex at (x, y).
 * A Sierpinksi triangle of order K can be drawn as three half-sized Sierpinski
 * triangles of order K-1, each forming a vertex of the order K triangle.
 */

void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    // Coordinates of bottom vertex of triangle
    double width = 0.5 * size;
    double height = (sqrt(3)/2) * size;
    // base case
    if(order == 1){
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, x + width, y + height);
        gw.drawLine(x + size, y, x + width, y + height);
    }
    // recursive case
    else if(order > 1){
        if(size > 0){
            drawSierpinskiTriangle(gw, x, y, size / 2, order - 1);
            drawSierpinskiTriangle(gw, x + width, y, size / 2, order - 1);
            drawSierpinskiTriangle(gw, x + 0.5 * width, y + 0.5 * height, size / 2, order - 1);
        }
        // Skips over iterations that are too small to draw
        else{
            drawSierpinskiTriangle(gw, x, y, 0, 1);
            drawSierpinskiTriangle(gw, x + width, y, 0, 1);
            drawSierpinskiTriangle(gw, x + 0.5 * width, y + 0.5 * height, 0, 1);
        }
    }
}

/* Function: floodFill
 * Usage: floodFill(x, y, width, height, color);
 * ----------------------------------------------
 * This function identifies the region that is contiguous with and the same
 * color as the pixel at (x, y), and then fills in that region with the
 * specified color.
 */

void floodFill(int x, int y, int width, int height, int color) {
    int oldColor = getPixelColor(x, y);
    floodFill(x, y, width, height, color, oldColor);
}

/* Function: floodFill
 * Usage: floodFill(x, y, width, height, color, oldColor);
 * --------------------------------------------------------
 * This is a helper function for the primary floodFill function. It keeps track
 * of the color of the original pixel so that getPixelColor() only needs to be
 * called once.
 */

void floodFill(int x, int y, int width, int height, int color, int oldColor) {
    // base case
    setPixelColor(x, y, color);
    //recursive case
    if(x > 0 && getPixelColor(x - 1, y) == oldColor){
        floodFill(x - 1, y, width, height, color, oldColor);
    }
    if(x < width && getPixelColor(x + 1, y) == oldColor){
        floodFill(x + 1, y, width, height, color, oldColor);
    }
    if(y > 0 && getPixelColor(x, y - 1) == oldColor){
        floodFill(x, y - 1, width, height, color, oldColor);
    }
    if(y < height && getPixelColor(x, y + 1) == oldColor){
        floodFill(x, y + 1, width, height, color, oldColor);
    }
}

/* Function: grammarGenerate
 * Usage: Vector<string> result = grammarGenerate(input, symbol, times);
 * ----------------------------------------------------------------------
 * This function generates random elements of a grammar based on a BNF
 * description from an input file. It generates the passed symbol the
 * specified number of times and returns the results in a vector. An exception
 * is thrown if the passed symbol is empty.
 */

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if(symbol == ""){
        throw "Invalid input: symbol cannot be an empty string.";
    }
    else{
        Map<string, Vector<string> > grammar;
        storeFile(input, grammar);

        Vector<string> v;
        for(int i = 0; i < times; i++){
            v.add(genElement(grammar, symbol));
        }
        return v;
    }
}

/* Function: storeFile
 * Usage: storeFile(input, grammar);
 * ----------------------------------
 * This function reads the input file and stores the BNF grammar in a Map,
 * with non-terminals as keys and expansion rules as values. It assumes that
 * each non-terminal is listed on the left of a line, followed by rules. An
 * exception is thrown if a non-terminal appears on the left of a line twice.
 * A ::= separates each non-terminal from its rules, and a | separates
 * rules belonging to the same nonterminal. A rule may contain multiple parts,
 * separated by a single space, but no other whitespace will be present.
 */

void storeFile(istream& input, Map<string, Vector<string> >& grammar) {
    string line;
    while(getline(input, line)){
        int index = line.find("::=");
        string nonterminal = line.substr(0, index);
        if(grammar.containsKey(nonterminal)){
            throw "Illegal grammar format: non-terminal listed twice.";
        }
        else{
            Vector<string> rules = stringSplit(line.substr(index + 3), "|");
            grammar[nonterminal] = rules;
        }
    }
}

/* Function: genElement
 * Usage: string terminals = genElement(grammar, symbol);
 * -------------------------------------------------------
 * This function randomly expands the passed symbol based on the rules in the
 * grammar. If the symbol is not found as a non-terminal, it is assumed to be
 * a terminal. The expansion may contain multiple parts, each of which may be
 * a non-terminal to further expand. Once all parts have been expanded into
 * terminals, the result is returned as a string.
 */

string genElement(Map<string, Vector<string> >& grammar, string symbol) {
    string terminals = "";
    // base case
    if(!grammar.containsKey(symbol)) terminals = symbol;
    // recursive case
    else{
        Vector<string> rules = grammar[symbol];
        int r = randomInteger(0, rules.size() - 1);
        Vector<string> parts = stringSplit(rules[r], " ");
        for(string elem: parts){
            terminals += (" " + genElement(grammar, elem));
        }
    }
    return trim(terminals);
}

