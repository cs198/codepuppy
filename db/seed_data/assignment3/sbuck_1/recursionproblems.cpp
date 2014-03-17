// This is the .cpp file I edited and turned in. It uses recursion
// to accomplish every problem as described in the directions given
// to us.

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
#include "filelib.h"
using namespace std;

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}
/**
 * @brief convertStringToIntegerHelper
 * @param converted
 * @param unused
 * convertStringToIntegerHelper() takes in a string and adds the
 * first char of the string to the end of the int and then calls itself
 * after it removes the first char.
 */
void convertStringToIntegerHelper(int& converted, string& unused){
    if(unused.length() != 0){
        if(unused[0] != '-'){
            if(converted != -1){
                converted = (converted * 10);
                converted += (unused[0] - '0');
                unused.erase(0, 1);
                convertStringToIntegerHelper(converted, unused);
            }else {
                converted = (converted * (unused[0] - '0'));
                unused.erase(0, 1);
                convertStringToIntegerHelper(converted, unused);
            }
        }else{
            if(unused.length() == 1) {
                converted = (converted * (-1));
            }else {
                unused.erase(0, 1);
                unused.append("-");
                convertStringToIntegerHelper(converted, unused);
            }
        }
    }
}
/**
 * @brief convertStringToInteger
 * @param exp
 * @return
 * Takes in a string and uses the helper function convertStringToIntegerHelper()
 * to return an int.
 */
int convertStringToInteger(string exp) {
    int converted;
    string unused = exp;
    convertStringToIntegerHelper(converted, unused);
    return converted;
}
/**
 * @brief isBalanced
 * @param exp
 * @return
 * isBalanced() takes in a string and always checks
 * the first char of the string. It then checks if the
 * string contains the closing version of the punctuation.
 * If it does then it removes both chars and calls itself.
 */
bool isBalanced(string exp) {
    if(exp.length() % 2 == 1){
        return false;
    }else if(exp.length() == 0){
        return true;
    }else{
        if(exp[0] == '{'){
            int something = exp.find("}");
            if(exp[something] == '}') {
                exp.erase(something, 1);
                exp.erase(0, 1);
                if(isBalanced(exp)){
                    return true;
                }else return false;
            }else return false;
        }else if(exp[0] == '['){
                int something = exp.find("]");
                if(exp[something] == ']') {
                    exp.erase(something, 1);
                    exp.erase(0, 1);
                    if(isBalanced(exp)){
                        return true;
                    }else return false;
                }else return false;
            }else if(exp[0] == '('){
                int something = exp.find(")");
                if(exp[something] == ')') {
                    exp.erase(something, 1);
                    exp.erase(0, 1);
                    if(isBalanced(exp)){
                        return true;
                    }else return false;
                }else return false;
            }else return false;
    }
    return false;
}

/**
 * @brief drawSierpinskiTriangle
 * @param gw
 * @param x
 * @param y
 * @param size
 * @param order
 * drawSierpinskiTriangle() uses recursion to draw the three traingles inside
 * of each big traingle for each order. If the order != 0 then it will call itelf
 * again.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, x + (size / 2), y +(size*sqrt(3)/2));
        gw.drawLine(x + (size / 2), y +(size*sqrt(3)/2), x + size, y);
   if(order - 1 != 0){
        drawSierpinskiTriangle(gw, x, y, size / 2, order - 1);
        drawSierpinskiTriangle(gw, x + size / 2, y, size / 2, order - 1);
        drawSierpinskiTriangle(gw, x + size / 4, y + (size*sqrt(3)/2) / 2, size / 2, order - 1);
    }
}

/**
 * @brief floodFillHelper
 * @param x
 * @param y
 * @param width
 * @param height
 * @param color
 * @param oldColor
 * floodFillHelper() checks the four pixels around it to see if they are the new Color,
 * if they are it calls itself and the checks the surrounding pixels again, changing them when they
 * are the oldColor to the new color.
 */
void floodFillHelper(int x, int y, int width, int height, int color, int oldColor){
    if(oldColor != COLOR_WHITE){
        if(getPixelColor(x, y) == oldColor) {
            setPixelColor(x, y, color);
            if(getPixelColor(x - 1, y) != color && x - 1 > 0){
                floodFillHelper(x - 1, y, width, height, color, oldColor);
            }
            if(getPixelColor(x + 1, y) != color && x + 1 < width){
                floodFillHelper(x + 1, y, width, height, color, oldColor);
            }
            if(getPixelColor(x, y - 1) != color && y - 1 > 0){
                floodFillHelper(x, y - 1, width, height, color, oldColor);
            }
            if(getPixelColor(x, y + 1) != color && y + 1 < height){
                floodFillHelper(x, y + 1, width, height, color, oldColor);
            }
        }
    }
}
/**
 * @brief floodFill
 * @param x
 * @param y
 * @param width
 * @param height
 * @param color
 * floodFill() uses a helper method to fill all of the touching countiguous
 * are of the same color where clicked on the graphics window.
 */
void floodFill(int x, int y, int width, int height, int color) {
    int oldColor = getPixelColor(x,y);
    floodFillHelper(x, y, width, height, color, oldColor);
}

/**
 * @brief fileReader
 * @param input
 * @param wordMap
 * fileReader reads the istream into a map of strings to vectors of vectors
 * of strings.
 */
void fileReader(istream& input, Map<string, Vector<Vector<string>>>& wordMap) {
    string line;
    Vector<Vector<string>> values;
    while(getline(input, line)){
        values.clear();
        Vector<string> keyAndValues = stringSplit(line, "::=");
        Vector<string> valueVector = stringSplit(keyAndValues[1], "|");
        for(int i = 0; i < valueVector.size(); i++){
            values.add(stringSplit(valueVector[i], " "));
        }
        if(wordMap.containsKey(keyAndValues[0])) throw "Error: Multiple rules for a symbol";
        if(keyAndValues[0] == "") throw "Error: symbol parameter is blank";
        wordMap.put(keyAndValues[0], values);
    }
}

/**
 * @brief secondGrammarGenerateHelper
 * @param wordMap
 * @param symbol
 * @param sentence
 * secondGrammarGenerateHelper() does all of the work in this recursion problem.
 * It takes in the map created from the file and dives into the problem. It checks if
 * the map contains the symbol passed in. If it does it runs itself on that key, else it
 * adds the symbol to the sentence because it has found a word.
 */
void secondGrammarGenerateHelper(Map<string, Vector<Vector<string>>>& wordMap,
                                 string symbol, string& sentence){
    if(wordMap.containsKey(symbol)){
        Vector<Vector<string>> nextVector = wordMap.get(symbol);
        Vector<string> nextSymbol = nextVector.get(randomInteger(0, nextVector.size() - 1));
        for(string line: nextSymbol){
            secondGrammarGenerateHelper(wordMap, line, sentence);
        }
    }else {
        sentence = sentence + symbol + " ";
    }
}

/**
 * @brief grammarGenerateHelper
 * @param wordMap
 * @param symbol
 * @param v
 * @param times
 * grammarGenerateHelper() uses recursion to add each sentence to a vector.
 * This same task could be accoplished using a for loop. However recurison proved
 * effective.
 */
void grammarGenerateHelper(Map<string, Vector<Vector<string>>>& wordMap,
                                     string symbol, Vector<string>& v, int times) {
    string sentence;
        secondGrammarGenerateHelper(wordMap, symbol, sentence);
        v.add(sentence);
        if(times - 1 != 0) grammarGenerateHelper(wordMap, symbol, v, times - 1);
}

/**
 * @brief grammarGenerate
 * @param input
 * @param symbol
 * @param times
 * @return
 * grammarGenerate() takes in an istream, a string, and an int and it
 * returns a vector with the int number of gramatically correct phrases
 * or sentences depending on what the string passed in was.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Map<string, Vector<Vector<string>>> wordMap;
    fileReader(input, wordMap);
    Vector<string> v;
    grammarGenerateHelper(wordMap, symbol, v, times);
    return v;
}
