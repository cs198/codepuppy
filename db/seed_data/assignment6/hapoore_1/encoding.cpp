// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"

void buildEncodingMapHelper(HuffmanNode* encodingTree, Map<int, string>& encodingMap, string& path);
void writeBits(string& encoded, obitstream& output);
int decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output, HuffmanNode* current);
void extractFrequencyTable(ibitstream& input, Map<int, int>& newFreqTable);
string readHeader(ibitstream& input);

/* This function reads data from an input stream and returns a
 * map from characters to their frequencies.
 */

Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while (true) {
        int next = input.get();
        if(input.fail()) {
            break;
        }
        if(freqTable.containsKey(next)) {
            int currFreq = freqTable[next];
            currFreq++;
            freqTable.put(next, currFreq);
        } else {
            freqTable.put(next, 1);
        }
    }
    freqTable.put(PSEUDO_EOF, 1); // Add PSEUDO_EOF to the map once at the end
    return freqTable;
}

/* This function builds a binary encoding tree using a frequency table.
 * The algorithm uses a priority queue to build the tree by combining
 * pairs of nodes into a single node and then reinserting the new node
 * into the queue. This continues until there is only one node remaining
 * in the queue. The function returns the pointer to this node.
 */


HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> treeBuilder;
    for(int character: freqTable) { // Make a node for each character in the table and enqueue.
        int currFreq = freqTable[character];
        HuffmanNode* newNode = new HuffmanNode(character, currFreq);
        treeBuilder.enqueue(newNode, currFreq);
    }
    while(treeBuilder.size() > 1) { // Combine the two nodes in the front of the queue. Re-add to the queue.
        HuffmanNode* first = treeBuilder.dequeue();
        HuffmanNode* second = treeBuilder.dequeue();
        int totCount = first->count + second->count;
        HuffmanNode* combined = new HuffmanNode(NOT_A_CHAR, totCount, first, second);
        treeBuilder.enqueue(combined, totCount);
    }
    HuffmanNode* front = treeBuilder.dequeue(); // Return the last pointer in the queue.
    return front;
}

/* This function uses a Huffman encoding tree to generate a map from characters
 * to their binary representations from the tree. It accepts a pointer to the top of the tree.
 */

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string path;
    buildEncodingMapHelper(encodingTree, encodingMap, path);
    return encodingMap;
}

/* This recursive helper function gets the encoding map passed to it by reference and adds all of the key-value pairs.
 * It takes as parameters a pointer to the start of the encoding tree, a reference to the encoding map, and a reference
 * to a string representing the path currently being traveled down the tree, which gets reset to empty whenever a character
 * is reached and added to the map.
 */

void buildEncodingMapHelper(HuffmanNode* encodingTree, Map<int, string>& encodingMap, string& path) {
    if(encodingTree != NULL) {
        if(encodingTree->isLeaf()) {
            encodingMap.put(encodingTree->character, path);
        } else {
            path += integerToString(0); //Choose
            buildEncodingMapHelper(encodingTree->zero, encodingMap, path); //Explore
            path = path.substr(0, path.size() - 1); //Unchoose
            path += integerToString(1); //Choose
            buildEncodingMapHelper(encodingTree->one, encodingMap, path); //Explore
            path = path.substr(0, path.size() - 1); //Unchoose
        }
    }
}

/* This function takes a reference to an input stream, a reference to an encoding map, and a reference
 * to an output stream for bits. It uses the encoding map to encode the data it reads from the imput stream,
 * and then writes the encoded data to the output stream bit by bit.
 */

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    rewindStream(input);
    while(true) {
        int next = input.get();
        if(input.fail()) {
            break;
        }
        string encoded = encodingMap[next];
        writeBits(encoded, output);
    }
    string endFile = encodingMap[PSEUDO_EOF];
    writeBits(endFile, output);
}

/* This function takes in a reference to a string of zeros and ones and a reference to an
 * output bitstream and then goes through the string character by character, writing bits
 * as they appear in the string (as either zeros or ones).
 */

void writeBits(string& encoded, obitstream& output) {
    for(int i = 0; i < encoded.size(); i++) {
        if(encoded[i] == '0') {
            output.writeBit(0);
        }
        if(encoded[i] == '1') {
            output.writeBit(1);
        }
    }
}

/* This method takes an input stream of bits, a pointer to an encoding tree, and a reference to an
 * output stream. It calls a recursive helper function to decode the bits from the input stream and
 * write the desired characters to the output stream. It turns a .huf file back into what it was before.
 */

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* current = encodingTree;
    int nextChar;
    while(!input.fail()) {
        nextChar = decodeDataHelper(input, encodingTree, output, current);
        if(nextChar == PSEUDO_EOF) break; // Don't want extraneous characters from extra zeros at the end of the last byte.
    }
}

/* This recursive helper function takes an input stream by reference, two pointers to the encoding tree, and a reference to
 * an output stream. It reads the bits from the input stream and walks the tree based on those bits until it reaches a leaf,
 * at which point it writes the proper character to the output stream. This function returns the integer representation of the
 * characters it adds to the output stream.
 */

int decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output, HuffmanNode* current) {
    if(current->isLeaf()) {
        if(current->character != PSEUDO_EOF) {
            output.put(current->character);
        }
        return current->character;
    } else {
        int nextBit = input.readBit();
        if(nextBit == 0) {
            current = current->zero;
        } else {
            current = current->one;
        }
        return decodeDataHelper(input, encodingTree, output, current);
    }
}

/* This function takes an input stream and an output stream, both by reference. It calls various
 * methods detailed earlier in the file to compress the data from the input stream and write the result
 * to the output stream. It also frees the memory from the encoding tree when finished. It also writes
 * a header at the top of the file to allow for the file to be decoded without being passed the encoding tree
 * directly.
 */

void compress(istream& input, obitstream& output) {
    Map<int, int> frequencies = buildFrequencyTable(input);
    output << frequencies; // Write the header so that the file can be decoded.
    HuffmanNode* topTree = buildEncodingTree(frequencies);
    Map<int, string> encodingMap = buildEncodingMap(topTree);
    encodeData(input, encodingMap, output);
    freeTree(topTree);
}

/* This function takes an input stream and an output stream, both by reference. It calls various
 * methods detailed earlier in the file to decompress the data from the input stream and write the result
 * to the output stream. It also frees the memory from the decoding tree when finished.
 */

void decompress(ibitstream& input, ostream& output) {
    Map<int, int> newFreqTable;
    extractFrequencyTable(input, newFreqTable);
    HuffmanNode* decodingTree = buildEncodingTree(newFreqTable);
    decodeData(input, decodingTree, output);
    freeTree(decodingTree);
}

/* This function recreates the frequency table from the header in the compressed file. It first
 * builds a string made up of the header. It then goes through the string and extracts the characters
 * and their frequencies and stores them to the Map newFrequencies, which is passed in as a reference parameter.
 */

void extractFrequencyTable(ibitstream& input, Map<int, int>& newFreqTable) {
    string result = readHeader(input);
    string newResult;
    int nextChar;
    int nextCount;
    for(int i = 0; i < result.size(); i++) {
        int curr = result[i];
        if(curr != ' ' && curr != '{') { // Don't want to add these characters to the map.
            if(curr == ':') { // newResult now represents a character
                nextChar = stringToInteger(newResult);
                newResult = "";
            } else if(curr == ',' || curr == '}') { // newResult now represents a count. Can now add a pair to the map.
                nextCount = stringToInteger(newResult);
                newFreqTable.put(nextChar, nextCount);
                newResult = "";
            } else {
                newResult += curr; // Not a complete character or count. Add it to newResult and continue.
            }
        }
    }
}

/* This function reads the header from a compressed input file and returns it as a string. */

string readHeader(ibitstream& input) {
    string result;
    while(!input.fail()) {
        int next = input.get();
        result += next;
        if(next == '}') {
            break;
        }
    }
    return result;
}

/* This function frees the memory from the Huffman tree which begins at the pointer that gets passed in. */

void freeTree(HuffmanNode* node) {
    if(node != NULL) {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
