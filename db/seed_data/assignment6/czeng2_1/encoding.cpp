/* Connie Zeng
 * CS 106B, Winter 2014
 * -------------------------------
 * Assignment 6: Huffman Encoding
 * This program encodes and decodes data from an input file using the Huffman
 * algorithm. This allows the file to be compressed and stored in a smaller
 * amount of memory.
 */

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"

/* Function prototypes */

void addChar(Map<int, string>& map, HuffmanNode* node, string code);
int findChar(ibitstream& input, HuffmanNode* node);

/* Function: buildFrequencyTable
 * Usage: Map<int, int> freqTable = buildFrequencyTable(input);
 * -------------------------------------------------------------
 * This function reads the passed input and returns a map with the number of
 * times each character appears in the input. The map also includes a single
 * count of the PSEUDO_EOF character to represent the end of the input. Assumes
 * that the input file exists and is readable.
 */

Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while(true){
        int ch = input.get();
        if(ch == -1){
            freqTable.put(PSEUDO_EOF, 1);
            break;
        }

        // Counts occurrences of each character in input
        if(freqTable.containsKey(ch)){
            freqTable[ch]++;
        }
        else{
            freqTable.put(ch, 1);
        }
    }
    return freqTable;
}

/* Function: buildEncodingTree
 * Usage: HuffmanNode* node = buildEncodingTree(freqTable);
 * ---------------------------------------------------------
 * This function creates a Huffman encoding tree, based on the passed frequency
 * table, and returns a pointer to the root of the tree. Assumes that the map
 * is non-empty, and all keys are either char values, PSEUDO_EOF, or NOT_A_CHAR.
 */

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    // Creates priority queue to store Huffman nodes in increasing order of frequency
    PriorityQueue<HuffmanNode*> charQueue;
    for(int ch : freqTable){
        charQueue.enqueue(new HuffmanNode(ch, freqTable[ch]), freqTable[ch]);
    }

    // Removes two nodes at a time to re-enqueue under a parent node
    while(charQueue.size() > 1){
        HuffmanNode* lnode = charQueue.dequeue();
        HuffmanNode* rnode = charQueue.dequeue();
        int count = lnode->count + rnode->count;

        HuffmanNode* newNode = new HuffmanNode(NOT_A_CHAR, count);
        newNode->zero = lnode;
        newNode->one = rnode;
        charQueue.enqueue(newNode, count);
    }
    return charQueue.peek();
}

/* Function: buildEncodingMap
 * Usage: Map<int, string> encodingMap = buildEncodingMap(encodingTree);
 * ----------------------------------------------------------------------
 * This function determines the binary code for each character, based on the
 * passed Huffman encoding tree, and returns a map with the binary codes.
 */

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    addChar(encodingMap, encodingTree, "");
    return encodingMap;
}

/* Function: encodeData
 * Usage: encodeData(input, encodingMap, output);
 * -----------------------------------------------
 * This function reads the passed input one character at a time and writes the
 * character's binary code to the given output, based on the passed encoding
 * map. The encoding also includes the binary code for the PSEUDO_EOF character
 * to indicate the end of the data. Assumes that the input/output streams are
 * open and able to be read/written.
 */

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    rewindStream(input);
    while(!input.fail()){
        // Read input
        int ch = input.get();

        // Convert character to code
        string code;
        if(ch == -1){
            code = encodingMap[PSEUDO_EOF];
        }
        else{
            code = encodingMap[ch];
        }

        // Write output
        for(int i = 0; i < code.length(); i++){
            output.writeBit(code[i] - '0');
        }
    }
}

/* Function: decodeData
 * Usage: decodeData(input, encodingTree, output);
 * ------------------------------------------------
 * This function reads the passed input one bit at a time and writes the coded
 * character to the given output, based on the passed Huffman encoding tree.
 * Assumes that the input/output streams are open and able to be read/written.
 */

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    while(true){
        int ch = findChar(input, encodingTree);
        if(ch == PSEUDO_EOF) break;
        output.put(ch);
    }
}

/* Function: compress
 * Usage: compress(input, output);
 * --------------------------------
 * This function compresses the passed input file into the given output file
 * and adds a header to allow the file to be decompressed later. Assumes that
 * the input/output streams are open and able to be read/written.
 */

void compress(istream& input, obitstream& output) {
    Map<int, int> header = buildFrequencyTable(input);
    output << header;

    HuffmanNode* encodingTree = buildEncodingTree(header);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

/* Function: decompress
 * Usage: decompress(input, output);
 * ----------------------------------
 * This function decompresses the passed input file to the given output file
 * using the header at the start of the file. Assumes that the input/output
 * streams are open and able to be read/written.
 */

void decompress(ibitstream& input, ostream& output) {
    Map<int, int> header;
    input >> header;

    HuffmanNode* encodingTree = buildEncodingTree(header);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

/* Function: freeTree
 * Usage: freeTree(node);
 * -----------------------
 * This function frees the memory allocated to the tree that the passed
 * node points to.
 */

void freeTree(HuffmanNode* node) {
    if(node != NULL){
        // Free leaf
        if(node->zero == NULL && node->one == NULL){
            HuffmanNode* trash = node;
            node = NULL;
            delete trash;
        }

        // Traverse subtrees
        else{
            freeTree(node->zero);
            freeTree(node->one);
        }
    }
}

/* Function: addChar
 * Usage: addChar(map, node, code);
 * ---------------------------------
 * This function determines the binary code for each character. It traverses
 * the Huffman encoding tree and adds to the code string accordingly until
 * reaching a leaf node, and then adds the binary code to the passed map.
 * A helper function for buildEncodingMap.
 */

void addChar(Map<int, string>& map, HuffmanNode* node, string code){
    if(node != NULL){
        if(node->isLeaf()){
            map.put(node->character, code);
        }
        // Appends 0 or 1 to code string so far
        else{
            addChar(map, node->zero, code + "0");
            addChar(map, node->one, code + "1");
        }
    }
}

/* Function: findChar
 * Usage: int ch = findChar(input, node);
 * ---------------------------------------
 * This function determines the character encoded for by the binary code in the
 * input. It reads the input one bit at a time and moves down the Huffman
 * encoding tree accordingly until reaching a leaf node, and then returns the
 * character stored in that node. A helper function for decodeData.
 */

int findChar(ibitstream& input, HuffmanNode* node){
    if(node == NULL){
        throw "Error: File encoding does not match tree.";
    }
    if(node->isLeaf()){
        return node->character;
    }
    int bit = input.readBit();
    if(bit == 0){
        return findChar(input, node->zero);
    }
    else if(bit == 1){
        return findChar(input, node->one);
    }
}
