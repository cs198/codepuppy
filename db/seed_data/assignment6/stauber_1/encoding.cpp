//Filename: encoding.cpp
//Author: Mark Stauber
//Description: This file contains the functions to either compress a file using huffman encoding
//or to decompress a compressed file that uses huffman encoding.

#include "encoding.h"
#include "filelib.h"
#include "pqueue.h"
#include "bitstream.h"
#include "strlib.h"

//Recursive helper funciton to build the encoding map.
void buildMap(HuffmanNode* node, string code, Map<int, string>& encodingMap);

//Recursive helper function to decode an input using an encoding tree and
//output the result.
bool decodeHelper(ibitstream& input, HuffmanNode* node, ostream& output);

//Builds a frequency table for input stored as a Map with key: character and value: count
//characters are stored as int to allow for additional characters PSEUDO_EOF and NOT_CHAR
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;

    while(true){
        int val = input.get();
        //if input.get() returns -1, then at EOF
        if(val < 0) break;
        //Increment count of val
        freqTable[val]++;
    }
    //Increment PSEUDO_EOF to 1
    freqTable[PSEUDO_EOF]++;
    return freqTable;
}

//Builds an encoding tree from a frequency table
//by first unloading it into a priority queue
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> myqueue;
    for(int i : freqTable){
        HuffmanNode* node = new HuffmanNode(i, freqTable[i]);
        myqueue.enqueue(node, node->count);
    }

    //pointer to top of tree as it forms.
    HuffmanNode* parent;
    //If there is only one entry in myqueue then return it.
    if(myqueue.size() == 1){
        parent = myqueue.dequeue();
        return parent;
    }
    //Otherwise gradually build tree from first two entries in priority queue.
    while(myqueue.size() > 1){
        parent = new HuffmanNode();
        //Add first entry under parent
        HuffmanNode* child = myqueue.dequeue();
        parent->count = child->count;
        parent->zero = child;
        //Add second entry under parent
        child = myqueue.dequeue();
        parent->count += child->count;
        parent->one = child;
        myqueue.enqueue(parent, parent->count);
    }
    return parent;
}

//Builds an encoding map using recursive function buildMap from an encoding tree
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    buildMap(encodingTree, "", encodingMap);
    return encodingMap;
}

//Encodes data from input using encoding map and stores in output
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while(true){
        int val = input.get();
        //if input.get() returns -1, then at EOF
        if(val < 0) break;
        //Write out encoded form of val using encodingMap
        for(char i: encodingMap[val]){
            output.writeBit(stringToInteger(charToString(i)));
        }
    }
    //Add PSEUDO_EOF to end of ouput
    for(char i: encodingMap[PSEUDO_EOF])
        output.writeBit(stringToInteger(charToString(i)));
}

//Decodes data from input with encoding tree using recursive helper function decodeHelper
//and stores in output.
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    //Each call to decodeHelper decodes a single character. Returns false when at EOF.
    while(decodeHelper(input, encodingTree, output));
}

//Compresses input and stores result in output. Will throw error if input is empty file.
void compress(istream& input, obitstream& output) {
    if(input.get() < 0) throw("Empty Input");
    rewindStream(input); //rewind to beginning of file after checking first character.

    //Build encodingMap
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree  = buildEncodingTree(freqTable);
    Map<int, string> encodingMap  = buildEncodingMap(encodingTree);

    //Compress input using encodingMap and store in output
    rewindStream(input);
    output<<freqTable;
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

//Decompresses input and stores in output. Will throw error if input is empty throws.
//Otherwise function assumes that there is a frequency table at the top of the input file.
void decompress(ibitstream& input, ostream& output) {
    if(input.get() < 0) throw("Empty Input");
    rewindStream(input); //rewind to beginning of file after checking first character.

    Map<int, int> freqTable;
    input>>freqTable;
    HuffmanNode* encodingTree  = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

//Frees the memory stored by the encoding tree
void freeTree(HuffmanNode* node) {
    if(node != NULL){
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}

//Recursive helper function that encoding map from encoding tree.
void buildMap(HuffmanNode* node, string code, Map<int, string>& encodingMap){
    if(node != NULL){
        if(node->isLeaf()){
            encodingMap[node->character] = code;
            return;
        }
        buildMap(node->zero, code + '0', encodingMap);
        buildMap(node->one, code + '1', encodingMap);
    }
}

//Recursive helper function that decodes a single char from input.
//Returns false when val is PSEUDO_EOF.
bool decodeHelper(ibitstream& input, HuffmanNode* node, ostream& output){
    //If *node is a leaf it stores a character and recursion has ended.
    if((*node).isLeaf()){
        int val = node->character;
        if(val == PSEUDO_EOF) return false;
        output.put(val);
        return true;
    }

    //Else readBit and move down tree in that direction.
    int bit = input.readBit();
    if(bit == 0){
        return decodeHelper(input, node->zero, output);
    }else{
        return decodeHelper(input, node->one, output);
    }
}
