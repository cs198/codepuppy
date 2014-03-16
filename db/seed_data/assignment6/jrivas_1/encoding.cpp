// This is the Huffman Encoding file. It takes a file and
// creates an encoded, compressed version of it, that
// can then be decompressed through the program.

#include "encoding.h"
#include "strlib.h"
#include "simpio.h"
#include "filelib.h"
#include "pqueue.h"
#include <string>
#include <fstream>
#include <sstream>
#include <ostream>
#include <iostream>
#include <cmath>
#include <map>

//function declarations
void WriteBytes(string s, obitstream& output);
int RecurseThroughData(ibitstream& input, HuffmanNode* encodingTree, ostream& output);

using namespace std;

/*
 * This builds the frequency table for the input file/string.
 * It takes the file and creates a frequency map depending on
 * how many times each specific character shows up in the
 * file.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    int x = 0;
    while(true) {
        x = input.get();
        if (x == -1) break;
        if (freqTable.containsKey(x)) {
            int value = freqTable.get(x);
            value++;
            freqTable.remove(x);
            freqTable.put(x,value);
        } else {
            freqTable.put(x,1);
        }
    }
    freqTable.put(PSEUDO_EOF,1);
    return freqTable;
}
/*
 * This creates a tree out of a frequency table. It enqueues
 * the highest frequencies on the top, and creates it so that
 * the characters will always be leaves. If the encoding tree
 * sees that there is more than one element at the top of the
 * tree, it will keep reshuffling and put the less important
 * elements at the bottom of the tree until it has an appropriate
 * hierarchy.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    for (int x : freqTable) {
        HuffmanNode *node = new HuffmanNode(x,freqTable.get(x),NULL,NULL);
        pq.enqueue(node,freqTable.get(x));
    }
    while (pq.size() > 1) {
        HuffmanNode* hn1 = pq.dequeue();
        HuffmanNode* hn2 = pq.dequeue();
        int freq = hn1->count + hn2->count;
        HuffmanNode* node2 = new HuffmanNode(NOT_A_CHAR,freq,hn1,hn2);
        pq.enqueue(node2,freq);
    }
    return pq.peek();
}

/*
 * Creates an encoding map. This is a recursive function that
 * checks all of the leaves of the tree to make sure that they
 * are read into the map. It makes different maps and
 * then combines them at the end once it is sure it has
 * checked all the possible paths.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    Map<int, string> encodingMap2;
    Map<int, string> encodingMap3;
    if (encodingTree == NULL) return encodingMap;
    if (encodingTree->isLeaf())
        encodingMap.put(encodingTree->character,"");

    if (encodingTree->one != NULL) {
        encodingMap2 = buildEncodingMap(encodingTree->one);
        for (int x : encodingMap2)
            encodingMap.put(x, ("1" + encodingMap2.get(x)));
            //adds 1 to beginning of string

    }if (encodingTree->zero != NULL) {
        encodingMap3 = buildEncodingMap(encodingTree->zero);
        for (int x : encodingMap3)
            encodingMap.put(x,"0" + encodingMap3.get(x));
            //adds zero to beginning of string
    }
    return encodingMap;
}

/*
 * This encodes the data. It reads in character by character
 * from the input, and then using the encoding map, grabs the
 * appropriate byte, and uses a helper function to write the
 * byte to the output.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int code = input.get();
    while (code != -1) {
        string byte = encodingMap.get(code);
        WriteBytes(byte,output);
        code = input.get();
    }
    string byte2 = encodingMap.get(PSEUDO_EOF);
    WriteBytes(byte2,output);
}

/*
 * This helper function writes the bytes by
 * going through the byte string and writing the corresponding
 * 0 or 1 in bit format.
 */
void WriteBytes(string s, obitstream& output) {
    for (int i = 0; i < s.length(); i++) {
        if (s.at(i) == '0') output.writeBit(0);
        else output.writeBit(1);
    }
}

/*
 * This decodes the data. It keeps track of the root node.
 * It checks to make sure that the first node is not
 * PSEUDO_EOF, because that would mean that the only thing
 * in the encoding Tree is the PSEUDO_EOF. Otherwise, it
 * goes through the tree and recurses. When it returns, it goes
 * back to the root node. It continues until it reaches the end
 * of the input, when i is -1.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int i = 0;
    HuffmanNode *node = encodingTree;
    if (node->character == PSEUDO_EOF) return;
    while (i != -1) {
        i = RecurseThroughData(input,node,output);
    }
}

/*
 * This helper function recurses through the data. It first
 * checks to see if it is at a leaf; as long as
 * it is not at PSEUDO_EOF, it puts the character that it has landed
 * on into the output; it then returns so it can go back to the root.
 * Otherwise, it reads the next bit and goes to either the zero or the
 * one path.
 */
int RecurseThroughData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    if (encodingTree->isLeaf()) {
        if (encodingTree->character != PSEUDO_EOF)
            output.put(encodingTree->character);
        return encodingTree->character;
    }
    int bit = input.readBit();
    if (bit == -1) return bit;
    else if (bit == 0) RecurseThroughData(input,encodingTree->zero,output);
    else if (bit == 1) RecurseThroughData(input,encodingTree->one,output);
}

/*
 * This function compresses a file. It builds the frequency table
 * and then outputs the table. It then creates the encoding tree
 * and map using the table, clears the input and returns to the beginning,
 * and encodes the data using the above functions.
 */
void compress(istream& input, obitstream& output) {
    Map<int,int> freqTable = buildFrequencyTable(input);
    output << freqTable;
    HuffmanNode *node = buildEncodingTree(freqTable);
    Map<int,string> encodingMap = buildEncodingMap(node);
    input.clear();             // removes any current eof/failure flags
    input.seekg(0, ios::beg);  // tells the stream to seek back to the beginning
    encodeData(input,encodingMap,output);
    freeTree(node);
}

/*
 * This decompresses the input into the output. It creates
 * a frequency table, and then uses this to build the encoding
 * tree and decodes the data.
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode *node = buildEncodingTree(freqTable);
    decodeData(input,node,output);
    freeTree(node);
}

/*
 * Frees the tree in the memory in the tree in
 * a recursive way.
 */
void freeTree(HuffmanNode* node) {
    if (node == NULL) return;
    if (node->isLeaf()) {
        HuffmanNode* trash = node;
        delete trash;
        return;
    } else {
        if (node->one != NULL)
            freeTree(node->one);
        if (node->zero != NULL)
            freeTree(node->one);
    }
    HuffmanNode* trash = node;
    node = node->one;
    delete trash;
}
