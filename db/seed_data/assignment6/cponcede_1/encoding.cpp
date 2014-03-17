/* Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * File: encoding.cpp
 * -----------------------------
 * Contains functions that are used within huffmain main in order to compress data into
 * .huf files or to decompress data that is already in a .huf file to its original data
 * type.
 */

#include "encoding.h"
#include "simpio.h"
#include "map.h"
#include <fstream>
#include "strlib.h"
#include "pqueue.h"
#include "bitstream.h"
#include "filelib.h"

//Funtion prototypes
void buildEncodingMapHelper(HuffmanNode *node, Map<int, string> &map, string line);
int decodeData(ibitstream &input, HuffmanNode *encodingTree, ostream &output, HuffmanNode* node);
Map<int,int> getFrequencyTable(string file);

/* Takes in input that describes a file and returns a Map that counts the number of
 * occurrences of each character in that file.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    string line;
    while (true) {
        int character = input.get();
        if (character == -1) {
            break;
        }
        if (!freqTable.containsKey(character)) {
            freqTable.put(character,1);
        } else {
            int count = freqTable[character] + 1;
            freqTable.put(character,count);
        }
    }
    freqTable.put(PSEUDO_EOF,1);
    return freqTable;
}

/* Accepts a Map that counts the frequency of each character in a file and
 * creates a binary tree out of that information that can be used to determine a Huffman Encoding
 * for the file. It does this by putting each character into a priority queue, where the
 * character's priority is the count. The function creates the binary tree by dequeueing the first two elements
 * in the priority queue before making them children of a new node, whose priority is the sum of its
 * childrens' priorities. After it has finished making the tree, the function returns a pointer
 * to the root of the tree.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    for (int key : freqTable) {
        HuffmanNode* node = new HuffmanNode(key,freqTable.get(key));
        pq.enqueue(node,freqTable.get(key));
    }
    while (pq.size() > 1) {
        HuffmanNode* node1 = pq.dequeue();
        HuffmanNode* node2 = pq.dequeue();
        int newPriority = node1->count + node2->count;
        HuffmanNode* root = new HuffmanNode(NOT_A_CHAR,newPriority,node1,node2);
        pq.enqueue(root,newPriority);
    }
    HuffmanNode* result = pq.dequeue();
    return result;
}

/* Traverses the encoding tree multiple times to determine the Huffman binary representation of each
 * character from the input file. Returns a map that contains the associated binary string value for each
 * character in the file.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string line = "";
    buildEncodingMapHelper(encodingTree,encodingMap,line);
    return encodingMap;
}

/* The helper function of buildEncodingMap that uses recursion to traverse the
 * encoding tree.
 */
void buildEncodingMapHelper(HuffmanNode* node, Map<int,string>& map, string line) {
    if (node == NULL) {
        return;
    }
    if (node->character != NOT_A_CHAR) {
        map.put(node->character,line);
    }
    line += '0';
    buildEncodingMapHelper(node->zero,map,line);
    line.erase(line.length()-1,1);
    line += '1';
    buildEncodingMapHelper(node->one,map,line);

}

/* Uses a given encodingMap and a file that is the input to translate that file into a
 * new .huf file that contains the new Huffman binary representation of the inputted file.
 * The function returns this binary encoding.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while (true) {
        int character = input.get();
        if (character == -1) break;
        string bits = encodingMap[character];
        for (int i = 0 ; i < bits.length() ; i++) {
            int bit = bits[i] - '0';
            output.writeBit(bit);
        }
    }
    string endOfFileMarker = encodingMap.get(PSEUDO_EOF);
    for (int i = 0 ; i < endOfFileMarker.length(); i++) {
        int bit = endOfFileMarker[i] - '0';
        output.writeBit(bit);
    }
}

/* Takes a given input that is represented by the Huffman encoding that is present
 * in the provided encoding tree and translates the file back to its original, uncompressed
 * force.
 */
void decodeData(ibitstream &input, HuffmanNode *encodingTree, ostream &output) {
    int character = 0;
    while (true) {
        character = decodeData(input,encodingTree,output,encodingTree);
        if (character == PSEUDO_EOF) break;
        output.put(character);
    }
}

/* Helper function to decodeData that has the additional parameter of the current node on the
 * encoding tree that the function is looking at. This program is called recursively to traverse
 * down the tree until a leaf is reached.
 */
int decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output, HuffmanNode* node) {
    if (node->zero == NULL && node->one == NULL) {
        return node->character;
    } else {
        int bit = input.readBit();
        if (bit == 0) {
            return decodeData(input,encodingTree,output,node->zero);
        } else if (bit == 1) {
            return decodeData(input,encodingTree,output,node->one);
        }
    }
}

/* Takes in a file and then returns a .huf file that represents the same file
 * but utilizing Huffman Encoding. The function also adds a header to the encoded file
 * that includes the frequency table for the inputted file. This is used as a map to help
 * decompress the .huf file later.
 */
void compress(istream& input, obitstream& output) {
    Map<int,int> freqTable = buildFrequencyTable(input);
    rewindStream(input);
    output << freqTable << endl;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int,string> encodingMap = buildEncodingMap(encodingTree);
    encodeData(input,encodingMap,output);
}

/*Takes in a file that has been compressed using Huffman Encoding, that also includes a header
 * of a frequency table for the original, uncompressed file, and returns the original uncompressed
 * file.
 */
void decompress(ibitstream& input, ostream& output) {
    string frequencyTableString = "";
    string line;
    while (getline(input,line)) {
        frequencyTableString += line;
        if (line.find("}") != string::npos) {
            break;
        }
    }
    Map<int,int> freqTable = getFrequencyTable(frequencyTableString);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input,encodingTree,output);
}

/* Takes in a string that represents a Map of integers to integers and interprets the
 * string to create the Map that it then returns.
 */
Map<int,int> getFrequencyTable(string frequencyTableString) {
    Map<int,int> freqTable;
    int endOfTable = frequencyTableString.find("}");
    frequencyTableString = frequencyTableString.substr(0,endOfTable+1);
    frequencyTableString = frequencyTableString.substr(1,frequencyTableString.length() - 2);
    Vector<string> mapPairs = stringSplit(frequencyTableString,", ");
    for (string pair : mapPairs) {
        int endOfKey = pair.find(":");
        int key = stringToInteger(pair.substr(0,endOfKey));
        pair.erase(0,endOfKey+1);
        int value = stringToInteger(pair);
        freqTable.put(key,value);
    }
    return freqTable;
}

/* Frees the memory associated with the tree whose root node is represented by the
 * given pointer.
 */
void freeTree(HuffmanNode* node) {
    if (node->zero != NULL) {
        freeTree(node->zero);
    }
    if (node->one != NULL) {
        freeTree(node->one);
    }
    delete node;
}
