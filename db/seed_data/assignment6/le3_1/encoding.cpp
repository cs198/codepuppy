// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "encoding.h"
#include "HuffmanNode.h"
#include "huffmanutil.h"
#include "map.h"
#include "strlib.h"
#include "pqueue.h"
// TODO: include any other headers you need

void buildEncodingMapHelper(HuffmanNode* node, string encoding,Map<int, string>& encodingMap);
int decodeDataHelper(ibitstream& input, HuffmanNode* node, int decodeInteger);

//use input to build the frequency table.
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    int nextChar = input.get();
    while(nextChar != -1){ // get char before EOF
        if(freqTable.containsKey(nextChar)){ // if the char is already in the map
            freqTable[nextChar]++;   // count++
        }else{
            freqTable.put(nextChar,1); // if char is not in the map, set count to be 1
        }
        nextChar = input.get();  // get next char
    }
    //nextChar = input.get(); // get the EOF
    nextChar = 256; // set EOF int to be 256
    freqTable.put(nextChar, 1); // put EOF in the map
    return freqTable;
}

//Use frequency table to build the encoding tree
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    // TODO: implement this function
    PriorityQueue<HuffmanNode*> pq; // creat a priority queue
    Vector<int> freqKeysVector = freqTable.keys();
    for(int i = 0; i < freqKeysVector.size(); i++){
        int character = freqKeysVector[i];
        int count = freqTable[character];
        HuffmanNode* hnode = new HuffmanNode(character, count, NULL, NULL);
        pq.enqueue(hnode, hnode->count);
    }
    while(pq.size() != 1){
        HuffmanNode* denode1 = pq.dequeue(); // dequeue the two elements
        HuffmanNode* denode2 = pq.dequeue();
        HuffmanNode* comNode = new HuffmanNode(NOT_A_CHAR,denode1->count+denode2->count, denode1,denode2); // add them up and put the new node back the PQ
        pq.enqueue(comNode,comNode->count);
    }
    HuffmanNode* root = pq.dequeue();
    return root;   // return the root
}

//Use the encoding tree to build the encoding map
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string encoding;
    if(encodingTree == NULL){
        encodingMap.clear(); // if the tree is empty, clear the map
    }else{
        buildEncodingMapHelper(encodingTree,encoding,encodingMap);
    }
    return encodingMap;
}

// helper function
void buildEncodingMapHelper(HuffmanNode* node, string encoding,Map<int, string>& encodingMap){
    if(node->isLeaf()){
        encodingMap.put(node->character, encoding);
        return;
    }else{
        buildEncodingMapHelper(node->zero,encoding + "0",encodingMap);
        buildEncodingMapHelper(node->one,encoding + "1",encodingMap);
    }
}

//read one character at a time from a given input file,
//and use the provided encoding map to encode each character to binary,
//then write hte character's encoded binary bits to the given bit output bit steam
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    // TODO: implement this function
    int nextChar = input.get();
    string encodingText;
    while(nextChar != -1){ // get char before EOF
        encodingText += encodingMap.get(nextChar);
        nextChar = input.get();
    }
    nextChar = 256;
    encodingText += encodingMap.get(nextChar);
    for(int i = 0; i < encodingText.size(); i++){ // put the int (transfered from the char) to the output
        int encodingInteger = stringToInteger(encodingText.substr(i,1));
        output.writeBit(encodingInteger);
    }
}

//read bits from the given input file one at a time,
//and recursively walk through the specified decoding tree
//to write the original uncompressed contents of the file
//to the given output stream.
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    // TODO: implement this function
    int decodeInteger = input.readBit();
    while(decodeInteger != -1){
        int newChar = decodeDataHelper(input, encodingTree, decodeInteger); // get a new char from helper function
        if(newChar != 256){
            output.put(newChar);// add the character to the output
        }else{
            break;
        }
        decodeInteger = input.readBit();
    }
}

// decode helper function
int decodeDataHelper(ibitstream& input, HuffmanNode* node, int decodeInteger){
   if(decodeInteger == 0){ // if read a zero
      if(node->zero->isLeaf()) { // if it reaches a leaf
          return node->zero->character;
      }else{ // if it is not a leaf
          int deInt = input.readBit(); // read the next bit
          return decodeDataHelper(input, node->zero, deInt);
      }
   }else if(decodeInteger == 1){ // if read a one
      if(node->one->isLeaf()){
          return node->one->character;
      } else{
          int deInt = input.readBit();
          return decodeDataHelper(input, node->one, deInt);
      }
   }
}

//compress the given input file into the given output file
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input); // input->frequency table
    input.clear(); // rewind the input file
    input.seekg(0, ios::beg);
    output << freqTable; // input the frequency table in the output file
    HuffmanNode* encodingTree = buildEncodingTree(freqTable); // use the frequency table to build the encoding tree
    Map<int, string> encodingMap = buildEncodingMap(encodingTree); //Use  the encoding tree to build the encoding map
    encodeData(input,encodingMap,output); //encode input file with encoding map to output
    freeTree(encodingTree); // clear the memory
}

void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable; // read out the frequency table
    HuffmanNode* encodingTree = buildEncodingTree(freqTable); // use the frequency table to build the encoding tree
    decodeData(input, encodingTree, output); // use input file and encoding tree to build the output
    freeTree(encodingTree); // clear the memory
}

// free the memory associated with the tree whose root node
// is represented by the given pointer
void freeTree(HuffmanNode* node) {
    if(node->isLeaf()){ // if the node is leaf
        delete node; //delete the node
        return;
    }else{
        HuffmanNode* nodeZero = node->zero; //go to zero side
        HuffmanNode* nodeOne = node->one; // go to one side
        delete node; // delete the node
        freeTree(nodeZero); // recurse the one and zero sides.
        freeTree(nodeOne);
    }
}
