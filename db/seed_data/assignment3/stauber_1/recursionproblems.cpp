//File: recursionproblems.cpp
//Author: Mark Stauber
//Date: 2/4/2014
//Description: Contains the functions to execute the recursive
//programs that are called by recursionmain.cpp.
//The file mygrammar.txt contains a Commandment Generator.
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

//Helper function for grammarGenerate that parses a grammar in input
//and loads it into a Map where a key is a non-terminal symbol
//and the entries contain a vector of the corresponding rules.
//The function also checks for errors in the grammar file.
void loadGrammar(istream& input, Map<string, Vector<string>>& grammar){
    string line;

    while(getline(input, line)){
        //vLine is a vector containing symbol on left of "::=" and rules on right
        Vector<string> vLine = stringSplit(line, "::=");

        //Check for errors with symbol on left of "::="
        if(vLine[0].length() == 0) throw "Empty Symbol.";
        if(grammar.containsKey(vLine[0])) throw "Repeat Symbol.";

        //Add rules to the symbol.
        grammar[vLine[0]] = stringSplit(vLine[1], "|");
    }
}

//Helper function for grammarGenerate that recursively produces
//a string corresponding to the non-terminal symbol. It chooses
//the contents of the string by randomly selecting rules from symbol.
string generateElement(Map<string, Vector<string>>& grammar, string symbol, string ans){
    //Select a random rule from symbol.
    int randEntry = randomInteger(0, grammar[symbol].size() - 1);
    string entry = grammar[symbol][randEntry];
    Vector<string> vEntry = stringSplit(entry," ");

    //If i is non-terminal then recurse, otherwise add to temp answer string.
    string temp;
    for(auto i : vEntry){
        if(grammar.containsKey(i)){
            temp += generateElement(grammar, i, ans);
        }else{
            temp += i + " ";
        }
    }
    return ans + temp;
}

//Counts possible ways for Karel to get back to 1,1 using recursion.
int countKarelPaths(int street, int avenue) {
    if(street < 1 || avenue < 1)
        throw "ERROR: Street or Avenue less than 1.";
    switch(street){
    case 1:
            if(avenue == 1) return 1;
            else return countKarelPaths(street, avenue - 1);
    default:
        if(avenue == 1) return countKarelPaths(street - 1, avenue);
        else return countKarelPaths(street, avenue - 1) +
                countKarelPaths(street - 1, avenue);
    }
}

//Converts a string representing a positive or negative integer
//into a integer of type int using recursion.
int convertStringToInteger(string exp) {
    if(exp.empty()) return 0;
    else if(exp[0] == '-')
        return -convertStringToInteger(exp.substr(1));
    else return exp[exp.length() - 1] - '0' + 10 *
            convertStringToInteger(exp.substr(0, exp.length() - 1));
}

//Recursively checks if string exp is balanced using the rules:
//1. An empty string is balanced.
//2. The string contains the substring (), [], or {} and if that
//substring is removed the remaining string is balanced.
bool isBalanced(string exp) {
    //Check for simple cases: string has odd # entries which must be unbalanced
    //Or string is empty which is balanced
    if(exp.length() % 2 != 0) return false;
    if(exp.empty()) return true;

    //Search for pair, remove it, and recurse.
    int pos = exp.find("()");
    if(pos <= exp.length() ||
            (pos =  exp.find("[]")) <= exp.length() ||
            (pos = exp.find("{}")) <= exp.length()){
        string expNew = exp.erase(pos, 2);
        return isBalanced(exp);
    }else return false;
}

//Draws a Sierpinski Triangle of specified order and size using recursion.
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if(order == 0) return;
    else if(order == 1){
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, x + size / 2, y + sqrt(3) * size / 2);
        gw.drawLine(x + size, y,  x + size / 2, y + sqrt(3) * size / 2);
    }else{
        drawSierpinskiTriangle(gw, x, y, size / 2, order - 1);
        drawSierpinskiTriangle(gw, x + size / 2, y, size / 2, order - 1);
        drawSierpinskiTriangle(gw, x + size / 4, y + sqrt(3) * size / 4, size / 2, order - 1);
    }
}

//Fills the shape at x, y with the specified color by recursively
//searching for the surrounding pixels of thisColor.
void floodFill(int x, int y, int width, int height, int color) {
    int thisColor = getPixelColor(x, y);
    if(thisColor != color){
        setPixelColor(x, y, color);

        if((x < width - 1) && (getPixelColor(x + 1, y) == thisColor))
        floodFill(x + 1, y, width, height, color);

        if((x > 0) && (getPixelColor(x - 1, y) == thisColor))
        floodFill(x - 1, y, width, height, color);

        if((y < 362 - 1) && (getPixelColor(x, y + 1) == thisColor))
        floodFill(x, y + 1, width, height, color);

        if((y > 0) && (getPixelColor(x, y - 1) == thisColor))
        floodFill(x, y - 1, width, height, color);
    }
}

//Function that generates a grammar from input and returns a vector
//containing 'times' instances of 'symbol'.
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    // TODO: write this function
    Vector<string> v(times);
    Map<string, Vector<string>> grammar;
    loadGrammar(input, grammar);
    for(int i = 0; i < times; i++){
        v[i] = generateElement(grammar, symbol, "");
    }
    return v;
}
