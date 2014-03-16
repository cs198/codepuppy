// Min Cheol Kim
//Section Leader Jimmy Lee Wed 5:15 Section
//Recursion Problems


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
void grammerGenerateHelper(HashMap<string, Vector<string> > &rules, string symbol, string &partial);

int countKarelPaths(int street, int avenue) {
    return 0;
}

/* This function takes in a string representing a valid integer and returns an integer that was found in the string.
 * The code calls on itself with one less character in the string each time it recurses. */
int convertStringToInteger(string exp) {
    if (exp.length() == 1)
        return exp[0] - '0';
    if (exp[0] == '-') {
        exp.erase(0,1);
        cout << exp << endl;
        int first = exp[0] - '0';
        return (-1)*( first*pow(10, (exp.length()-1)) + convertStringToInteger(exp.substr(1)) );
    }
    else {
        int first = exp[0] - '0';
        return first*pow(10, (exp.length()-1)) + convertStringToInteger(exp.substr(1));
    }
}

/* This function takes in a string of different types of brackets and checks if the set of brackets is valid or not. If
 * there is an imbalance, the function returns false; if the set is valid, than the function returns true. This function
 * works by finding a valid set of brackets, and calling itself again with those brackets deleted. */
bool isBalanced(string exp) {
    // TODO: write this function
    if (exp.size() == 0)
        return true;
    int normalBrac = exp.find("()");
    int squareBrac = exp.find("[]");
    int squiglyBrac = exp.find("{}");
    if (normalBrac != -1) {
        exp.erase(normalBrac, 2);
        return isBalanced(exp);
    }
    else if (squareBrac != -1) {
        exp.erase(squareBrac, 2);
        return isBalanced(exp);
    }
    else if (squiglyBrac != -1) {
        exp.erase(squiglyBrac, 2);
        return isBalanced(exp);
    }
    else
        return false;
}
/* This function takes in a GWindow, a starting position (top left of triangle), size, and the order of the Sierpinski
 * triangle to be drawn. This function works by drawing three triangles and recursing with the next set of points and sizes. */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order <= 0)
        return;
    gw.drawLine(x,y,x+size,y);
    gw.drawLine(x,y,x+(size/2), y +(sqrt(3)*size/2) );
    gw.drawLine(x+size,y,x+(size/2), y +(sqrt(3)*size/2) );
    drawSierpinskiTriangle(gw, x, y, size/2, order-1);
    drawSierpinskiTriangle(gw, x+size/2, y, size/2, order-1);
    drawSierpinskiTriangle(gw, x+size/4, y +(sqrt(3)*size/4), size/2, order-1);

}

/* This function takes in a position in the grid, width/height of the grid, and the color to replace the originial
 * color with. This function then recurses and replaces the pixel color until a different background color is reached
 * or the pixel has run off the grid. */
void floodFill(int x, int y, int width, int height, int color) {
    int currentColor = getPixelColor(x,y);
    setPixelColor(x,y,color);
    if (x > 0 && getPixelColor(x-1,y) == currentColor)
        floodFill(x-1, y, width, height, color);
    if (x < width-1 && getPixelColor(x+1,y) == currentColor)
        floodFill(x+1, y, width, height, color);
    if (y > 0 && getPixelColor(x,y-1) == currentColor)
        floodFill(x, y-1, width, height, color);
    if (y < height-1 && getPixelColor(x,y+1) == currentColor)
        floodFill(x, y+1, width, height, color);
    return;

}

/* This function takes in a istream for reading the grammar file, a symbol and the number of times to generate the random
 * words. This function stores the grammar file into a map than calls the recursive helpder function in order to actually
 * generate the random grammatically correct sentences. */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    HashMap<string, Vector<string>> rules;
    string line;
    while (getline(input, line)) {
        int pos = line.find("::=");
        string key = line.substr(0, pos);
        string valueString = line.substr(pos+3);
        Vector<string> value = stringSplit(valueString, "|");
        rules.put(key, value);
    }
    Vector<string> randomGrammars;
    for (int n = 0; n < times; n++) {
        string s;
        grammerGenerateHelper(rules, symbol, s);
        randomGrammars.add(s);
    }
    return randomGrammars;
}

/* This function recurses until the current symbol is a terminal. If the current symbol is not terminal, then the function
 * calls on itself to acess the components that make up the non-terminal. This way, the recursive function generates
 * random grammatically correct sentences */
void grammerGenerateHelper(HashMap<string, Vector<string>>& rules, string symbol, string& partial) {
    Vector<string> ruleSet = rules.get(symbol);
    int random = randomInteger(0, ruleSet.size()-1);
    string randomSymbol = ruleSet[random];
    Vector<string> symbolSet = stringSplit(randomSymbol, " ");
    for (string element : symbolSet) {
        if (rules.containsKey(element) ) {
            grammerGenerateHelper(rules, element, partial);
        }
        else {
        partial = partial + " " + element;
        return;
        }
    }
}
