#include "encoding.h"
#include "bitstream.h"
#include "pqueue.h"
#include "HuffmanNode.h"
#include "strlib.h"
#include "console.h"

Map<int, string> buildEncodingMapHelper(HuffmanNode* encodingTree, string code, Map<int, string>& map);
int decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output, HuffmanNode* current, int bit);

// Read input from a given istream
// Count and return a mapping from each character to the number of times that character appears in the file
// A single occurrence of the fake character PSEUDO_EOF is added into your map.
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    int next_char = input.get();
    while (next_char != -1) {
        if (freqTable.containsKey(next_char)) {
            freqTable[next_char] = freqTable[next_char] + 1;
        } else {
            freqTable[next_char] = 1;
        }
        next_char = input.get();
    }
    freqTable[PSEUDO_EOF] = 1;
    return freqTable;
}

// Accept a frequency table and use it to create a Huffman encoding tree based on those frequencies.
// Return a pointer to the node representing the root of the tree.
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    for (int ch : freqTable) {
        HuffmanNode* node = new HuffmanNode(ch, freqTable[ch], NULL, NULL);
        pq.enqueue(node, freqTable[ch]);
    }
    HuffmanNode* root;
    if (pq.size() == 1) {
        root = pq.dequeue();
    } else {
        while (pq.size() > 1) {
            HuffmanNode* leftNode = pq.dequeue();
            HuffmanNode* rightNode = pq.dequeue();
            HuffmanNode* notCharNode = new HuffmanNode(NOT_A_CHAR, rightNode->count + leftNode->count, leftNode, rightNode);
            pq.enqueue(notCharNode, notCharNode->count);
        }
        root = pq.dequeue();
    }
    return root;
}

// Calls the helper function to build the encoding map.
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    string code = "";
    Map<int, string> map;
    map = buildEncodingMapHelper(encodingTree, code, map);
    return map;
}

// Accept a pointer to the root node of a Huffman tree and use it to create and return a Huffman encoding map based on the tree's structure.
// Each key in the map is a character, and each value is the binary encoding for that character represented as a string.
Map<int, string> buildEncodingMapHelper(HuffmanNode* current, string code, Map<int, string>& map) {
    if (current->isLeaf()) {
        map[current->character] = code;
        return map;
    } else {
        map = buildEncodingMapHelper(current->zero, code + integerToString(0), map);
        map = buildEncodingMapHelper(current->one, code + integerToString(1), map);
    }
    return map;
}

// Read one character at a time from a given input file,
// and use the provided encoding map to encode each character to binary,
// then write the character's encoded binary bits to the given bit output bit stream.
// A single occurrence of the binary encoding for PSEUDO_EOF is added into the output.
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int next_char = input.get();
    while (next_char != -1) {
        if (encodingMap.containsKey(next_char)) {
            string binary = encodingMap[next_char];
            for (int i = 0; i < binary.size(); i++) {
                output.writeBit(stringToInteger(charToString(binary[i])));
            }
        }
        next_char = input.get();
    }
    string eof = encodingMap[PSEUDO_EOF];
    for (int i = 0; i < eof.size(); i++) {
        output.writeBit(stringToInteger(charToString(eof[i])));
    }
}

// Calls the helper function to decode the data.
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* current = encodingTree;
    int bit = input.readBit();
    while (bit != -1) {
        int ch = decodeDataHelper(input, encodingTree, output, current, bit);
        if (ch == PSEUDO_EOF) {
            break;
        } else {
            output.put(ch);
            bit = input.readBit();
        }
    }
}


// Read bits from the given input file one at a time, and recursively walk through the specified
// decoding tree to write the original uncompressed contents of that file to the given output stream.
int decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output, HuffmanNode* current, int bit) {
    if (bit == 0) {
        current = current->zero;
    } else if (bit == 1) {
        current = current->one;
    }
    if (current->isLeaf()) {
        return current->character;
    } else {
        bit = input.readBit();
        return decodeDataHelper(input, encodingTree, output, current, bit);
    }
}

// This is the overall compression function;
// in this function the given input file is compressed into the given output file.
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    input.clear();             // removes any current eof/failure flags
    input.seekg(0, ios::beg);  // tells the stream to seek back to the beginning
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    output << freqTable;
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

// This function should do the opposite of compress;
// it reads the bits from the given input file one at a time,
// including header packed inside the start of the file, to write the original contents of that file to the file specified by the output parameter.
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

// This function frees the memory associated with the tree whose root node is represented by the given pointer.
void freeTree(HuffmanNode* node) {
    if (node != NULL) {
        if (node->isLeaf()) {
            delete node;
        } else {
            HuffmanNode* leftNode = node->zero;
            HuffmanNode* rightNode = node->one;
            delete node;
            freeTree(leftNode);
            freeTree(rightNode);
        }
    }
}



