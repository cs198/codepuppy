// Xinsheng Chu

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

int countKarelPaths(int street, int avenue) {
    return 0;
}

/*
 * This function converts a string to an integer in a recursive way.
 * Two base cases are if string is blank, and only character remains in the string.
 * In recursive case, the last number is convered to an integer and taken from the string.
 * Rest of the string then go into recursion.
 */
int convertStringToInteger(string exp) {
    int integer = 0;
    if (exp == "") integer = 0;
    else if (exp.size() == 1) {
        integer = exp[0] - '0';
    } else {
        int lastNumber = exp[exp.size() - 1] - '0';
        string restString = exp.substr(0,exp.size() - 1);
        if (exp[0] == '-') {
            restString = restString.substr(1, exp.size() -1);
            integer = - (convertStringToInteger(restString) * 10 + lastNumber);
        } else {
            integer = convertStringToInteger(restString) * 10 + lastNumber;
        }
    }
    return integer;
}

/*
 * This function exams if a string of "() {} []" is balanced in a recursive way.
 * The base cases are if the string is empty and if no coupled "()", "{}" or "[]" is found in the string.
 * Otherwise, the "()", "{}" or "[]" pair is removed from the string, and rest of the string goes into recursion.
 */
bool isBalanced(string exp) {
    if (exp == "") return true;
    else if (exp.find("()") != string::npos || exp.find("[]") != string::npos || exp.find("{}") != string::npos) {
        if (exp.find("()") != string::npos) {
            exp.erase(exp.find("()"), 2);
        } else if (exp.find("[]") != string::npos) {
            exp.erase(exp.find("[]"), 2);
        } else if (exp.find("{}") != string::npos) {
            exp.erase(exp.find("{}"), 2);
        }
        return isBalanced(exp);
    } else {
        return false;
    }
}

/*
 * This function draws one base triangle on the console,
 * and finish rest of Sierpinski triangles in a recursive way, to the desired order.
 * Each time order is passed into the function by order - 1, until order reaches 0.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order > 0) {
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, x + size/2, y + sqrt(3)/2*size);
        gw.drawLine(x + size/2, y + sqrt(3)/2*size, x + size, y);
        drawSierpinskiTriangle(gw, x, y, size/2, order - 1);
        drawSierpinskiTriangle(gw, x + size/2, y, size/2, order - 1);
        drawSierpinskiTriangle(gw, x + size/4, y + sqrt(3)/4*size, size/2, order - 1);
    }
}

/*
 * This function fills the flood on the random generated console as required.
 * It first sets the new color of the current pixel.
 * Then it checks four surrounding pixels, whether they are within the boundry or whether they have the same color as the previous pixel from last state.
 * If yes, these surrounding pixels are passed into next resursion.
 */
void floodFillHelper(int x, int y, int w, int h, int newColor, int oldColor) {
    setPixelColor(x, y, newColor);
    if (x - 1 >= 0 && getPixelColor(x - 1, y) == oldColor) {
        floodFillHelper(x - 1, y, w, h, newColor, oldColor);
    }
    if (x + 1 < w && getPixelColor(x + 1, y) == oldColor) {
        floodFillHelper(x + 1, y, w, h, newColor, oldColor);
    }
    if (y - 1 >= 0 && getPixelColor(x, y - 1) == oldColor) {
        floodFillHelper(x, y - 1, w, h, newColor, oldColor);
    }
    if (y + 1 < h && getPixelColor(x, y + 1) == oldColor) {
        floodFillHelper(x, y + 1, w, h, newColor, oldColor);
    }
}

/*
 * This function gets the old color of the current pixel,
 * and call the helper function with the extra input of old input.
 */
void floodFill(int x, int y, int width, int height, int color) {
    int oldColor = getPixelColor(x, y);
    floodFillHelper(x, y, width, height, color, oldColor);
}

/*
 * This function reads the input stream and checks if the content is valid.
 * If one rule is not defined twice in the input file, the keys and values pairs are split,
 * and then stored int o a map, passed by reference.
 */
bool storeValidContent(istream& input, Map<string, Vector<Vector<string> > >& ruleMap) {
    string line;
    while (getline(input, line)) {
        Vector<string> splitKeys = stringSplit(trim(line), "::=");
        Vector<string> splitValues = stringSplit(splitKeys[1], "|");
        Vector<Vector<string> > splitRules;
        for (int i = 0; i < splitValues.size(); i++) {
            splitRules.add(stringSplit(splitValues[i], " "));
        }
        if (ruleMap.containsKey(splitKeys[0])) {
            return false;
            break;
        }
        ruleMap.put(splitKeys[0], splitRules);
    }
    return true;
}

/*
 * This is the helper function check weither a symbol is a rule.
 * If not, it adds the symbol to a string.
 * If yes, it keeps looking the rule in the map in a recursive way.
 */
string grammarGenerateHelper(string symbol, Map<string, Vector<Vector<string> > >& ruleMap) {
    string output;
    //cout << ruleMap.toString() << endl;
    if (!ruleMap.containsKey(symbol)) {
        output = symbol;
    } else {
        Vector<Vector<string> > ruleVector = ruleMap.get(symbol);
        //cout << ruleVector.toString() << endl;
        Vector<string> randomRule = ruleVector[randomInteger(0, ruleVector.size() - 1)];
        //cout << randomRule.toString() << endl;
        for (int i = 0; i < randomRule.size(); i++) {
            output += grammarGenerateHelper(randomRule[i], ruleMap) + " ";
        }
    }
    return trim(output);
}

/*
 * The funciton calls storeValidContent to check weither the content is valid, and checks if the input is valid.
 * If not, it throw the exception.
 * Then it calls the helper function to generate the grammar for the required times.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Map<string, Vector<Vector<string> > > ruleMap;
    bool validContent = storeValidContent(input, ruleMap);
    Vector<string> result;
    if (!validContent) {
        throw "Invalid. Symbol defined by more than one lines.";
    } else if (symbol == "") {
        throw "Invalid. No symbol input.";
    } else {
        for (int i = 0; i < times; i++) {
            //cout << grammarGenerateHelper(symbol, ruleMap) << endl;
            result.add(grammarGenerateHelper(symbol, ruleMap));
        }
    }
    return result;
}
