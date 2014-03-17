/* Name: Kevin Vo
 * TA: Jimmy Lee
 * These are the sets of recursion problems that are called by main. The user has
 * six possible choices: Karel,converting a string to an integer,checking a balanced
 * sentence, flood fill and grammarGenerater. Note I did not do the Karel problem becuase
 * it was optional. Further detail for each choice is detailed in the comments above each
 * function.
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
#include "filelib.h"
using namespace std;

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}
/* The following two functions convert a string into an integer.
 * The strategy that I implemented was to parse the string into
 * parts while taking into account the digits place the string was
 * parsed at, hence the need for a count, which increments by a factor
 * of 10. Then the rest is just adding to sum. Result is the final output string.
*/
int convertStringHelper(string exp, int count, int result){
    if (exp.length() == 0){
        return result;
    }else{
        int first = exp[exp.size()-1]-'0';
        if (first == ('-' - '0')){
            result = result*-1;
        }else{
            result += first*count;
        }
        string rest = exp.substr(0, exp.size()-1);     
        return convertStringHelper(rest, count*10, result);
    }
}

int convertStringToInteger(string exp) {
    return convertStringHelper(exp,1,0);
}

/* The strategy to check for balance is to examine each case
 * (,{,[ and see if it has a match. If it does then check to see
 * if (), {}, [] exist (this is to for the edge case of nested brackets.
 * To check if removing the substring will keep the balance intact I
 * use recursion.
 */

bool isBalanced(string exp) {
    if (exp.length() == 0){
        return true;
    }else{
        char first = exp[0];
        switch (first){
            case '{':
                if (exp.find("}") ==  -1){
                    return false;
                }else if (exp.find("()") != -1 || exp.find("{}") != -1 || exp.find("[]") != -1){
                    exp.erase(exp.find("}"),1);
                    exp.erase(0,1);
                } else{
                    return false;
                }
                break;
            case '[':
                if (exp.find("]") == -1){
                    return false;
                } else if (exp.find("()") != -1 || exp.find("{}") != -1 || exp.find("[]") != -1){
                    exp.erase(exp.find("]"),1);
                    exp.erase(0,1);
                } else{
                    return false;
                }
                break;
            case '(':
                if (exp.find(")") == -1){
                    return false;
                }else if (exp.find("()") != -1 || exp.find("{}") != -1 || exp.find("[]") != -1){
                    exp.erase(exp.find(")"),1);
                    exp.erase(0,1);
                } else{
                    return false;
                }
                break;
            default:
                return false;
                break;
        }
        return isBalanced(exp);
    }
}
/*
    The two following functions are responsible for drawing the Sierpinski
    Triangle. I used a switch statement to calculate the tree points on which
    the recursion will follow through on. I then recursively call the helper
    function three times because the pattern of the Sierpinski triange is to draw
    3 sets of triangles, within another triangle.
*/
void drawSHelper(GWindow& gw, int x, int y, int size, int order, int count){
    if (order == 0){
        return;
    } else {
        int nsize = size;
        int nx = x;
        int ny = y;
        if (count != 0){
            switch (count){
            case 1:
                nsize = size/2.0;
                break;
            case 2:
                nx = x + size/2.0;
                nsize = size/2.0;
                break;
            case 3:
                nsize = size/2.0;
                ny = y + sqrt(3)/2.0 *(size)/2.0;
                nx = x + nsize/2.0;
                break;
            }
        }
        gw.drawLine(nx, ny, nx + nsize, ny);
        gw.drawLine(nx + nsize, ny, nx + nsize/2.0, ny + (sqrt(3.0)/(2.0))*nsize);
        gw.drawLine(nx + nsize/2.0, ny + (sqrt(3.0)/2.0)*nsize,nx, ny);

        drawSHelper(gw, nx, ny, nsize,order - 1, 1);
        drawSHelper(gw, nx, ny, nsize,order - 1, 2);
        return drawSHelper(gw, nx, ny, nsize, order - 1, 3);
    }
}
void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order){
    int count = 0;
    return drawSHelper(gw,x,y,size,order,count);
}
/*
    My strategy for implementing floodFill was to recursively explore four paths: up,
    down, left, right. The base case triggers when the next pixel's color does not equal
    the old color.
*/
void floodFillHelper(int x, int y, int width, int height, int color, int oldColor){
    int nextColor = getPixelColor(x,y);
    if (nextColor != oldColor || oldColor == color){
        return;
    }else{
        setPixelColor(x,y,color);
        floodFillHelper(x,y-1,width, height, color, oldColor);
        floodFillHelper(x-1,y,width,height, color, oldColor);
        floodFillHelper(x, y + 1, width, height, color, oldColor);
        return floodFillHelper(x+1,y,width, height, color, oldColor);
    }
}
void floodFill(int x, int y, int width, int height, int color) {
    int oldColor = getPixelColor(x,y);
    return floodFillHelper(x,y,width,height,color,oldColor);
}
/*
    For this function I broke it up into these steps.
    1) I created a map that mapped non terminals to rules
    2) I then used recursion to go through the map pulling out
       the values(vectors)
    3) Keep recursing until the value is not a key in the map, this means I have hit a terminal
    4) Use a for loop to determine the number of output strings to create.
    5) Output the string to the console

    The helper function is for the recursive part of the functionality
*/
void grammarGenerateHelper (HashMap<string, Vector <string>>& map, string symbol, string& output){
    if (!map.containsKey(symbol)){
        output += (symbol + " ");
    } else{
        Vector <string> rules = map.get(symbol);
        int random = randomInteger(0,rules.size()-1);
        string rule = rules[random];
        Vector <string> temp = stringSplit(rule," ");
        for(string value : temp){
            grammarGenerateHelper(map, value, output);
        }
    }
}

/* This creates the map and prints out the output strings
*/
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    int i = 0;
    string line;
    Vector <string> keys;
    HashMap<string, Vector <string>> map;
    while (getline(input,line)){
        Vector <string> first = stringSplit(line,"::=");
        if (map.containsKey(first[0])){
            throw "Illegal File Format Keys Must Be Unique!";
        }
        keys.add(first[0]);
        first.remove(0);
        Vector<string> values = stringSplit(first[0],"|");    
        map.put(keys[i],values);
        i++;
    }
    Vector <string> endProduct;
    for (int i = 0; i < times; i++){
        string output = "";
        grammarGenerateHelper(map, symbol, output);
        endProduct.add(output);
    }
   return endProduct;
}
