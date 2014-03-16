/**
 * Name: Milan Doshi
 * Assignment: recursionproblems.cpp
 * TA: Jimmy Lee
 *
 * This program gives users options on what programs they may want to run (converting a string to an integer, seeing if a chain of brackets, parentheses, and braces are balanced,
 * draws a Sierpinski triangle, be able to change the color of a specific rectangle based on a color the user chooses, and generate different output based on a grammar input file.
 * All of the problems were solved using recursion.
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
#include "error.h"

using namespace std;
void floodFill(int x, int y, int w, int h, int newColor, int oldColor);
Map<string, Vector<Vector<string> > > setGrammar(istream& input);
string grammarGenerate(istream& input, string symbol, int times, Map<string, Vector<Vector<string> > > grammar, string& line);

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

/**
 * method: convertStringToInteger()
 * return: int
 * This method takes in a string and converts it to an integer and then returns that integer.
 */
int convertStringToInteger(string exp) {
    if(exp == "") {
        return 0;
    } else if (exp.length() == 1) {
        return exp[0] - '0';
    } else if (exp[0] == '-') {
        return -1 * convertStringToInteger(exp.substr(1, exp.length() - 1));
    }
    return convertStringToInteger(exp.substr(exp.length() - 1, 1)) + convertStringToInteger(exp.substr(0, exp.length() - 1)) * 10;
}

/**
 * method: isBalanced()
 * return: bool
 * This method returns true or false based on whether or not a certain group of brackets, parentheses, or braces is balanced or not.
 */
bool isBalanced(string exp) {
    if (exp == "") {
        return true;
    } else if (exp.length() == 1) {
        return false;
    } else if (exp.find("[]") != string::npos ) {
        return isBalanced(exp.erase(exp.find("[]"),2));
    } else if (exp.find("{}") != string::npos) {
        return isBalanced(exp.erase(exp.find("{}"),2));
    } else if (exp.find("()") != string::npos) {
        return isBalanced(exp.erase(exp.find("()"),2));
    }
}

/**
 * method: drawSierpinskiTriangle()
 * return: void
 * This method draws a Sierpinski triangle based on the parameters that are passed in.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if(order == 0) return;
    else if(order == 1) {
       gw.drawLine(x,y,x+size,y);
       gw.drawLine(x,y,x+size/2,y+size/2*sqrt(3));
       gw.drawLine(x+size,y,x+size/2,y+size/2*sqrt(3));
    } else {
        drawSierpinskiTriangle(gw, x, y, size/2, order - 1);
        drawSierpinskiTriangle(gw, x+size/2, y, size/2 , order - 1);
        drawSierpinskiTriangle(gw, x+size/4, y+size/4*sqrt(3), size/2, order - 1);
    }
}

/**
 * method: floodFill(int x, int y, int width, int height, int color)
 * return: void
 * This method sets the old color based on where a users mouse currently is and then calls a method to recursively change the colors of the bordering (up, down, both sides)
 * pixels by calling a helper method.
 */
void floodFill(int x, int y, int width, int height, int color) {
   int oldColor = getPixelColor(x,y);
   floodFill(x,y,width,height,color,oldColor);
}

/**
 * method: floodFill(int x, int y, int w, int h, int newColor, int oldColor)
 * return: void
 * This method makes sure the coordinates are not out of bounds and the bordering pixel is the same color or not. If it is the same color, then the method recursively
 * checks the bordering pixels (up, down, both sides) and changes the pixel's color as necessary.
 */
void floodFill(int x, int y, int w, int h, int newColor, int oldColor) {
    if(((x < 0 && x > w) || (y < 0 && y > h)) || ( getPixelColor(x,y) != oldColor)) {
        return;
    } else {
        setPixelColor(x,y,newColor);
        floodFill(x+1, y, w, h, newColor, oldColor);
        floodFill(x-1, y, w, h, newColor, oldColor);
        floodFill(x, y-1, w, h, newColor, oldColor);
        floodFill(x, y+1, w, h, newColor, oldColor);
    }
}

/**
 * method: grammarGenerate(istream& input, string symbol, int times)
 * return: Vector<string>
 * This method ensures that the symbol is valid, makes the call to create a map, and then makes a call to a helper method for the number of times the grammar needs
 * to generate.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if(symbol == "") {
        error("The symbol is empty.");
    }
    Map<string, Vector<Vector<string> > > grammar = setGrammar(input);

    Vector<string> v;
    for (int i = 0; i < times; i++) {
        string line;
        v.add(grammarGenerate(input, symbol, times, grammar, line));
    }
    return v;
}

/**
 * method: grammarGenerate(istream& input, string symbol, int times, Map<string, Vector<Vector<string> > > grammar, string& line)
 * return: string
 * This method checks to see if a symbol is in the map, and if it is, then it selects a rule randomly, and then recursively goes through the parts of the rule to then
 * output the string when a symbol passed in as a parameter is no longer a key in the map.
 */
string grammarGenerate(istream& input, string symbol, int times, Map<string, Vector<Vector<string> > > grammar, string& line) {
    if (!grammar.containsKey(symbol)) {
        line += symbol+" ";
    } else {
        Vector<Vector<string> > allRules = grammar.get(symbol);
        Vector<string> rule = allRules.get(randomInteger(0, allRules.size() - 1));
        for(int j = 0; j < rule.size(); j++) {
            grammarGenerate(input, rule.get(j), times, grammar, line);
        }
    }
    return line;
}

/**
 * method: setGrammar(istream& input)
 * return: Map<string, Vector<Vector<string> > >
 * This method creates a map of the symbols and their potential rules based on the initial grammar file inputed by the user.
 */
Map<string, Vector<Vector<string> > > setGrammar(istream& input) {
    Map<string, Vector<Vector<string> > > grammarTemp;
    string temp;
    while(getline(input,temp)) {
        Vector<string> rule = stringSplit(temp, "::=");
        string key = rule.get(0);
        if(grammarTemp.containsKey(key)) {
            error("This key already exists in the map.");
        }
        Vector<string> ruleOptions = stringSplit(trim(rule.get(1)), "|");
        Vector<Vector<string> > ruleParts;
        for(int i = 0; i < ruleOptions.size(); i++) {
            Vector<string> oneRuleParts = stringSplit(trim(ruleOptions.get(i)), " ");
            ruleParts.add(oneRuleParts);
        }
        grammarTemp.put(key, ruleParts);
    }
        return grammarTemp;
 }
