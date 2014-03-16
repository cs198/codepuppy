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
#include "math.h"
#include "cmath"
#include <cmath>
using namespace std;


const double PI = 3.14159;

void convert(int& returnValue, int& counter, string& exp);
bool checkValid(char ch);
string checkString(char ch);
bool checkBalance(string& exp, int index);
void drawSubdivisions(GWindow& gw, int x, int y, int size, int order);
bool pixelInBounds(int x, int y, int width, int height);
void setFloodColor(int x, int y, int width, int height, int color, int pixelColor);
void changeColor(int x, int y, int width, int height, int color, int pixelColor);
void generateSentence(Map<string, string> map, string symbol, string& sentence);

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

int convertStringToInteger(string exp) {
    int counter=0;
    int returnValue=0;
    convert(returnValue, counter, exp);
    return returnValue;
}

void convert(int& returnValue, int& counter, string& exp) {
    if(exp=="") {
        cout << "final call" << endl;
        cout << "returnValue=" << returnValue << endl;
        //return returnValue;
    } else {
        char ch=exp[exp.length()-1];
        int next=ch-'0';
        cout << "ch=" << ch << endl;
        cout << "next=" << next << endl;
        double exp1=counter;
        int exponent=pow(10.0, exp1);
        next=next*exponent;
        returnValue+=next;
        cout << "returnValue=" << returnValue << endl;
        counter++;
        cout << "counter=" << counter << endl;
        exp.erase(exp.length()-1, 1);
        cout << "exp=" << exp << endl;
        convert(returnValue, counter, exp);
    }
}

bool isBalanced(string exp) {
    int index=0;
    return checkBalance(exp, index);
}

bool checkBalance(string& exp, int index) {
    if(exp=="") {
        cout << "string is empty, return true" << endl;
        return true;
    }
    if(index==exp.length()-1) {
        cout << "reached end of string, return false" << endl;
        return false;
    }
    char ch=exp[index];
    cout << "current ch=" << ch << endl;
    if(!checkValid(ch)) {
        cout << "invalid starting character" << endl;
        return false;
    }
    string check=checkString(ch);
    cout << "checkString=" << check <<endl;
    if(exp.find(check)!=string::npos) {
        exp.erase(exp.find(check), 2);
        cout << "found pair, new exp=" << exp << endl;
        index=0;
        cout << "index=" << index<< endl;
        if(checkBalance(exp, index)){
            return true;
        }
    } else {
        cout << "pair not found" << endl;
        index++;
        cout << "new index=" << index << endl;
        if(checkBalance(exp, index)) {
            return true;
        }
    }
    return false;
}

bool checkValid(char ch) {
    if(ch!='{' && ch!='(' && ch!='[') {
        return false;
    }
    return true;
}

string checkString(char ch) {
    if(ch=='(') {
        return "()";
    }
    if(ch=='[') {
        return "[]";
    }
    if(ch=='{') {
        return "{}";
    }
}

void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if(order>0){
        order--;
        gw.drawLine(x, y, x+size, y);
        gw.drawLine(x, y, x+size/2.0, y+size*sin(60*PI/180.0));
        gw.drawLine(x+size, y, x+size/2.0, y+size*sin(60*PI/180.0));
        drawSubdivisions(gw, x, y, size, order);
    }

}

void drawSubdivisions(GWindow& gw, int x, int y, int size, int order) {
    if(order>0) {
        order--;
        size=size/2.0;
        int x1=x+size;
        int x2=x+size*0.5;
        int x3=x+size*1.5;
        int y2=y+size*sin(60*PI/180.0);
        gw.drawLine(x1, y, x2, y2);
        gw.drawLine(x1, y, x3, y2);
        gw.drawLine(x2, y2, x3, y2);
        drawSubdivisions(gw, x, y, size, order);
        drawSubdivisions(gw, x1, y, size, order);
        drawSubdivisions(gw, x2, y2, size, order);
    }
}


void floodFill(int x, int y, int width, int height, int color) {
    int pixelColor=getPixelColor(x, y);
    setPixelColor(x, y, color);
    setFloodColor(x, y, width, height, color, pixelColor);
}

void setFloodColor(int x, int y, int width, int height, int color, int pixelColor) {
    int x1=x+1;
    int x2=x-1;
    int y1=y+1;
    int y2=y-1;
    changeColor(x, y1, width, height, color, pixelColor);
    changeColor(x, y2, width, height, color, pixelColor);
    changeColor(x1, y, width, height, color, pixelColor);
    changeColor(x2, y, width, height, color, pixelColor);
}

void changeColor(int x, int y, int width, int height, int color, int pixelColor) {
    if(pixelInBounds(x, y, width, height)) {
        if(getPixelColor(x, y)==pixelColor) {
            setPixelColor(x, y, color);
            setFloodColor(x, y, width, height, color, pixelColor);
        }
    }
}

bool pixelInBounds(int x, int y, int width, int height) {
    if(x>width-1 || x<0) {
        return false;
    }
    if(y>height-1 || y<0) {
        return false;
    }
    return true;
}

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Vector<string> v;
    Vector<string> vector;
    Map<string, string> map;
    string line;
    while(getline(input, line)) {
        //cout << line << endl;
        vector=stringSplit(line, "::=");
        if(map.containsKey(vector[0])) {
            throw error;
        }
        map.put(vector[0], vector[1]);
        //cout << "key=" << v[0] << endl;
        //cout << "value=" << map.get(v[0]) << endl;
    }
    //cout << "made map" << endl;
    for(int i=0; i<times; i++) {
        string sentence;
        generateSentence(map, symbol, sentence);
        v.add(sentence);
    }

    return v;
}

void generateSentence(Map<string, string> map, string symbol, string& sentence) {
    string value=map.get(symbol);
    Vector<string> rules=stringSplit(value, "|");
    int random=randomInteger(0, rules.size()-1);
    string rule=rules[random];
    Vector<string> components=stringSplit(rule, " ");
    cout << "made components vector" << endl;
    for(int i=0; i<components.size(); i++) {
        string component=components[i];
        cout << "generated components string" << endl;
        if(map.containsKey(component)) {
            generateSentence(map, component, sentence);
        } else {
            cout << "entered else loop" << endl;
            sentence=sentence+component+" ";
            cout << sentence << endl;
        }
    }

}
