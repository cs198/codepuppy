/*
 * Emma Daugherty; CS106B; Section Leader: Jimmy Lee
 *
 * This program runs the following recursively solved problems:
 * 1)Convert String to Integer
   2)Balancing Parentheses
   3)Sierpinski Triangle
   4)Flood Fill
   5)Grammar Solver

   It does not run Karel goes home

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
//Helper Methods
void floodFillHelper(int x, int y, int width, int height, int colorChange, int colorWas);
void readFile(istream& input, HashMap<string, Vector<Vector<string> > >& map);
string generateText(HashMap<string, Vector<Vector<string> > >& map, string symbol);

int countKarelPaths(int street, int avenue) {
    return 0;
}

/*
 * This method takes a string of numbers and converts it to an integer
 */
int convertStringToInteger(string exp) {
    if (exp.length() == 1) {
        if (exp[0] == '-') {
            return 0;
        }
        else {
            return exp[0] - '0';
        }
    }
    else {
        if (exp[0] == '-') {
            return (exp[exp.length() - 1] - '0') * -1 + 10 * convertStringToInteger(exp.substr(0, exp.length() - 1));
        }
        else {
           return (exp[exp.length() - 1] - '0') + 10 * convertStringToInteger(exp.substr(0, exp.length() - 1));
        }

    }
}

/*
 * This method takes in a string consisting only of {}, () and/or [] and returns whether or not the string is balanced
 * (every parenthesis comes in an open/ closed pair)
 */
bool isBalanced(string exp) {
   if (exp == "") {
       return true;
   }
   else {
       int index;
       if(exp.find("()") != -1) {
           index = exp.find("()");
           exp.erase(index, 2);
       } else if (exp.find("[]") != -1) {
           index = exp.find("[]");
           exp.erase(index, 2);
       } else if (exp.find("{}") != -1) {
           index = exp.find("{}");
           exp.erase(index, 2);
       } else {
           return false;
       }

       return isBalanced(exp);
   }
}

/*
 * This method takes in a starting (x,y) coordinate, a size, an order and a graphics window and draws in that graphics
 * window a Sierpinski triangle. The order is the number of layers of triangles and the (x,y) point refers to the
 * upper lefthand point of the biggest triangle.
 */
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order >= 0) {
        double height = sqrt(pow(size, 2) - pow(size / 2, 2));
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, x + (size / 2), y + height);
        gw.drawLine(x + size, y, x + (size / 2), y + height);

        drawSierpinskiTriangle(gw, x, y, size / 2, order - 1);
        drawSierpinskiTriangle(gw, x + (size / 2), y, size / 2, order - 1);
        double h = sqrt(pow(size / 2, 2) - pow(size / 4, 2));
        drawSierpinskiTriangle(gw, x + (size / 4), y + h, size / 2, order - 1);
    }

}
/*
 * This method changes all the pixels of the same color as (x,y) bounded by the edge of the window or boxes of different
 * colors to a color chosen by the user.
 */
void floodFill(int x, int y, int width, int height, int color) {
    int colorWas = getPixelColor(x, y);
    floodFillHelper(x, y, width, height, color, colorWas);
}

/*
 * This method actually implements the full functionality of Flood Fill by changing the color of an area.
 */
void floodFillHelper(int x, int y, int width, int height, int colorChange, int colorWas) {
    //top
    if (y - 1 >= 0 && getPixelColor(x, y - 1) != colorChange && getPixelColor(x, y - 1) == colorWas) {
        setPixelColor(x, y - 1, colorChange);
        floodFillHelper(x, y - 1, width, height, colorChange, colorWas);
    }

    //bottom
    if (y + 1 < height && getPixelColor(x, y + 1) != colorChange && getPixelColor(x, y + 1) == colorWas) {
        setPixelColor(x, y + 1, colorChange);
        floodFillHelper(x, y + 1, width, height, colorChange, colorWas);
    }

    //right
    if (x + 1 < width && getPixelColor(x + 1, y) != colorChange && getPixelColor(x + 1, y) == colorWas) {
        setPixelColor(x + 1, y, colorChange);
        floodFillHelper(x + 1, y, width, height, colorChange, colorWas);
    }

    //left
    if (x - 1 >= 0 && getPixelColor(x - 1, y) != colorChange && getPixelColor(x - 1, y) == colorWas) {
        setPixelColor(x - 1, y, colorChange);
        floodFillHelper(x - 1, y, width, height, colorChange, colorWas);
    }
}

/*
 * This method generates random text based on a grammar read in from a file
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if (symbol == "") {
        throw new std::string("");
    }
    HashMap<string, Vector<Vector<string> > > map;
    readFile(input, map);
    Vector<string> v;
    for (int i = 0; i < times; i++) {
        string answer = generateText(map, symbol);
        v.add(answer);
    }
    return v;
}

/*
 * This method generates random text of the type of symbol inputted by the user
 */
string generateText(HashMap<string, Vector<Vector<string> > >& map, string symbol) {
    if (!map.containsKey(symbol)) {
        return symbol;
    }
    else {
        Vector<Vector<string> > values = map.get(symbol);
        int rand = randomInteger(0, values.size() - 1);
        Vector<string> use = values.get(rand);
        string result;
        for (int i = 0; i < use.size(); i++) {
            result += " " + trim(generateText(map, use.get(i)));
        }
        return result;
    }
}

/*
 * This method reads in a file with the specific grammar setup:
 * non-terminal::= rule|rule|...|rule
 * and stores that data for use in randomly generating text of a certain type based on that grammar
 */
void readFile(istream& input, HashMap<string, Vector<Vector<string> > >& map) {
    string line;
    while (getline(input, line)) {
        int index = line.find("::=");
        string key = line.substr(0, index);
        if (map.containsKey(key)) {
            throw new std::string("");
        }
        string rest = line.substr(index + 3);
        Vector<string> value = stringSplit(rest, "|");
        Vector<Vector<string> > value2;
        for (int i = 0; i < value.size(); i++) {
            value2.add(stringSplit(value.get(i), " "));
        }
        map.put(key, value2);
    }
}














