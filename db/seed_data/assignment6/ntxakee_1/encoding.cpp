/* Name: Lindsey Txakeeyang
 * Course: CS106B, Wednesday 4:15 Section
 * Assignment: Huffman Encoding
 * This program intakes files to encode and decode data using the Huffman algorithm.
 */

#include "encoding.h"
#include "pqueue.h"

void generateMap(Map<int, string>& encodingMap, HuffmanNode* encodingTree, string currentString);
void decodeDataHelper(ibitstream& input, HuffmanNode* root, HuffmanNode* current, int bit, ostream& output);

/*
 * Intakes an istream, then counts and returns a mapping from each character to the
 * number of times that character appears in the file. Adds a single occurrence of
 * the fake character PSEUDO_EOF to the map as well.
 * Assumes the input file exists and can be read, even if empty.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while (input.good()) {
        int ch = input.get();
        if (input.good()) {
            if (freqTable.containsKey(ch)) {
                int counter = freqTable.get(ch);
                counter++;
                freqTable.remove(ch);
                freqTable.put(ch, counter);
            } else {
                freqTable.put(ch, 1);
            }
        }
    }
    freqTable.put(PSEUDO_EOF, 1);
    return freqTable;
}

/*
 * Intakes a frequency table, such as the one built in buildFrequencyTable(), and
 * uses it to build a Huffman encoding tree based on those counts. Returns a pointer
 * to the node at the root of the tree.
 * Assumes the frequency table is valid, as in it does not contain keys other than
 * char values, PSEUDO_EOF, and NOT_A_CHAR, all counts are positive integers, and it
 * contains at least one key/value pairing.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    for (int key : freqTable) {
        int count = freqTable.get(key);
        HuffmanNode* character;
        character = new HuffmanNode(key, count, NULL, NULL);
        pq.enqueue(character, character->count);
    }
    while (pq.size() > 1) {
        HuffmanNode* rarest = pq.dequeue();
        HuffmanNode* secondRarest = pq.dequeue();
        int count = rarest->count + secondRarest->count;
        HuffmanNode* combinedNode = new HuffmanNode(NOT_A_CHAR, count, rarest, secondRarest);
        pq.enqueue(combinedNode, combinedNode -> count);
    }
    HuffmanNode* root = pq.dequeue();
    return root;
}

/*
 * Intakes a pointer to the root node of a Huffman tree, such as the one built in
 * buildEncodingTree(), and returns a Huffman encoding map based on the tree's
 * structure.
 * Returns an empty map if the encoding tree is NULL.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string currentString = "";
    generateMap(encodingMap, encodingTree, currentString);
    return encodingMap;
}

/*
 * Helper function that intakes the map, a pointer to the root node of a Huffman tree, and a string,
 * and returns the actual map after recursively moving through the Huffman tree.
 */
void generateMap(Map<int, string>& encodingMap, HuffmanNode* encodingTree, string currentString) {
    if (encodingTree -> character != NOT_A_CHAR) {
        encodingMap.put(encodingTree -> character, currentString);
    } else {
        generateMap(encodingMap, encodingTree -> zero, currentString + '0');
        generateMap(encodingMap, encodingTree -> one, currentString + '1');
    }
}

/*
 * Reads one character at a time from a given input file and uses the encoding map passed through
 * the parameters to encode each character to binary. Writes the characters encoded binary bits
 * to the provided outbit bit stream passed through the parameters.
 * Assumes the encoding map is valid and contains all needed data, the input stream is readable,
 * and the output sream is writable.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    string encodedLine = "";
    while(input.good()) {
        int ch = input.get();
        if (input.good()) {
            encodedLine = encodedLine + encodingMap.get(ch);
        }
    }
    for (int i = 0; i < encodedLine.length(); i++) {
        output.writeBit(encodedLine[i] - '0');
    }
    encodedLine = "";
    encodedLine = encodingMap.get(PSEUDO_EOF);
    for (int i = 0; i < encodedLine.length(); i++) {
        output.writeBit(encodedLine[i] - '0');
    }
}

/*
 * Reads bits from the input file passed through the parameters one at a time and then
 * traverses through the provided Huffman encoding tree to write the original
 * uncompressed contents of the file to the output stream passed through the parameters.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int bit;
    HuffmanNode* current = encodingTree;
    if ((bit = input.readBit()) != -1) {
        decodeDataHelper(input, encodingTree, current, bit, output);
    }
}

/*
 * Performs the recursive action of decoding the data.
 */
void decodeDataHelper(ibitstream& input, HuffmanNode* root, HuffmanNode* current, int bit, ostream& output) {
    if (current -> isLeaf()) {
        if (current -> character == PSEUDO_EOF) return;
        output.put(current -> character);
        current = root;
    }
    if (bit == 0) {
        if ((bit = input.readBit()) != -1) {
            decodeDataHelper(input, root, current->zero, bit, output);
        }
    } else if (bit == 1) {
        if ((bit = input.readBit()) != -1) {
            decodeDataHelper(input, root, current->one, bit, output);
        }
    }
}

/*
 * Intakes an istream and an obitstream and combines previous functions
 * in order to compress a file. More specifically it reads the input file
 * one character at a time, builds an encoding of its contents, and writes a
 * compressed version of the file to the specified output file.
 * Assumes the streams are valid and read/writeable.
 */
void compress(istream& input, obitstream& output) {
    Map<int, int> frequencyTable = buildFrequencyTable(input);
    output << frequencyTable;
    input.clear();
    input.seekg(0, ios::beg);
    HuffmanNode* encodingTree = buildEncodingTree(frequencyTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

/*
 * Intakes an ibitstream and ostream and combines previous functions in order
 * to decompress a file. More specifically it reads the bits from the input
 * file one at a time to write the original contents of the file to the file
 * specified by the output parameter.
 * Assumes the streams are valid and read/writeable.
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> frequencyTable;
    input >> frequencyTable;
    HuffmanNode* encodingTree = buildEncodingTree(frequencyTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

/*
 * Frees memory associated with the tree whose root node is represented by the pointer.
 */
void freeTree(HuffmanNode* node) {
    if (node != NULL) {
        freeTree(node->zero);
        freeTree(node->one);
    } else {
        delete node;
    }
}
