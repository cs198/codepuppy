// This file includes the implementation for a program that has the capacity to compress and decompress inputted files

#include "encoding.h"

#include "pqueue.h"
#include "string.h"
#include "strlib.h"
#include "filelib.h"

int huffmanCode(Map<int, string>& encodingMap, HuffmanNode* node, string code);
void writeBits(string code, obitstream& output);
int decoded(HuffmanNode* encodingTree, ibitstream& input);

//This method builds a "frequency table," or a map of characters in a given stream to the number of times that they have occurred
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    char ch;
    while(input.get(ch)) {
        if(freqTable.containsKey(ch)==false) {
            freqTable.put(ch, 1);
        } else {
            int freq=freqTable.get(ch);
            freq++;
            freqTable.put(ch, freq);
        }
    }
    freqTable.put(PSEUDO_EOF, 1);
    return freqTable;
}

//This method builds an "encoding tree." It begins by building a priority queue of characters in a given frequency table, with elements that have the lowest frequency being placed at the front. This method will systematically combine the first two nodes in the queue by making them the children of a new node whose priority is the sum of its children, and then re-enqueue the new node. This process will continue until there is only one node left in the tree, at which point this method will return a pointer associated with the node.
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pqueue= PriorityQueue<HuffmanNode*>();
    for(int ch:freqTable) {
        int freq=freqTable.get(ch);
        HuffmanNode* node=new HuffmanNode(ch, freq);
        pqueue.enqueue(node, freq);
    }
    while(pqueue.size()>1) {
        HuffmanNode* node1=pqueue.dequeue();
        HuffmanNode* node2=pqueue.dequeue();
        int freq1=node1->count;
        int freq2=node2->count;
        int sum=freq1+freq2;
        HuffmanNode* zero=node1;
        HuffmanNode* one=node2;
        HuffmanNode* newNode=new HuffmanNode(NOT_A_CHAR, sum, zero, one);
        pqueue.enqueue(newNode, sum);
    }
    HuffmanNode* encodingTree=pqueue.peek();
    return encodingTree;
}

//This method builds an "encoding map," which contains characters and their associated binary codes, as determined by traversing the encoding tree passed in by a pointer.
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string code="";
    huffmanCode(encodingMap, encodingTree, code);

    return encodingMap;
}

//This method recursively finds the binary codes for all the characters in the encoding tree. It takes in a pointer to a HuffmanNode, an encoding map, and a string representing the current binary code. If the pointer represents a leaf, this method will add the character and its corresponding code to the map. If not, it will update the pointer and the code, and recurse down the tree until it finds a leaf to add to the map.
int huffmanCode(Map<int, string>& encodingMap, HuffmanNode* node, string code) {
    if(node->isLeaf()) {
        int ch=node->character;
        encodingMap.put(ch, code);
    } else {
        if(node->zero!=NULL) {
            string newCode=code;
            newCode+="0";
            huffmanCode(encodingMap, node->zero, newCode);
        }
        if(node->one!=NULL) {
            string newCode=code;
            newCode+="1";
            huffmanCode(encodingMap, node->one, newCode);
        }
    }
}

//This method will take in a reference to an input stream of characters, find the binary code associated with each character, and encode the input stream into a binary representation
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    char ch;
    while(input.get(ch)) {
        string code=encodingMap.get(ch);
        writeBits(code, output);
    }
    cout << endl;
    string code=encodingMap.get(PSEUDO_EOF);
    writeBits(code, output);
}

//This method takes in a string representing a binary code, and writes the contents of the code to an obitstream
void writeBits(string code, obitstream& output) {
    for(int i=0; i<code.length(); i++) {
        char character=code.at(i);
        string stringCode="";
        stringCode+=character;
        int intCode=stringToInteger(stringCode);
        output.writeBit(intCode);
    }
}

//This method decodes binary data using a pointer to an encoding tree, and prints out a decoded string of characters
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    while(true) {
        int ch=decoded(encodingTree, input);
        if(ch==PSEUDO_EOF) {
            break;
        } else {
            output.put(ch);
        }
    }
}

//This method recursively finds characters associated with the given binary input stream. Using the bits in the stream, it will recurse down the encoding tree until it finds a leaf character, which it subsequently returns
int decoded(HuffmanNode* node, ibitstream& input) {
    if(node->isLeaf()) {
        return node->character;
    } else {
        int bit=input.readBit();
        if(bit==0) {
            HuffmanNode* zero=node->zero;
            return decoded(zero, input);
        }
        if(bit==1) {
            HuffmanNode* one=node->one;
            return decoded(one, input);
        }
    }
}

//This method combines previous methods to compress inputted data by building a frequency table from the input, building an encoding tree and encoding map, and then encoding the data. It also generates a header with the information from the frequency table to help subsequent calls to "decompress" reconstruct encoding information
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable=buildFrequencyTable(input);
    output << freqTable;
    HuffmanNode* encodingTree=buildEncodingTree(freqTable);
    Map<int, string> encodingMap=buildEncodingMap(encodingTree);
    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

//This method combines previous methods to decompress data. It reads in a header and encoded data, and the builds an encoding tree to decompress the data
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* encodingTree=buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

//This method frees any data associated with  given encoding tree
void freeTree(HuffmanNode* node) {
    if(node!=NULL) {
        HuffmanNode* zero;
        HuffmanNode* one;
        if(node->zero!=NULL) {
            zero=node->zero;
        }
        if(node->one!=NULL) {
            one=node->one;
        }
        delete node;
        freeTree(zero);
        freeTree(one);
    }
}
