/*
 * CS106B Assignment #6: Huffman Encoding
 * Emma Daugherty
 * Section Leader: Jimmy Lee
 *
 * This program implements Huffman encoding, allowing files to be compressed and decompressed using the Huffman
 * Encoding method.
 */

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"

//Helper methods
int decodeDataHelper(ibitstream& input, HuffmanNode* node);
void buildEncodingMapHelper(HuffmanNode* node, Map<int, string>& map, string result);

/*
 * This method reads a file and builds a frequency table based off the frequency of each character
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;

    int ch;
    ch = input.get();
    while (true) {
        if (ch == EOF) break;
        freqTable[ch]++;
        ch = input.get();
    }
    freqTable.put(PSEUDO_EOF, 1);

    return freqTable;
}

/*
 * This method builds a tree of all the values, where all the leaves are HuffmanNodes that have a character and
 * that characters frequency.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    for (int key : freqTable) {
        pq.enqueue(new HuffmanNode(key, freqTable[key], NULL, NULL), freqTable[key]);
    }

    while (pq.size() > 1) {
        HuffmanNode* zero = pq.dequeue();
        HuffmanNode* one = pq.dequeue();
        int sum = zero->count + one->count;
        pq.enqueue(new HuffmanNode(NOT_A_CHAR, sum, zero, one), sum);
    }

    HuffmanNode* root = pq.dequeue();
    return root;

}

/*
 * This method builds an encoding map from the tree. Each nodes left pointer represents a '0' and right pointer
 * represents a '1'. The map stores the characters as keys and the encoded representation, a string of ones and
 * zeros, as values.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;

    string result = "";
    buildEncodingMapHelper(encodingTree, encodingMap, result);

    return encodingMap;
}

/*
 * This method is a helper function for buildEncodingMap that recursively traces through the tree until a leaf
 * is reached, then that character is put in the map and mapped to its encoded form.
 */
void buildEncodingMapHelper(HuffmanNode* node, Map<int, string>& map, string result) {
    if (node->isLeaf()) {
        map.put(node->character, result);
    } else {
        buildEncodingMapHelper(node->zero, map, result + "0");
        buildEncodingMapHelper(node->one, map, result + "1");
    }
}

/*
 * This method encodes a piece of data based off of the encoding map. It reads in the characters and codes them,
 * then writes the code to a new file.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    rewindStream(input);
    int ch;
    ch = input.get();
    while (true) {
        if (ch == EOF) break;
        string code = encodingMap[ch];
        for (int i = 0; i < code.length(); i++) {
            output.writeBit(code[i] - '0');
        }
        ch = input.get();
    }
    string code = encodingMap[PSEUDO_EOF];
    for (int i = 0; i < code.length(); i++) {
        output.writeBit(code[i] - '0');
    }
}

/*
 * This method decodes the data from a file.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {

    int letter = NOT_A_CHAR;
    while (true) {
        letter = decodeDataHelper(input, encodingTree);
        if (letter == PSEUDO_EOF) break;
        output << (char)letter;
    }

}

/*
 * This method helps decodeData by reading in one bit at a time and traversing the tree recursively until it
 * hits a leaf. Then in returns the character at that leaf in int form.
 */
int decodeDataHelper(ibitstream& input, HuffmanNode* node) {
    if (node->isLeaf()) {
        return node->character;

    } else { 
        int bit = input.readBit();
        if (bit != -1) {
            if (bit == 0) {
                return decodeDataHelper(input, node->zero);
            } else {
                return decodeDataHelper(input, node->one);
            }
        }
    }
}

/*
 * This method compresses a file by coding it and writing it to a new file.
 */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    output << freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

/*
 * This method decompresses a file by decoding it and writing it to a new file.
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    rewindStream(input);
    input >> freqTable;
    HuffmanNode* pointer = buildEncodingTree(freqTable);
    decodeData(input, pointer, output);
    freeTree(pointer);
}

/*
 * This method deletes all the nodes of an encoding tree when passed in a pointer to the root.
 */
void freeTree(HuffmanNode* node) {
    if (node == NULL) {
        return;
    } else {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
