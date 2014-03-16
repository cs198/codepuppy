/* Michelle S. Lee
   CS 106B
   Section Leader: Jimmy Lee

This program implements the Huffman encoding algorithm to compress and decompress files.*/

#include "encoding.h"
#include "simpio.h"
#include <iostream>
#include "pqueue.h"
#include "HuffmanNode.h"
#include "bitstream.h"
#include "filelib.h"


/* Reads input from a given istream (which could be either a file on disk, a string buffer, etc.)
 * Counts and returns a mapping from each character to the number of times that character appears in the file.
 * Assumptions: input file exists and can be read.
 * An empty file would return a map containing only the 1 occurrence of PSEUDO_EOF.
 */
Map<int, int> buildFrequencyTable(istream& input) {

    Map<int, int> freqTable;

    //Read input until it reaches the end
    while (input.peek() != -1) {
        int ch = input.get();

        //Add the character (represented as integer) to the map
        if (!freqTable.containsKey(ch)) {
            freqTable.put(ch, 1);
        } else {
            int freq = freqTable.get(ch);
            freq++;
            freqTable.put(ch, freq);
        }
    }

    //Add a fake character to represent the end of the input file
    freqTable.put(PSEUDO_EOF, 1);

    return freqTable;
}

/* Accepts a frequency table and uses it to create a Huffman encoding tree based on those frequencies.
 * Returns a pointer to the node representing the root of the tree.
 *
 * Assumptions: frequency table is valid, it does not contain any keys other than char values, PSEUDO_EOF, and NOT_A_CHAR;
 *    all counts are positive integers; it contains at least one key/value pairing
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {

    // Init. priority queue to keep track of which nodes to process next
    PriorityQueue<HuffmanNode*> q;

    //Iterate through the frequency table and create HuffmanNode nodes
    //Enqueue the node and its "priority" or frequency value
    for (int ch : freqTable) {
        HuffmanNode* node = new HuffmanNode();
        node->character = ch;
        node->count = freqTable.get(ch);
        q.enqueue(node, node->count);
    }

    //Dequeue two nodes from queue and joins them into a new node with
    //whose frequency is their sum
    while (q.size() > 1) {
        HuffmanNode* firstNode = q.dequeue();
        HuffmanNode* secondNode = q.dequeue();
        HuffmanNode* newNode = new HuffmanNode();

        //The two nodes are placed as children of the new node
        newNode->zero = firstNode;
        newNode->one = secondNode;
        newNode->character = NOT_A_CHAR;
        newNode->count = firstNode->count + secondNode->count;

        //Reinsert node into the queue
        q.enqueue(newNode, newNode->count);
    }

    //dequeue the root with the HuffmanNode tree
    HuffmanNode* root = q.dequeue();

    return root;
}

/* This helper function accepts a pointer to the root node of the Huffman Tree,
 the encoding map, and an empty string. It recursively modifies the map to add
 the character and the binary encoding for each node*/
void buildEncodingMapHelper(HuffmanNode* currentNode, Map<int, string>& encodingMap, string currentPath) {

    //base case
    if (currentNode->isLeaf()) {
        encodingMap.put(currentNode->character, currentPath);
        return;
    }

    //Recurse on left and right nodes
    buildEncodingMapHelper(currentNode->zero, encodingMap, currentPath + "0");
    buildEncodingMapHelper(currentNode->one, encodingMap, currentPath + "1");
}

/* Accepts a pointer to the root node of a Huffman tree and uses it to create
 * and return a Huffman encoding map based on the tree's structure */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {

    Map<int, string> encodingMap;
    buildEncodingMapHelper(encodingTree, encodingMap, "");
    return encodingMap;
}

/* Reads one character at a time from a given input file and uses the provided encoding map
 * to encode each character to binary, then writes the character's encoded binary bits
 * to the given bit output bit stream

    Assumptions: parameters are valid, and the streams are already opened*/

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {

    //For each character in the input, get the code from the map
    input.seekg(0, input.beg);
    while (input.peek() != -1) {
        int ch = input.get();
        string code = encodingMap.get(ch);
        //and write the binary code to the output file
        writeCodeToOutput(output, code);
    }

    //Write a single occurrence of the binary encoding for PSEUDO_EOF into the output
    //to identify the end of the data when decompressing the file later
    string pseudoEOF = encodingMap.get(PSEUDO_EOF);
    writeCodeToOutput(output, pseudoEOF);
}

//For each 0 or 1 in the binary code, write either 0 or 1 to the output
void writeCodeToOutput (obitstream& output, string code) {
    for (int i = 0; i < code.length(); i++) {
        char c = code[i];
        if (c == '0') {
            output.writeBit(0);
        } else {
            output.writeBit(1);
        }
    }
}

//This helper function reads bits from the given input file one at a time, and recursively walk through the specified
// decoding tree to write the original uncompressed contents of that file to the given output stream
int decodeDataHelper(ibitstream& input, HuffmanNode* currentNode) {

    //Base case
    if (currentNode->isLeaf()) {
        return currentNode->character;
    }

    //If 0, go left; if 1, go right
    if (input.readBit() == 0) {
        return decodeDataHelper(input, currentNode->zero);
    } else {
        return decodeDataHelper(input, currentNode->one);
    }
}

// This function decodes each bit from the input file and writes the original uncompressed contents
// of that file to the given output stream
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int ch = decodeDataHelper(input, encodingTree);

    // Until end of file encoding is identified,
    // Call the helper function for each character
    while (ch != PSEUDO_EOF) {
        output << char(ch);
        ch = decodeDataHelper(input, encodingTree);
    }
}

// This function compresses the given input file into the given output file
void compress(istream& input, obitstream& output) {
    //Return if the input file is empty
    if (input.peek() == -1) return;

    //Build a frequency table from the input file
    Map<int, int> freqMap = buildFrequencyTable(input);
    //Build an encoding tree
    HuffmanNode* node = buildEncodingTree(freqMap);
    //Create a map from the tree
    Map<int, string> encodingMap = buildEncodingMap(node);

    //Write the frequency map to the output file as a header
    output << freqMap;

    //Clear and rewind to the beginning of the input
    input.clear();
    rewindStream(input);

    //Go back through the input file and write its encoding to the output file
    encodeData(input, encodingMap, output);

    //Free the memory associated with the tree
    freeTree(node);
}


/* This function reads the bits from the given input file one at a time, including
 * the header packed inside the start of the file, to write the original contents
 * of that file to the file specified by the output parameter */
void decompress(ibitstream& input, ostream& output) {

    //If empty file, return
    if (input.peek() == -1) return;

    //Get the frequency table from the header
    Map<int, int> frequencyTable;
    input >> frequencyTable;

    //Build tree from the frequency table and write the decoded data to the output file
    HuffmanNode* node = buildEncodingTree(frequencyTable);
    decodeData(input, node, output);

    //Free the memory associated with the tree
    freeTree(node);
}

/* This function frees the memory associated with the tree
 * whose root node is represented by the given pointer */
void freeTree(HuffmanNode* node) {
    if (node == NULL) return;
    freeTree(node->one);
    freeTree(node->zero);
    delete node;
}
