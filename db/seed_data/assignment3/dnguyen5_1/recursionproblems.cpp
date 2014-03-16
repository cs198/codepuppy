// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

// Please feel free to add any other #includes you need!

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <cmath>
#include <sstream>
#include <string>
#include "hashmap.h"
#include "map.h"
#include "vector.h"
#include "recursionproblems.h"
#include "console.h"
#include "hashset.h"
#include "set.h"
#include "random.h"
#include "queue.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

/*Didn't do Karel warmup*/
int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

/*Takes in a string of numbers and converts it to an integer.*/
int convertStringToInteger(string exp) {
    if (exp == "")  return 0;/*
//    if (exp[0] == '-') {
//        exp = exp.substr(1, exp.size());
//        return -1 * stringToInteger(exp);
//    }*/
    else {
        int length = exp.size();
        int integer = exp[length-1] - '0';
        exp = exp.substr(0, length-1);
        return 10 * convertStringToInteger(exp) + integer;
    }
}

/*Checks if string is balanced. Removes each occurrence of (), []. or {} until no more pairs exist. If the
resulting string is empty, then the function returns true. If the resulting string is not empty, then the function returns
false.*/
bool isBalanced(string exp) {
    if (exp == "")
        return true;
    if (exp.size() == 1)
        return false;
    int index = exp.find("()");
    if (index != -1)
        exp = exp.substr(0, index) + exp.substr(index+2, exp.size()-1);
    int index1 = exp.find("{}");
    if (index1 != -1)
        exp = exp.substr(0, index1) + exp.substr(index1+2, exp.size()-1);
    int index2 = exp.find("[]");
    if (index2 != -1)
        exp = exp.substr(0, index2) + exp.substr(index2+2, exp.size()-1);
    if (index == -1 && index1 == -1 && index2 == -1)
        return false;
    return isBalanced(exp);
    return false;
}

/*Draws Sierpinski Triangles. A sierpinski triangle of order one is one large triangle, and for each additional order, three smaller triangles
are drawn within the larger triangle.*/
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    //base case. Draws large triangle.
    if (order == 1) {
        gw.drawLine(x, y, x+size, y);
        gw.drawLine(x+size, y, x+size/2, y+size*(sqrt(3)/2));
        gw.drawLine(x+size/2, y+size*(sqrt(3)/2), x, y);
    }
    //Recursively draws three triangles which as a total form a larger triangle.
    else {
        drawSierpinskiTriangle(gw, x, y, size/2, order-1);
        drawSierpinskiTriangle(gw, x+size/2, y, size/2, order-1);
        drawSierpinskiTriangle(gw, x+size/4, y + size*(sqrt(3)/2)/2, size/2, order-1);
    }
}


void floodFillHelper(int x, int y, int width, int height, int newColor, int oldColor);

/*floodFill replaces contiguous areas of a color with a  new color.*/
void floodFill(int x, int y, int width, int height, int color) {
    int oldColor = getPixelColor(x, y);
    int newColor = color;
    if (oldColor != newColor)
        floodFillHelper(x, y, width, height, newColor, oldColor);
}

/*floodFillHelper uses the parameters oldColor and newColor to recursively fill neighboring pixels
of oldColor with the newColor*/
void floodFillHelper(int x, int y, int width, int height, int newColor, int oldColor) {
    if (getPixelColor(x, y) == oldColor && x >= 0 && x <= width && y <= height && y >= 0) {
        setPixelColor(x, y, newColor);
        floodFillHelper(x+1, y, width, height, newColor, oldColor);
        floodFillHelper(x-1, y, width, height, newColor, oldColor);
        floodFillHelper(x, y+1, width, height, newColor, oldColor);
        floodFillHelper(x, y-1, width, height, newColor, oldColor);
    }
}
Map<string, Vector<Vector<string>>> createGrammarMap(istream& input);
string grammarRecursion(Map<string, Vector<Vector<string>>>& grammarMap, string symbol);

/*grammarGenerate returns a Vector of randomly generated strings that follow a certain grammar.*/
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Map<string, Vector<Vector<string>>> grammarMap = createGrammarMap(input);
    Vector<string> v;
    for (int i = 0; i < times; i++) {
        string grammar = trim(grammarRecursion(grammarMap, symbol));
        v.add(grammar);
    }
    return v;
}

/*grammarRecursion creates a random string from the grammar rules saved in grammarMap. */
string grammarRecursion(Map<string, Vector<Vector<string>>>& grammarMap, string symbol) {
    string sentence;
    //error exception
    if (symbol == "") {
        throw("Error: Symbol parameter is empty");
    }
    //base case; symbol is a terminal value.
    if (!grammarMap.containsKey(symbol)) {
        sentence += " " + symbol;
    }
    //Recursively + randomly chooses an expansion rule for the non-terminal symbol.
    else {
        Vector<Vector<string>> bigVector = grammarMap.get(symbol);
        Vector<string> smallVector = bigVector[randomInteger(0, bigVector.size() - 1)];
        for(int i = 0; i < smallVector.size(); i++){
            sentence += grammarRecursion(grammarMap, smallVector[i]);
        }
    }
    return sentence;
}

/*Creates and returns a Map of grammar rules from a file; keys represent non-terminal symbols.*/
Map<string, Vector<Vector<string>>> createGrammarMap(istream& input) {
    Map<string, Vector<Vector<string>>> grammarMap;
    string line;
    while (getline(input, line)) {
        Vector<string> v = stringSplit(line, "::=");
        string key = v[0];
        if (grammarMap.containsKey(key)) {
            //error exception
            throw("Grammar contains more than one line for the same non-terminal.");
        }
        else {
            //stores key
            grammarMap[key];
        }
        string rules = trim(v[1]);
        Vector<string> ruleSplitStrings = stringSplit(rules, "|");
        int ruleSplitLength = ruleSplitStrings.size();
        Vector<Vector<string>> ruleSplitVectors;
        for (int i = 0; i < ruleSplitLength; i++) {
            ruleSplitVectors.add(stringSplit(ruleSplitStrings[i], " "));
        }
        grammarMap[key] = ruleSplitVectors;
    }
    return grammarMap;
}
