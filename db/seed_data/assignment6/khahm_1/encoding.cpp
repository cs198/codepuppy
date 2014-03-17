/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * Huffman Encoding cpp file
 * Compresses the given file into a smaller file
 * *** functions ***
 * 1. buildFrequencyTable: builds a map to store the code to encode
 * 2. buildEncodingTree: builds a tree to translate the code to encode
 * 3. buildEncodingMap: builds a map translating each character to bits
 * 4. encodeData: encodes the given file using huffman encoding algorithm
 * 5. decodeData: decodes the given file using huffman decoding algorithm
 * 6. compress: compresses the given file using huffman algorithm
 * 7. decompress: decompresses the given file using reverse huffman algorithm
 * 8. freeTree: frees the memory associated with building the encoding tree
 * (took two late days on this assignment)
 */

#include "encoding.h"
#include "pqueue.h"
#include "HuffmanNode.h"
#include "bitstream.h"
#include "filelib.h"
#include "strlib.h"

/*
 * parameter: input stream
 * returns: frequency table map
 * This function counts the number of occurances of each character in the file
 * and returns a map of those counts, including the EOF count.
 * precondition: input file exists and can be read
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    int character = input.get();
    while (character != -1) {
        if (freqTable.containsKey(character)) {
            int tempValue = freqTable.get(character);
            tempValue++;
            freqTable.put(character, tempValue);
        }
        else {
            freqTable.put(character, 1);
        }
        character = input.get();
    }
    freqTable.put(PSEUDO_EOF, 1);
    return freqTable;
}

/*
 * parameter: frequency table map
 * returns: pointer to the root of the encoding tree
 * This method creates an encoding binary tree based on the given map.
 * It uses a priority queue to count which node should be processed next.
 * precondition: frequency table is completely valid
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    // builds the priority queue of huffman nodes
    for (int key : freqTable.keys()) {
        int priority = freqTable[key];
        HuffmanNode* temp = new HuffmanNode(key, priority);
        pq.enqueue(temp, priority);
    }
    // builds the encoding tree from the priority queue
    while (pq.size() > 1) {
        HuffmanNode* firstPoint = pq.dequeue();
        int combineFreq = firstPoint->count;
        HuffmanNode* secondPoint = pq.dequeue();
        combineFreq += secondPoint->count;
        HuffmanNode* parentPoint = new HuffmanNode(NOT_A_CHAR, combineFreq);
        parentPoint->zero = firstPoint;
        parentPoint->one = secondPoint;
        pq.enqueue(parentPoint, combineFreq);
    }
    HuffmanNode* root = pq.dequeue();
    return root;
}

/*
 * Helper function for buildEncodingMap
 * parameter: pointer to the encoding tree, encoding map, current string of bits
 * This function recursively organizes the tree into an encoding map
 */
void encodingMapHelper(HuffmanNode* encodingTree, Map<int, string>& encodingMap, string current) {
    if (encodingTree != NULL) {
        if (encodingTree->character != NOT_A_CHAR) {
            encodingMap.put(encodingTree->character, current);
        }
        encodingMapHelper(encodingTree->zero, encodingMap, current + "0");
        encodingMapHelper(encodingTree->one, encodingMap, current + "1");
    }
}

/*
 * parameter: root of encoding tree
 * returns: map of all characters to its new code
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    if (encodingTree != NULL) {
        if (encodingTree->character == PSEUDO_EOF) {
            encodingMap.put(encodingTree->character, "");
        }
        else {
            encodingMapHelper(encodingTree->zero, encodingMap, "0");
            encodingMapHelper(encodingTree->one, encodingMap, "1");
        }
    }
    return encodingMap;
}

/*
 * Helper function to encodeData
 * parameter: string containing the bit code of the character, output stream
 * This function writes the corresponding bit code to the output stream.
 */
void writeCharToOutput(string& bitValue, obitstream& output) {
    for (int i = 0; i < bitValue.size(); i++) {
        int num = stringToInteger(bitValue.substr(i,1));
        output.writeBit(num);
    }
}

/*
 * parameter: input stream, encoding map, output stream
 * This function reads each character from the input file and uses the encoding map
 * to encode each character to binary bits and writes them into the output stream.
 * precondition: parameters are all valid,
 * input/output stream are opened, readable, and writeable.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int character = input.get();
    while (character != -1) {
        string bitValue = encodingMap.get(character);
        writeCharToOutput(bitValue, output);
        character = input.get();
    }
    string bitValue = encodingMap.get(PSEUDO_EOF);
    writeCharToOutput(bitValue, output);
}

/*
 * Helper function for decodeData
 * parameter: input stream, pointer to root of encoding tree,
 * pointer to current position of encoding tree, output stream
 * This function recursively traverses through the tree until it finds
 * a character that the input stream has been referring to.
 * This process is repeated until each bit in the input stream has been converted.
 */
void decodeHelper(bool& isEnd, ibitstream& input, HuffmanNode* encodingTree, HuffmanNode* current, ostream& output) {
    if (current->isLeaf()) {
        int currentChar = current->character;
        if (currentChar != PSEUDO_EOF) {
            output.put(currentChar);
        }
        else {
            isEnd = true;
        }
    }
    else {
        int newBit = input.readBit();
        if (newBit == 0) {
            current = current->zero;
        }
        else if (newBit == 1){
            current = current->one;
        }
        decodeHelper(isEnd, input, encodingTree, current, output);
    }
}

/*
 * parameter: input stream, pointer to root of the encoding tree, output stream
 * This function reads bits from the input file and uses the tree to write the
 * original file to the output stream.
 * precondition: streams are opened/closed
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    bool isEnd = false;
    while (input.good() && !isEnd) {
        decodeHelper(isEnd, input, encodingTree, encodingTree, output);
    }
}

/*
 * parameter: input stream, output stream
 * This function compresses the input file into the output file
 * The output file includes a header of the frequency table
 * precondition: input/output streams are valid, readable, writeable, opened/closed automatically
 */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* root = buildEncodingTree(freqTable);
    Map<int, string> encodeMap = buildEncodingMap(root);
    freeTree(root);
    rewindStream(input);
    output << freqTable;
    encodeData(input, encodeMap, output);
}

/*
 * parameter: input stream, output stream
 * This function decompresses the input file to the output file
 * The input file includes a header of the frequency table, which we extract.
 * precondition: input/output streams are valid, readable, writeable, opened/closed automatically
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    rewindStream(input);
    input >> freqTable;
    HuffmanNode* root = buildEncodingTree(freqTable);
    decodeData(input, root, output);
    freeTree(root);
}

/*
 * parameter: pointer to the root of the encoding tree
 * This function frees the memory associated with the encoding tree
 */
void freeTree(HuffmanNode* node) {
    HuffmanNode* trash = node;
    if (node->zero != NULL) {
        HuffmanNode* zeroNode = node->zero;
        freeTree(zeroNode);
    }
    if (node->one != NULL) {
        HuffmanNode* oneNode = node->one;
        freeTree(oneNode);
    }
    delete trash;
}
