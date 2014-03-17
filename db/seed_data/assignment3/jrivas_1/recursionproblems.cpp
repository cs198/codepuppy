// This is a number of recursive problems done for

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include "gwindow.h"
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "filelib.h"
#include "recursionproblems.h"
using namespace std;

//declaring functions
bool pixelIsGood(int x, int y, int width, int height, int color);
HashMap<string, Vector<Vector<string>>> createMap(istream& input);
string recursiveMaker(HashMap<string, Vector<Vector<string>>> map, string symbol);


int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

/*
 * The recursive function convertStringToInteger. This function
 * takes in an integer and if the string is empty, it returns 0;
 * if the string is not empty, the program converts the last
 * character in the string to a number, because that will be the
 * number in the "ones" place. It then removes that character and
 * returns that number plus the function times 10. This continues
 * until the string is zero. If there is a negative sign in the
 * beginning, it removes it and makes the number negative.
 */
int convertStringToInteger(string exp) {
    if (exp.size() == 0) {
        return 0;
    } else if (exp.at(0) == '-'){
        return -convertStringToInteger(exp.substr(1));
    } else {
        int temp = exp.at(exp.length() - 1) - '0';
        cout << exp.substr(1) << endl;
        return temp + convertStringToInteger(exp.substr(0, exp.length() - 1))*10;
    }
    return 0;
}

/*
 * This function checks to see if parentheses are balanced. It takes
 * the designated string of parentheses and removes the innermost balanced
 * parentheses, then passes the string again until it either reaches zero or
 * it can't remove anything more.
 */

bool isBalanced(string exp) {
    if (exp.length() == 0) return true;
    string string1 = "()";
    string string2 = "[]";
    string string3 = "{}";
    if (exp.find(string1) != string::npos || exp.find(string2) != string::npos || exp.find(string3) != string::npos) {
        if (exp.find(string1) != string::npos) {
            return isBalanced(exp.erase(exp.find(string1),string1.length()));
        } else if (exp.find(string2) != string::npos) {
            return isBalanced(exp.erase(exp.find(string2),string2.length()));
         }else {
            return isBalanced(exp.erase(exp.find(string3),string3.length()));
        }
    }
    return false;
}

/*
 * This function draws a triangle of a certain order. If the order is zero,
 * the function returns zero; if the order is one, it draws the triangle
 * at the designated spot; otherwise, it divides according to math rules and
 * draws three triangles, calling the function again.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order <= 0)
        return;
    else if (order == 1) {
        gw.drawLine(x, y, x+size, y);
        gw.drawLine(x, y, x + size/2, y  + sqrt(3)/2 * size);
        gw.drawLine(x+size, y, x + size/2, y + sqrt(3)/2 * size);
    }
    drawSierpinskiTriangle(gw, x + size/2, y, size/2, order - 1);
    drawSierpinskiTriangle(gw, x, y, size/2, order - 1);
    drawSierpinskiTriangle(gw, x + size/4, y  + (sqrt(3)/2 * size)/2, size/2, order - 1);
}

/*
 * FloodFill takes in a pixel and saves its initial color; it then changes
 * the color of that pixel, then checks the surrounding four pixels to see if
 * they are the same initial color as the original. If they are, then it calls
 * FloodFill on that pixel as well.
 */
void floodFill(int x, int y, int width, int height, int color) {
    int color2 = getPixelColor(x,y);
    setPixelColor(x, y, color);
    if (pixelIsGood(x + 1, y, width, height, color2))
        floodFill(x + 1, y, width, height, color);
    if (pixelIsGood(x, y + 1, width, height, color2))
        floodFill(x, y + 1, width, height, color);
    if (pixelIsGood(x, y - 1, width, height, color2))
        floodFill(x, y - 1, width, height, color);
    if (pixelIsGood(x - 1, y, width, height, color2))
        floodFill(x - 1, y, width, height, color);
}

/*
 * PixelIsGood checks to see if the pixel examined is the
 * correct color and has correct x and y coordinates (in bounds).
 */
bool pixelIsGood(int x, int y, int width, int height, int color) {
    if (x < 0 || y < 0 || (x > width - 1) || (y > height -1))
        return false;
    int color3 = getPixelColor(x,y);
    if (color3 != color) return false;
    return true;
}

/*
 * GrammarGenerate first creates a map based on the input file,and then
 * calls recursiveMaker for "times" number of times, sending the map and
 * the symbol, to make a string that will be added to the vector. Once
 * this process is done, it will return the vector of strings.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Vector<string> v;
    if (symbol == "") return v;
    HashMap<string, Vector<Vector<string>>> map = createMap(input);
    for (int i = 0; i < times; i++) {
        v.add(recursiveMaker(map, symbol));
    }
    return v;
}

/*
 * This function creates the map where the key is a symbol, and the
 * values are the different ways to create that symbol. It reads
 * the input file line by line, and splits it up using the appropriate
 * designations. It then stores all the things and returns the completed
 * map.
 */

HashMap<string, Vector<Vector<string>>> createMap(istream& input) {
    Vector<string> v;
    Vector<string> v2;
    Vector<string> temp_v;
    HashMap<string, Vector<Vector<string>>> map;
    Vector<Vector<string>> vectorsquared;
    string s = "";
    while(true) {
        bool success = getline(input, s);
        if (!success) break;
        v = stringSplit(s, "::=");
        string key = v.get(0);
        v2 = stringSplit(v.get(1), "|");
        for (int i = 0; i < v2.size(); i++) {
            temp_v = stringSplit(v2.get(i), " ");
            vectorsquared.add(temp_v);
            temp_v.clear();
        }
        if (map.containsKey(key)) cout << "exception" << endl;
        map.put(key,vectorsquared);
        v.clear();
        v2.clear();
        vectorsquared.clear();
    } return map;
}

/*
 * This function recursively creates the string for the symbol. If the symbol is a
 * terminal, then it calls the function again, adding the parts of the grammar
 * to the string until it reaches a terminal; at the end, it then returns the
 * completed string.
 */

string recursiveMaker(HashMap<string, Vector<Vector<string>>> map, string symbol) {
    Vector<string> v;
    Vector<Vector<string>> v2;
    if (symbol.size() = 0) {
        return "exception";
    }
    if (!map.containsKey(symbol)) return symbol;
    v2 = map.get(symbol);
    int random = randomInteger(0,v2.size() - 1);
    v = v2.get(random);
    string output = "";
    for (int i = 0; i < v.size(); i++) {
        string word = v.get(i);
        if (map.containsKey(word)) {
            output += trim(recursiveMaker(map,word));
            output += " ";
        }
        else {
            return trim(word);
        }
    } return output;
}
