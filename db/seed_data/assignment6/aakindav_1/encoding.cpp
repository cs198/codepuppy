/* Charles Akin-David
* CS106B
* SL: Jimmy Lee
* Sources used: Lecture Slides, LaIR
* This program uses the Huffman coding algorithm for compression
* which will usually want the file smaller and can decompress the
* file back into it's original state.
*/

#include "encoding.h"
#include "pqueue.h"
#include "strlib.h"
#include "filelib.h"

void buildEncodingMapHelper(Map<int, string>& encodingMap, HuffmanNode* node, string sofar);
int decodeDataHelper(ibitstream& input, HuffmanNode* node);

/* Initializes a map and continues to read in the input until EOF is hit, puting each character
 * and the number of times it appears into the map. Returns the Map when done.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while (true) {
        int ch = input.get();
        if (ch == EOF) {
            freqTable[PSEUDO_EOF] = 1;
            break;
        }
        if (freqTable.containsKey(ch)) freqTable[ch] += 1;
        else freqTable[ch] = 1;
    }
    return freqTable;
}

/* Creates a PriorityQueue and for each character in the created table, a Huffman node is made and enqueued into
 * the PriorityQueue, then while the size of the PriorityQueue is greater than 1 dequeues out the first and second node
 * combine them into one node and enqueue it back into the queue.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> queue;
    for (int ch:freqTable) {
        HuffmanNode* node = new HuffmanNode (ch, freqTable[ch], NULL, NULL);
        queue.enqueue(node, node->count);
    }
    while (queue.size() > 1) {
        HuffmanNode* first = queue.dequeue();
        HuffmanNode* second = queue.dequeue();
        HuffmanNode* combined = new HuffmanNode (NOT_A_CHAR, first->count+second->count,first,second);
        queue.enqueue(combined, combined->count);
    }
    return queue.dequeue();
}

/* Initializes a map and a string then calls the helper function.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string line = "";
    buildEncodingMapHelper(encodingMap, encodingTree, line);
    return encodingMap;
}

/* If the node is tree is empty return immediately. Also if the node is a stream put the character and
 * the string that has been tracking it's process into the map. Else go left and add 0 to the string
 * and go right and add 1 to the string.
 */
void buildEncodingMapHelper(Map<int, string>& encodingMap, HuffmanNode* node, string sofar) {
    if (node == NULL) return;
    if (node->isLeaf()) {
        encodingMap[node->character] = sofar;
        return;
    } else {
        buildEncodingMapHelper(encodingMap, node->zero, sofar + "0");
        buildEncodingMapHelper(encodingMap, node->one, sofar + "1");
    }
}

/* Gets all the ints in the stream until the end is hit and each bit is written in the
 * obitstream one by one by going through the stored string and calling the string to integer
 * function. Then putting in EOF at the end.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int bit = 0;
    while (true) {
        bit = input.get();
        if (bit == EOF) break;
        string val = encodingMap[bit];
        for (int i=0; i<val.size(); i++) {
            output.writeBit(stringToInteger(val.substr(i,1)));
        }
    }
    string val = encodingMap[PSEUDO_EOF];
    for (int i=0; i<val.size(); i++) {
        output.writeBit(stringToInteger(val.substr(i,1)));
    }
}

/* Calls the helper function until the file is empty getting back one int at a
 * time then that input is outputted.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int ch;
    while (true) {
        ch = decodeDataHelper(input, encodingTree);
        if (ch == PSEUDO_EOF) break;
        output.put(ch);
    }
}

/* Gets the stream and the node and goes down the tree until a leaf is hit and then
 * the character for that leaf is returned.
 */
int decodeDataHelper(ibitstream& input, HuffmanNode* node) {
    if (node->isLeaf()) return node->character;
    else {
        int bit = input.readBit();
        if (bit == 0) return decodeDataHelper(input, node->zero);
        if (bit == 1) return decodeDataHelper(input, node->one);
    }
}
/* Uses the buildFrequencyTable function to build a table, then the
 * table is stored. The the stream is rewinded and outputted into the
 * obitstream. A tree is built with the table, themn encodeMap is called to
 * encode the stream. Then the tree is deleted.
 */
void compress(istream& input, obitstream& output) {
    Map<int,int> freqTable = buildFrequencyTable(input);
    if (freqTable.size() != 1) {
        rewindStream(input);
        output << freqTable;
        HuffmanNode* tree = buildEncodingTree(freqTable);
        Map<int,string> encodeMap = buildEncodingMap(tree);
        encodeData(input, encodeMap, output);
        freeTree(tree);
    }
}

/* First gets the first element in the stream if is no element throws
 * an exception if there is the stream is rewinded and put into a table
 * then a tree is built off the table and decodeData is called to decode
 * the input. Then the tree freed, so save memory.
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input.get();
    if (!input.fail()) {
        rewindStream(input);
        input >> freqTable;
        HuffmanNode* tree = buildEncodingTree(freqTable);
        decodeData(input,tree,output);
        freeTree(tree);
    } else {
        throw "Input file is empty!";
    }
}

/* Method uses recursion to delete all nodes in a binary tree
 */
void freeTree(HuffmanNode* node) {
    if (node != NULL) {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
