
// This file implements the encoding.h file.
//mini-extension (lines 139, 179): gives user the option to add a 6-digit password encoding to the file.

#include "encoding.h"
//Additional '#include's for member functions
#include "pqueue.h"
#include "foreach.h"
#include "filelib.h"
#include "strlib.h"
#include "simpio.h"

/*Frees memory associated with an encoding tree.*/
void freeTree(HuffmanNode *node);

//Builds a table of how frequently each character appears in a text. Called on by Compress method.
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while (true) {
        int i = input.get();
        if (i == -1) break;
        freqTable[i]++;
    }
    freqTable[PSEUDO_EOF]++; //adds PSEUDO-EOF character to map once file is done reading the text
    return freqTable;
}

/*Constructs a tree of HuffmanNodes that point to the Huffman encoding of each character.
 * Returns the pointer to the "root" node of the tree.
 * Inputs the Frequency Table map of how often characters appear in the file.
 * Called on by the Compress File and Decompress File methods.*/
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> freqQueue;
    for (int letter : freqTable) {
        HuffmanNode* insert = new HuffmanNode(letter, freqTable[letter], NULL, NULL);
        freqQueue.enqueue(insert, insert->count);
    }
    while (freqQueue.size() > 1) {
        HuffmanNode* left = freqQueue.dequeue();
        HuffmanNode* right = freqQueue.dequeue();
        int sum = left->count + right->count;
        HuffmanNode* sumNode = new HuffmanNode(NOT_A_CHAR, sum, left, right);
        freqQueue.enqueue(sumNode, sum);
    }
    HuffmanNode* front = freqQueue.dequeue();
    return front;
}

/*Helper for the buildEncodingMap function.*/
void buildEncodingMapHelper(HuffmanNode* current, Map<int, string>& encodingMap, string& path);

/*Builds a map containing the Huffman encodings of each unique character in the file.
 * Inputs the Encoding Tree.
 * Called on by the Compress File method.*/
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string path;
    buildEncodingMapHelper(encodingTree, encodingMap, path);
    return encodingMap;
}

/*Recursive helper function for encodingMap function.
 * Inputs: a pointer node to a branch on the Huffman Tree, the map of characters->encodings,
 *  a string that represents the path.*/
void buildEncodingMapHelper(HuffmanNode* current, Map<int, string>& encodingMap, string& path) {
    if (current->isLeaf()) { //base case: reach the end of path
        encodingMap[current->character] = path;
    } else { //traverse the map
        string left = path + integerToString(0);
        string right = path + integerToString(1);
        buildEncodingMapHelper(current->zero, encodingMap, left);
        buildEncodingMapHelper(current->one, encodingMap, right);
    }
}

/*Encodes an input stream (file) of data into bits using the Huffman encoding strategy.
 * Inputs a data stream, the Encoding Map with the Huffman Encodings of each character in the file,
 * and writes the respective bits to an output stream.
 * Called on by the Compress File method.*/
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int letter;
    while (true) {
        letter = input.get();
        if (letter == -1) break;
        string outbits = encodingMap[letter];
        for (int i = 0; i < outbits.length(); i++) {
            int num = outbits[i] - '0';
            output.writeBit(num);
        }
    }
    string outbits = encodingMap[PSEUDO_EOF]; //adds the encoding for the EOF character to the map
    for (int i = 0; i < outbits.length(); i++) {
        int num = outbits[i] - '0';
        output.writeBit(num);
    }
}

/*Recursive helper function for decodeData.
 * Inputs: the input stream, a pointer to a HuffmanNode on the tree, an output stream,
 * an int n that represents the last read bit from the file, a boolean for if the end of the
 * file has been reached.
*/
void decodeDataHelper(ibitstream& input, HuffmanNode* current, ostream& output, int &n, bool& endOfFile);

/*Decodes an input stream (file) of data from bits encoded by Huffman encoding into characters.
 * Inputs a data stream (in bits), the Encoding Tree that generates the Huffman Encodings of each character
 * in the file, and an output stream (the text file). Writes the decoded characters to the stream.
 * Called on by the Decode File method.*/
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* current = encodingTree;
    int n = input.readBit();
    bool endOfFile = false;
    while (true) {
        if (endOfFile) break;
        if (n == -1) break; //just in case for whatever reason EOF is not in file
        decodeDataHelper(input, current, output, n, endOfFile);
    }
}

void decodeDataHelper(ibitstream& input, HuffmanNode* current, ostream& output, int& n, bool& endOfFile) {
    if (current->isLeaf()) { //base case: reach the end of the tree
        if (current->character == PSEUDO_EOF) { //reached the EOF character
            endOfFile = true;
            return;
        } else { //reached a normal character
            output.put(current->character);
            return;
        }
    }
    if (n == 0) { //traverse left
        n = input.readBit();
        decodeDataHelper(input, current->zero, output, n, endOfFile);
    } else if (n == 1) { //traverse right
        n = input.readBit();
        decodeDataHelper(input, current->one, output, n, endOfFile);
    }
}

/*Encodes the file with a 6-digit passcode stored at the beginning of the document.*/
string compressPasscode(obitstream& output) {
    string passcode;
    while (true) {
        bool isValid = true;
        passcode = getLine("Please enter a six-digit passcode: ");
        if (passcode.length() != 6 /*or another number*/) {
            isValid = false;
            cout << "Passcode is not six digits long." << endl;
        }
        for (char ch : passcode) {
            if (!isdigit(ch)) {
                isValid = false;
                cout << "Passcode should only contain numbers." << endl;
                break;
            }
        }
        if (isValid) break;
    }
    output << passcode; //'######'
    return passcode;
}

/*Compresses a file using the Huffman encoding strategy. Stores the frequency map as the header
 * of the output document.
 * Frees memory associated with the Huffman encoding tree.*/
void compress(istream& input, obitstream& output) {
    bool usePasscode = getYesOrNo("Do you want to use a passcode? ");
    string passcode;
    if (usePasscode) passcode = compressPasscode(output);
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    output << freqTable; //writes the frequency table to the file
    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
    if (usePasscode) cout << "To decompress this file, use passcode: " << passcode << endl;
}

/*Checks to see if the document is secured with a passcode. Assumes that the document is formatted
 * as follows: "[6-digit passcode][{map}][encrypted file text]" */
bool thereIsPasscode(ibitstream& input) {
    char ch = input.get();
    input.rewind();
    if (ch == '{') return false;
    else return true; // ch == number
}

/*Asks the user to enter a valid password if the document is password-secured.*/
bool checkPasscode(ibitstream& input) {
    if (!thereIsPasscode(input)) return true; //document is not secured by a passcode
    else {
        string checkcode;
        for (int i = 0; i < 6 /*or password length*/; i++) { //retrieves the passcode stored in the file
            checkcode += input.get();
        }
        for (int chances = 2; chances >= 0; chances--) { //gives user three chances to guess passcode
            string entered = getLine("Please enter the passcode to decompress this file: ");
            if (entered == checkcode) return true;
            else {
                if (chances == 2) cout << "Wrong passcode. You have " << chances << " guesses left." << endl;
                else if (chances == 1) cout << "Wrong passcode. You have " << chances << " guess left." << endl;
            }
        }
        cout << "Invalid password, access denied." << endl;
        return false;
    }
}

/*Decompresses the file using a Huffman encoding map stored in the .huf file header.
 * Frees memory associated with the Huffman encoding tree.*/
void decompress(ibitstream& input, ostream& output) {
    if (checkPasscode(input)) {
        Map<int, int> freqTable;
        input >> freqTable;
        HuffmanNode* encodingTree = buildEncodingTree(freqTable);
        decodeData(input, encodingTree, output);
        freeTree(encodingTree);
    }
}

//Defined at beginning of file
void freeTree(HuffmanNode* node) {
    if (node == NULL) return;
    if (node->isLeaf()) { delete node;}
    else {
        freeTree(node->zero);
        freeTree(node->one);
    }
}
