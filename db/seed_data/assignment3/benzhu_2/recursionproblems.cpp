//Name: Benzheng Zhu
//CS 106B Assignment 3
//Citation: lecture slides
//This program consists 6 recursion practices
//I skipped the warm-up practice: countKarelPaths

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
#include "error.h"
using namespace std;

int countKarelPaths(int street, int avenue) {
    return 0;
}

int convertStringToInteger(string exp) {
    if (exp[0] == '-') {
        return -1*convertStringToInteger(exp.substr(1));
    } else if (exp.size()== 1) {
        return exp[0]-'0';
    } else {
        int i = exp[0]-'0';
        return i*pow(10,(exp.size()-1))+ convertStringToInteger(exp.substr(1));
    }
}

bool isBalanced(string exp) {
    if (exp == "") {
        return true;
    } else if (exp[exp.rfind('(')+1] == ')') { //if the last ( is followed by a )
        exp.erase(exp.rfind('('),2);  //remove this pair of ()
        return isBalanced(exp);
    } else if (exp[exp.rfind('[')+1] == ']') { //if the last [ is followed by a ]
        exp.erase(exp.rfind('['),2); //remove this pair of []
        return isBalanced(exp);
    } else if (exp[exp.rfind('{')+1] == '}') { //if the last { is followed by a }
        exp.erase(exp.rfind('{'),2); //remove this pair of {}
        return isBalanced(exp);
    } else {
        return false;
    }
}

void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order <= 0) {
        return;
    } else {
        //draw a downward-pointing equilateral triangle with each side being length size
        gw.drawLine(x, y, x+size, y);
        gw.drawLine(x+size/2, y+size*sqrt(3)/2, x+size, y);
        gw.drawLine(x+size/2, y+size*sqrt(3)/2, x, y);

        //set 3 new sets of starting points (x,y), reduce size by half, and reduce order by 1
        drawSierpinskiTriangle(gw, x, y, size/2, order-1);
        drawSierpinskiTriangle(gw, x+size/4, y+size*sqrt(3)/4, size/2, order-1);
        drawSierpinskiTriangle(gw, x+size/2, y, size/2, order-1);
    }
}

void floodFill(int x, int y, int width, int height, int color) {
    //base case: index goes out of the boundaries
    if (x<0 || y<0 || x>=width || y>=height) {
        return;
    } else {
        int color2 = getPixelColor(x, y); //the original color of the pixel
        setPixelColor(x, y, color);

        //in each of the four directions
        //check if the neighboring pixel is of the same color as the original one
        if (getPixelColor(max(x-1,0), y) == color2) {
            floodFill(x-1, y, width, height, color);
        }
        if (getPixelColor(min(width-1,x+1), y)==color2) {
            floodFill(x+1, y, width, height, color);
        }
        if (getPixelColor(x, max(y-1,0))==color2) {
            floodFill(x, y-1, width, height, color);
        }
        if (getPixelColor(x, min(y+1,height-1))==color2) {
            floodFill(x, y+1, width, height, color);
        }
        return;
    }
}

void readFile (istream& input, HashMap<string,Vector<Vector<string>>>& map) {
    //this fuction takes a file and a map as inputs
    //it reads the file and stores the grammar rules into the map

    string line;
    while (getline(input, line)) {
        int j = line.find("::=");
        Vector<Vector<string>> values;

        //split the rules by "|"
        Vector<string> s = stringSplit(line.substr(j+3),"|");

        //split each rule by white space
        //so that each rule is a vector of strings
        //store these rule vectors into a vector called "values"
        for (int i = 0; i < s.size(); i++) {
            Vector<string> p = stringSplit(s[i]," ");
            values += p;
        }

        if (map.containsKey(line.substr(0,j))) {
            throw "the grammar contains more than one line for the same non-terminal.";
        } else {
            map[line.substr(0,j)] = values; //create a pair of key and value
        }
    }
}

void generateElement(string symbol, string& y, HashMap<string,Vector<Vector<string>>>& map) {
    //this fuction takes a symbol, a string y and the map as inputs
    //it generates a phrase based on the symbol and the map, and stores it in the string y

    if (symbol == "") {
        throw "the symbol cannot be empty.";
    }

    //base case: if the symbol is  a terminal (i.e. not a key of the map)
    if (!map.containsKey(symbol)) {
        y = y + " " + symbol;
        return;
    } else {
        //randomly choose one of the rules corresponding to this non-terminal
        int m = randomInteger(0,map[symbol].size()-1);
        Vector<string> rule = map[symbol][m];

        //use each element in this rule as the new symbol and run the recursion
        for (int i=0; i<rule.size(); i++) {
            generateElement(rule[i],y, map);
        }
    }
}

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    HashMap<string,Vector<Vector<string>>> map;
    readFile(input,map);

    Vector<string> result;
    for (int i=0; i < times; i++) {
        string y = "";
        generateElement(symbol, y, map);
        y = trim(y); //remove unwanted spaces around edges
        result.add(y);
    }
    return result;
}
