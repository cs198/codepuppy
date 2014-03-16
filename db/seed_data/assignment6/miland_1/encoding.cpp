/**
 * Name: Milan Doshi
 * Assignment: encoding.cpp
 * TA: Jimmy Lee
 *
 * This file implements many of the functions this program is meant to carry out, from building a frequency table with
 * an input file, to building an encoding map and tree. From there, the user is able to encode or decode input or output
 * through the use of other methods.
 *
 * EXTENSION: Add support for encryption in addition to encoding in the checkForPassword() method.
 */

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"
#include "simpio.h"

const string password = "Password";
const int maxAttempts = 3;

/**
 * method: buildFrequencyTable()
 * return: Map<int,int>
 * This method takes in an input file and based on a characters in the input file, creates a frequency table of how
 * frequently each character is present in the input file.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    checkForPassword();
    Map<int, int> freqTable;
    int ch;
    ch = input.get();
    while(!input.fail()){
        if(!freqTable.containsKey(ch)) {
            freqTable.put(ch,1);
        } else {
            int value = freqTable.get(ch);
            freqTable.put(ch,++value);
        }
        ch = input.get();
    }
    if(input.eof()) {
        freqTable.put(PSEUDO_EOF,1);
    }
    return freqTable;
}

/**
 * method: checkForPassword();
 * return: void
 * This method adds support for encryption by adding a password feature to the program. If the user does not enter the correct
 * password with a maximum of three attempts, then the program throws an exception.
 */
void checkForPassword() {
    string passwordEntered = getLine("Please enter a password to access the encoding table: ");
    int attempts = 1;
    while(passwordEntered != password) {
        if(attempts != maxAttempts) {
            cout << "That attempt was incorrect." << " You have " << maxAttempts - attempts << " attempts left.";
            passwordEntered = getLine(" Please try again: ");
            attempts++;
        } else if(attempts == maxAttempts) {
            cout << "You are out of attempts" << endl;
            throw "You are out of attempts";
        }
    }
}

/**
 * method: buildEncodingTree(const Map<int,int>& freqTable)
 * return: HuffmanNode*
 * This method builds an encoding tree based on the frequency table created for a specific input file.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> huffman;
    HuffmanNode* front;
    for(int key : freqTable) {
        HuffmanNode* current = new HuffmanNode(key, freqTable[key]);
        huffman.enqueue(current, freqTable[key]);
    }
    while(huffman.size() > 1) {
        HuffmanNode* first = huffman.dequeue();
        HuffmanNode* second = huffman.dequeue();
        HuffmanNode* combination = new HuffmanNode(NOT_A_CHAR, first->count + second->count,first,second);
        huffman.enqueue(combination,combination->count);
    }
    return huffman.dequeue();
}

/**
 * method: buildEncodingMap(HuffmanNode* encodingTree)
 * return: Map<int, string>
 * This method allows the user to build an encoding map based on the encoding tree for a certain input file.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string value = "";
    HuffmanNode* root = encodingTree;
    buildEncodingMapHelper(root, value, encodingMap);
    return encodingMap;
}

/**
 * method: buildEncodingMapHelper(HuffmanNode* root, string value, Map<int,string>& encodingMap)
 * return: void
 * This is the recursive helper method that allows the program to the build the encoding map more efficiently.
 */
void buildEncodingMapHelper(HuffmanNode* root, string value, Map<int,string>& encodingMap) {
    if(root->character != NOT_A_CHAR && root->isLeaf()) {
        int ch = root->character;
        encodingMap.put(ch, value);
    }

    if(root->zero != NULL) {
        HuffmanNode* left = root->zero;
        string value2 = value + "0";
        buildEncodingMapHelper(left, value2, encodingMap);
    }

    if(root->one != NULL) {
        HuffmanNode* right = root->one;
        string value2 = value + "1";
        buildEncodingMapHelper(right, value2, encodingMap);
    }

}

/**
 * method: encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output)
 * return: void
 * This is the method that encodes data given an input file and the encoding map, and outputs the result
 * in bits (0's and 1's)
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int ch = input.get();
    while(!input.fail()) {
        string line = encodingMap[ch];
        for(char character: line) {
            int choutput = character - '0';
            output.writeBit(choutput);
        }
        ch = input.get();
    }

    string line = encodingMap[PSEUDO_EOF];
    for(char character: line) {
        int choutput = character - '0';
        output.writeBit(choutput);
    }

}

/**
 * method: decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output)
 * return: void
 * This is method that initiates the process of decoding data based on an input file of 0's and 1's and does so
 * by calling a helper method.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* root = encodingTree;
    decodeDataHelper(root, input, output, encodingTree);

}

/**
 * method: decodeDataHelper(HuffmanNode* root, ibitstream& input, ostream& output, HuffmanNode* encodingTree)
 * return: void
 * This is the helper method that decodes data based on the next bit that is in an input file.
 */
void decodeDataHelper(HuffmanNode* root, ibitstream& input, ostream& output, HuffmanNode* encodingTree) {
    if(root->character == NOT_A_CHAR) {
        int nextBit = input.readBit();
        if(nextBit == 1) {
            root = root->one;
            if(root->character != NOT_A_CHAR && root->isLeaf() && root->character != PSEUDO_EOF) {
                int choutput = root->character;
                output.put(choutput);
                decodeDataHelper(encodingTree, input, output, encodingTree);
            } else {
                decodeDataHelper(root, input, output, encodingTree);
            }
        } else if (nextBit == 0) {
            root = root->zero;
            if(root->character != NOT_A_CHAR && root->isLeaf() && root->character != PSEUDO_EOF) {
                int choutput = root->character;
                output.put(choutput);
                decodeDataHelper(encodingTree, input, output, encodingTree);
            } else {
                decodeDataHelper(root, input, output, encodingTree);
            }
        }
    }
    if(root->character == PSEUDO_EOF && root->isLeaf()) {
        return;
    }
}

/**
 * method: compress(istream& input, obitstream& output)
 * return: void
 * This method compresses many of the tasks in this program. It creates a compressed file with the encoding map and the
 * the output after certain input is encoded.
 */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    output << freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

/**
 * method: decompress(ibitstream& input, ostream& output)
 * return: void
 * This method decompresses a given compressed file with an encoding map and the compressed bits.
 */
void decompress(ibitstream& input, ostream& output) {
   Map<int, int> freqTable;
   input >> freqTable;
   HuffmanNode* encodingTree = buildEncodingTree(freqTable);
   decodeData(input, encodingTree, output);
   freeTree(encodingTree);
}

/**
 * method: freeTree(HuffmanNode* node)
 * return: void
 * This method frees any memory that was used in the process of creating an encoding tree.
 */
void freeTree(HuffmanNode* node) {
    if(node == NULL) return;
    else {
        HuffmanNode* left = node->zero;
        HuffmanNode* right = node->one;
        delete node;
        freeTree(left);
        freeTree(right);
    }
}
