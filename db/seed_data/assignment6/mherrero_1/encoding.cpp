/* Filename: Encoding.cpp - Huffman Encoding Assignment
 * -------------------------------------------------------------------------------------------------------------
 * This program uses the Huffman encoding algorithm described in the Assignment 6 spec sheet to read a file or
 * a string and encode/compress the data. The program can also decompress or decode a string of Huffman
 * encoded bits (provided it has the tree that was used to encode it) or a properly formatted .huf file.
 */
#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"

/*
 * This function builds a freqency table of the characters it is fed from an input stream. It first declares a map
 * that will hold the ascii values of each character in the file paired with the frequency of that character. Then,
 * while the input stream still has value values to return (input.peek() != -1), the next ascii value is retrieved
 * from the map, and its frequency count is incremented in the table. After this loop completes, our end of file
 * marker "PSEUO_EOF" is added to the map with a frequency of 1. The map is then returned by the function.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while(input.peek() != -1) {
        freqTable[input.get()]++;
    }
    freqTable[256]++;
    return freqTable;
}

/*
 * This function uses the frequency table that it is passed to build an encoding tree. This tree will be used to
 * find the appropriate binary representations for each ascii character in the file, as well as for decoding
 * a string of binary, Huffman-encoded bits. This method first creates a priority queue of Huffman nodes, fills
 * this queue with the data supplied in the frequency Table, then builds the tree using this priority queue.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> nodeQueue;
    fillPriorityQueue(freqTable, nodeQueue);
    return buildTree(nodeQueue);
}

/*
 * This method fills a priority queue of HuffmanNode pointers with the table from a frequency table of characters
 * in a file. First, the method iterates over every key in the frequency table map and adds a pointer to a new
 * HuffmanNode object housing the character's ascii value and the character's frequency, and then pairs this pointer
 * in the priority queue with the characters frequency.
 */
void fillPriorityQueue(const Map<int, int>& freqTable, PriorityQueue<HuffmanNode*>& nodeQueue) {
    for(int asciiVal: freqTable.keys()) {
        nodeQueue.enqueue(new HuffmanNode(asciiVal, freqTable[asciiVal]), freqTable[asciiVal]);
    }
}

/*
 * This method recursively handles a priority queue of HuffmanNodes with their respective frequency to build an
 * encoding tree. It does this by removing the front two nodes in the priority queue, which will be the two
 * highest priority (lowest frequency) nodes, and creating a new node that points toward each of the two lesser
 * nodes, stores NOT_A_CHAR as its character value, and its count as the sum of the counts of the two nodes.
 * The method then returns the result of the next recursive calls. If the method is called with only one node in
 * the priority queue, we know that the tree is complete. A root pointer is declared and returned. If the priority
 * queue is passed with no pointers inside, then they method simply returns null.
 */
HuffmanNode* buildTree(PriorityQueue<HuffmanNode*>& nodeQueue) {
    if(nodeQueue.size() == 0){
        return NULL;
    }
    if(nodeQueue.size() == 1){
        HuffmanNode* root = nodeQueue.dequeue();
        return root;
    }
    HuffmanNode* node1 = nodeQueue.dequeue();
    HuffmanNode* node2 = nodeQueue.dequeue();
    nodeQueue.enqueue(new HuffmanNode(NOT_A_CHAR,(node1->count) + (node2->count), node2, node1),
                      (node1->count) + (node2->count));
    return buildTree(nodeQueue);
}

/*
 * This method builds the encoding map storing each character in the file with its equivalent Huffman binary
 * representation by iterating through the encoding tree to build the binary strings. After it does this, it
 * returns a map of int ascii values to strings of binary representations.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    buildEncodingMapHelper(encodingTree, encodingMap);
    return encodingMap;
}

/*
 * This method recursively builds an encoding map after being passed an encoding tree and a reference to the
 * location of a map to write to. The method is also passed a string of the running representation (path the
 * recursive calls have taken through the tree so far) which has a default value of "". First, the method checks
 * if the node that is passed to the method contains an acutal character. If it does, the character is added to
 * the encoding Map with the current binary representation as its binary value, and the method returns. If this
 * is not true, then the method is called on the left substring of the node with a zero appended to the
 * currentEncode string and the method is called on the right substring of the node with a one appended to the
 * currentEncode string.
 */
void buildEncodingMapHelper(HuffmanNode* node, Map<int, string>& encodingMap, string currentEncode) {
    if(node->character != NOT_A_CHAR) {
        encodingMap[node->character] = currentEncode;
        return;
    }
    buildEncodingMapHelper(node->zero, encodingMap, currentEncode + "0");
    buildEncodingMapHelper(node->one, encodingMap, currentEncode + "1");
}

/*
 * This method takes an input stream from a file, a map of the characters in that file and their equivalent
 * Huffman binary representations, and an output bitstream, and uses these to print out a Huffman encoded version
 * of the file. It reads the file character by character, and while it is not at the end of the file, it gets the
 * characters binary string, iterates through each 1 or 0 in the string, and prints it out to the bitstream. When
 * iterating through the binary string, 48 is subtracted from each character's ascii value, which will result in
 * the int 0 if the char is '0' and 1 if the char is '1'. After this loop executes, the binary representation of
 * PSEUDO_EOF is written to the bitstream.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while(input.peek() != -1) {
        string binary = encodingMap[input.get()];
        for(int i = 0; i < binary.size(); i++) {
            output.writeBit((int)binary[i] - 48);
        }
    }
    for(int i = 0; i < encodingMap[PSEUDO_EOF].size(); i++) {
        output.writeBit((int) encodingMap[PSEUDO_EOF][i] - 48);
    }
}

/*
 *This method recieves an input bitstream reading the bits of a .huf file, an encoding tree made from the
 * frequency table header of the .huf file, and an output stream and decodes the file using the recursive
 * decodeDataHelper function. In order to terminate the calling of this function once it the helper function
 * has found the PSEUDO_EOF value, the method call is placed in a while true loop. Then, the call of the method
 * is placed within an if statement, which ends the while true loop when the method returns true. The method
 * returns true once it has found the PSEUDO_EOF constant.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    while(true) {
        if(decodeDataHelper(input, encodingTree, output)) break;
    }
}

/*
 * This is the recursive helper method for the decodeData function. It is passed a bitstream input reading a .huf
 * file, a node in the tree (initially the root of the tree), an output stream, and a string of the running binary
 * representation of the next byte that will be found (default value of "") first, the method checks if the node
 * that is currently being passed contains the PSEUDO_EOF value, and returns true. Then, if the method has not
 * found the PSEUDO_EOF value and the node contains a character, then the character is written to the output and
 * the method returns false. If neither of these are true, then the method reads the next bit from the input. If
 * the nextBit retrieved is a 1, then the next method call is called on the one pointer of the node. If the
 * nextBit is a 0, then the method is called on the 0 pointer of the node. In both of these calls, the nextBit
 * is added to the end of the runningByte.
 */
bool decodeDataHelper(ibitstream& input, HuffmanNode* currentNode, ostream& output, string runningByte) {
    if(currentNode->character == PSEUDO_EOF) {
        return true;
    }
    if(currentNode->character != NOT_A_CHAR) {
        output << (char) currentNode->character;
        return false;
    } else {
        int nextBit = input.readBit();
        if(nextBit == 1) {
            return decodeDataHelper(input, currentNode->one, output, runningByte + integerToString(nextBit));
        } else {
            return decodeDataHelper(input, currentNode->zero, output, runningByte + integerToString(nextBit));
        }
    }
}

/*
 * This method is passed an input stream reading a file and an output bitstream and uses the entire Huffman
 * encoding algorithm to compress the the file into a new .huf file. It first creates a frequency table using
 * the buildFrequencyTable method, then it uses this table to create a encoding tree using the buildEncodingTree
 * method, then it uses this tree to build an encoding Map using the buildEncodingMap method. After this, it rewinds
 * the input stream to the beginning so that the encodeData method can read the characters of the file from the start.
 * it then prints the frequency table to the output and calls the encodeData method the encode the characters in
 * the file. After this, the memory allocated to the tree is freed using the freeTree method.
 */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* root = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(root);
    rewindStream(input);
    output << freqTable;
    encodeData(input, encodingMap, output);
    freeTree(root);
}

/*
 * This method recieves an bitstream input reading a .huf file and a output stream and uses these to decompress a
 * .huf file into its original state. It first reads out the frequency table that is stored as a header in the .huf
 * file and uses it to build a new encoding tree. After it does this, it passes the input, the root of the encoding
 * tree, and the output to the decodeData method to decode the binary in the file. After this occurs, the memory
 * allocated to the tree is freed using the freeTree method.
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* root = buildEncodingTree(freqTable);
    decodeData(input, root, output);
    freeTree(root);
}

/*
 * This method recursively frees the memory allocated to an encoding tree. While the root passed to the method is
 * not NULL, the method frees the subtree to the right of the node, then the subtree to the left of the node, then
 * deletes the node.
 */
void freeTree(HuffmanNode* node) {
    if(node != NULL) {
        freeTree(node->one);
        freeTree(node->zero);
        delete node;
    }
}
