// Min Cheol Kim
// SL Jimmy Lee

/* This assignment involved implementation of the Huffman encoding algorithm using HuffmanNodes.
 * The parts of hte assignment included both encoding and decoding Huffman codes. */

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"

void buildEncodingMapHelper(HuffmanNode* node, string code, Map<int, string> &encodingMap);
int getCharacter(ibitstream& input, HuffmanNode* encodingTree);
/* This function takes in an input stream and returns the frequency table of each occuring
 * character. The end of file is marked by the PSEUDO_EOF marker. */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while (input.good()) {
        int c = input.get();
        if (c == -1)
            freqTable[PSEUDO_EOF]++;
        else
            freqTable[c]++;
    }
    return freqTable;
}

/* This tree takes a frequency table and generates a tree of huffman nodes. Traversing this tree
 * later will tell us the strings for individual characters. */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    for (int key : freqTable)
        pq.enqueue(new HuffmanNode(key, freqTable.get(key), NULL, NULL), freqTable.get(key) );
    while (pq.size() > 1) {
        HuffmanNode* temp1 = pq.dequeue();
        HuffmanNode* temp2 = pq.dequeue();
        HuffmanNode* newNode = new HuffmanNode(NOT_A_CHAR, temp1->count + temp2->count, temp1, temp2);
        pq.enqueue(newNode,newNode->count);
    }
    return pq.dequeue();   // this is just a placeholder so it will compile
}

/* This function takes in an encoding tree built by the previous function and returns a map
 * of each character matched to its code. */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    // TODO: implement this function
    Map<int, string> encodingMap;
    string s = "";
    if (encodingTree != NULL)
        buildEncodingMapHelper(encodingTree, s, encodingMap);
    return encodingMap;
}

/* This function traverses the given tree, and when a leaf is reached, the path is saved
 * into the map and the recursion ends. */
void buildEncodingMapHelper(HuffmanNode *node, string code, Map<int, string>& encodingMap) {
    if (node->isLeaf()) {
        encodingMap.put(node->character, code);
        return;
    }
    buildEncodingMapHelper(node->zero, code + "0", encodingMap);
    buildEncodingMapHelper(node->one, code + "1", encodingMap);
}

/* This function takes in an input stream, an encoding map, and a bit outstream to take whatever
 * is in the input file and output the coded version of the message. */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while (input.good()) {
        int c = input.get();
        string temp = encodingMap.get(c);
        for (int n = 0; n < temp.length(); n++)
            output.writeBit(temp[n] - '0' );
    }
    for (int n = 0; n < encodingMap.get(PSEUDO_EOF).length(); n++) {
        output.writeBit(encodingMap.get(PSEUDO_EOF)[n] - '0');
    }
}

/* This function reverses what is done in the function above; it takes in a input bit stream,
 * a encoding tree, and out stream. The function outputs whatever is read in the input bit streambuf
 * translated back to characters. */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    while(input.good()) {
        int ch = getCharacter(input, encodingTree);
        if (ch == PSEUDO_EOF)
            return;
        output.put(ch);
    }
}

/* This function reversively walks the encoding tree until a leaf is reached, and when that
 * happens, the character is returned and recursion is stopped. */
int getCharacter(ibitstream &input, HuffmanNode *node) {
    if (!input.good() || node->isLeaf())
        return node->character;
    if (input.readBit() == 0)
        return getCharacter(input, node->zero);
    else
        return getCharacter(input, node->one);
}

/* This function takes in an input stream and out bit stream to compress a given file
 * into a huffman code. */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* tree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(tree);
    output << freqTable;
    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(tree);
}

/* This file takes in an bit input a huffman code and outputs the encoded characters. */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* tree = buildEncodingTree(freqTable);
    decodeData(input, tree, output);
    freeTree(tree);
}

/* This function frees up the memory taken up by the tree recursively. */
void freeTree(HuffmanNode* node) {
    if (node == NULL)
        return;
    freeTree(node->zero);
    freeTree(node->one);
    delete node;
}

