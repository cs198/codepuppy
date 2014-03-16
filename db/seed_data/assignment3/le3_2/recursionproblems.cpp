// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

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

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

int convertStringToInteger(string exp) {
    // TODO: write this function
    int size = exp.size();
    if (exp[0] != '-'){ // positive num
        // base case
        if(exp == ""){ // if an empty string
            return 0;
        }
        if(size == 1){
            return exp[0]-'0';
        }else{ // recurrsion case
           char lastchar = exp[size-1];
           int lastnum = lastchar - '0';
           exp = exp.substr(0,size-1); // substrate the last num
           return convertStringToInteger(exp)*10 + lastnum;
        }
    }else{ // negtive num
        exp = exp.substr(1,size-1);
        return -convertStringToInteger(exp);
    }
    return 0;
}

bool isBalanced(string exp) {
    // TODO: write this function
    if(exp == ""){ // base case
        return true;
    } else {
        if (exp.find("()") == string::npos && exp.find("[]") == string::npos && exp.find("{}") == string::npos){
            return false;
        }else{ // recurrsion: if paired (){}[] are found, cut them and check the left until no pairs left
            if(exp.find("()") != string::npos){
                int index = exp.find("()");
                exp = exp.erase(index, 2);
            }
            if(exp.find("[]") != string::npos){
                int index = exp.find("[]");
                exp = exp.erase(index,2);
            }
            if(exp.find("{}") != string::npos){
                int index  = exp.find("{}");
                exp = exp.erase(index,2);
        }
            return isBalanced(exp);
        }
    }
}
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    // TODO: write this function
    if(order > 0){// recurrsion
    // draw a triangle from lefttop
    gw.drawLine(x,y, x+size, y);
    gw.drawLine(x,y, x+size/2, y+size*sqrt(3)/2);
    gw.drawLine(x+size, y, x+size/2, y+size*sqrt(3)/2);
    // draw three half-size triangles from lefttop, righttop bottom
    drawSierpinskiTriangle(gw, x, y, size/2, order-1);
    drawSierpinskiTriangle(gw,x+size/2,y,size/2,order-1);
    drawSierpinskiTriangle(gw, x+size/4, y+size*sqrt(3)/4,size/2,order-1);
    }
}

void floodFillHelper(int x, int y, int width, int height, int newColor, int oldColor){
    int currentColor = getPixelColor(x,y);
    if (oldColor == currentColor){// recurrsion conditions: 1.point(x,y) is different from the old color
        // 2. out of bounds
        // recurrsion dos: 1. change the color 2) explore the left, down, left, right
        setPixelColor(x,y,newColor);
        if(x >=1){ //left
        currentColor = getPixelColor(x-1,y);
        floodFillHelper(x-1,y,width,height,newColor, oldColor);
        }
        if(x < width - 1){ // right
        currentColor = getPixelColor(x+1,y);
        floodFillHelper(x+1,y,width,height,newColor, oldColor);
        }
        if(y >= 1){ // up
        currentColor = getPixelColor(x,y-1);
        floodFillHelper(x,y-1,width,height,newColor, oldColor);
        }
        if(y < height - 1){ //down
        currentColor = getPixelColor(x,y+1);
        floodFillHelper(x,y+1,width,height,newColor, oldColor);
        }
    }

}

void floodFill(int x, int y, int width, int height, int color) {
    // TODO: write this function
    int oldColor = getPixelColor(x,y); //get the point color to compare with the colors of other points befor change
    floodFillHelper(x, y, width, height, color, oldColor);
}

//read the file line by line, which is made of symbol and rule.
// collect the symbol(key) and comresponding rule(value) in a map
void generateMap(istream& input, HashMap<string, Vector< Vector<string> > >& map){
    string line;
    while(getline(input, line)){
    // separate the key and the possible value
        Vector<string> parts = stringSplit(line, "::=");
        string key = parts[0]; // get the key
        string pvalue = parts[1]; // possible value

        if(map.containsKey(key)){
            throw "The key already exists";  //if the symbol is already in the key, then throw an exception
        }else{
            //make the values vector
                Vector<string> rules;
                if(pvalue.find("|") != string::npos){ // separate rules by and put them in a vector
                rules = stringSplit(pvalue, "|");
                }else {
                rules.add(pvalue); // here the rule is still a string
                }
                int rulesSize = rules.size();
                Vector<Vector<string>> values;
                Vector<string> vectorRules; // change the rule string into rule vector
                for(int i=0; i < rulesSize; i++){
                    if(rules[i].find(" ") != string::npos){
                        vectorRules = stringSplit(rules[i]," ");// split each rule into vector by " "
                    }else{
                        vectorRules.clear(); // empty the vector from last one
                        vectorRules.add(rules[i]);
                    }
                    values.add(vectorRules);
                }
            // add key and value to map
            map.put(key, values);
        }
    }
}

// make a helper
void grammarGenerateHelper(HashMap<string, Vector< Vector<string> > >& map, string symbol, Vector<string>& output){
    Vector<Vector<string>> rules;
    int max;
    int min = 0;
    int rdm;
    Vector<string> randomRule;
    string symbolInRule;
    if(!map.containsKey(symbol)){ // basecase we get a terminal, which is not in the key: do nothing
       output.add(symbol);
       output.add(" "); // add a space
    }else{ // recursive step
        rules = map[symbol]; // get the rules from the map for the current symbol
        max = rules.size() - 1;
        rdm = randomInteger(min, max);
        randomRule = rules[rdm]; // get random rule from those rules
        for(int i = 0; i < randomRule.size(); i++){
            symbolInRule = randomRule[i];
            grammarGenerateHelper(map, symbolInRule, output); // recursion
        }
    }
}

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    // TODO: write this function
    if(symbol == ""){
        throw "This is an empty string";
    }
    HashMap<string, Vector< Vector<string> > > map;
    generateMap(input, map);
    Vector<string> output;
    Vector<string> v;
    for(int i=0; i < times; i++){ // generate grammar
        output.clear();
        grammarGenerateHelper(map, symbol, output);
        string outputWords;
        for (int i = 0; i<output.size(); i++) {
            outputWords += output[i];
        }
        v.add(outputWords);
    }
    return v;
}
