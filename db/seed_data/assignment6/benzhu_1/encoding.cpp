//Name: Benzheng Zhu
//CS 106B Assignment 6
//Citation: lecture slides
//Thie program compresses and decompresses any given file using Huffman's method

#include "encoding.h"
#include "bitstream.h"
#include "pqueue.h"
#include <string>
#include <fstream>
#include "filelib.h"

Map<int, int> buildFrequencyTable(istream& input) {
    //takes an istream by reference and reads the file
    //counts the occurances of each character in the file
    //stores the count in a map

    Map<int, int> freqTable;

    while (!input.fail()) {
        int c = input.get();
        if (c==-1) {
            freqTable[PSEUDO_EOF] = 1;
            break;
        }
        if (freqTable.containsKey(c)) {
            freqTable[c] += 1;
        } else {
            freqTable[c] = 1;
        }
    }
    return freqTable;
}

HuffmanNode* buildTreeHelper(PriorityQueue<HuffmanNode>& pq, HuffmanNode*& root) {
    //takes a priority queue and a pointer by reference
    //builds the encoding tree
    //returns the pointer pointing at the root of the tree

    if (pq.size()==1) {
        return root;
    } else {
        HuffmanNode h1 = pq.dequeue();
        HuffmanNode h2 = pq.dequeue();

        HuffmanNode* temp = new HuffmanNode(NOT_A_CHAR,h1.count+h2.count);
        temp->zero = new HuffmanNode(h1.character,h1.count,h1.zero,h1.one);
        temp->one = new HuffmanNode(h2.character,h2.count,h2.zero,h2.one);

        pq.enqueue(*temp, h1.count+h2.count);

        if ((h1.character == root->character && h1.count == root->count)||
                (h2.character == root->character && h2.count == root->count)){
            root = temp;
        }
        buildTreeHelper(pq,root);
    }
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    //takes the map by reference
    //stores the characters into a priority queue in the form of HuffmanNode, with the count being the priority
    //calls the Helper function to build the three

    PriorityQueue<HuffmanNode> pq;
    for (int c: freqTable) {
        pq.enqueue(HuffmanNode(c,freqTable[c]),freqTable[c]);
    }
    HuffmanNode* root = new HuffmanNode(pq.peek().character,pq.peek().count); //pq is never empty
    return buildTreeHelper(pq,root);
}

void buildMapHelper(Map<int, string>& encodingMap, HuffmanNode*& encodingTree, string code) {
    //takes the map, the pointer, and a string as inputs
    //traverses the tree until, and tracks the steps using the string
    //when reaching a leave, store the code for that characted in the map

    if (encodingTree == NULL) {
        return;
    }  else if (encodingTree->isLeaf()) {
        encodingMap[encodingTree->character] = code;
        return;
    } else {
        buildMapHelper(encodingMap,encodingTree->one,code+"1");
        buildMapHelper(encodingMap,encodingTree->zero,code+"0");
    }
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    //takes a pointer by reference
    //calls the Helper function to create the map

    Map<int, string> encodingMap;
    buildMapHelper(encodingMap,encodingTree,"");
    return encodingMap;
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    //takes an input file, an encoding map and a output file by reference
    //reads and encodes the input file character by character
    //writes the code for each character to the output file bit by bit

    while(!input.fail()) {
        int c = input.get();
        if (c == -1) {
            c = PSEUDO_EOF;
        }
        string s = encodingMap[c];
        for (int i=0; i<s.size(); i++) {
            output.writeBit(stringToInteger(s.substr(i,1)));
        }
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    //takes an encoded file, an encoding tree pointer, and a output file by reference
    //reads the encoded file, if 0 go left, if 1 go right, until reaches a leaf
    //writes the character stored in the leaf to the output file
    //resets the current pointers back to the root of the tree when a leaf is reached

    HuffmanNode* current = encodingTree;

    while (!input.fail()) {
        int b = input.readBit();
        if(input.fail()) {
            break;
        } else if (b == 0) {
            current = current->zero;
        } else if (b == 1) {
            current = current->one;
        }
        if (current->isLeaf()) {
            if (current->character == PSEUDO_EOF) {
                return;
            }
            output<<charToString(current->character);
            current = encodingTree;
        }
    }
}

void compress(istream& input, obitstream& output) {
    //compresses the provided input file
    //writes the code to the output file
    //frees the tree memory at the end

    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    output<<freqTable;
    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree); //free the memory
}

void decompress(ibitstream& input, ostream& output) {
    //decompresses the provided input file
    //writes the text to the output file
    //frees the tree memory at the end

    Map<int, int> freqTable;
    input>>freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree,output);
    freeTree(encodingTree); //free the memory
}

void freeTree(HuffmanNode* node) {
    //takes a pointer by reference
    //does nothing if the node is NULL
    //otherwise, frees the tree pointed by the pointer

    if (node == NULL) {
        return;
    } else {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
