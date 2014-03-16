//encoding.cpp

#include "encoding.h"
#include "pqueue.h"

/*
 * Function Prototypes
 */
void createPQueue(const Map<int, int>& freqTable, PriorityQueue<HuffmanNode*>& pq);
HuffmanNode* createTree(PriorityQueue<HuffmanNode*>& pq);
void encodingHelper(HuffmanNode* node, Map<int, string>& encodingMap, string code = "");
int decodeHelper(HuffmanNode* node, string& bitString, ibitstream& input);
string getBinaryString (istream& input, const Map<int, string>& encodingMap);
void writeToFile (string binary, obitstream& output);

/*
 * Reads from input from a given istream that counts and returns a mapping from each character
 * (represented as int) to the numbe rof times that character appears in the file. The fake character
 * PSEUDO_EOF is inserted into the map to represent the end of the file. Input files are assumed to
 * exist and be readable.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while(!input.fail()) {
        int letter = input.get();
        if(letter == -1){
            freqTable[PSEUDO_EOF]++;
        } else {
            freqTable[letter]++;
        }
    }
    return freqTable;
}

/*
 * Creates a Huffman encoding tree based on the frequencies in the frequency map and returns a pointer
 * to the root of the tree.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue <HuffmanNode*> pq;
    createPQueue(freqTable, pq);
    return createTree(pq);
}

/*
 * Builds the Huffman Encoding Tree based on the given priority queue and then returns a pointer to the root
 * of the tree.
 */
HuffmanNode* createTree(PriorityQueue<HuffmanNode*>& pq) {
    while (pq.size() > 1) {
        HuffmanNode* first = pq.dequeue();
        HuffmanNode* sec = pq.dequeue();
        HuffmanNode* temp = new HuffmanNode(NOT_A_CHAR, first->count + sec->count);
        if (first->count > sec->count) {
            temp->zero = sec;
            temp->one = first;
        } else {
            temp->zero = first;
            temp->one = sec;
        }
        pq.enqueue(temp, temp->count);
    }
    return pq.dequeue();
}

/*
 * Creates a node for every frequency entry in the map and then enqueues these nodes into a priority queue.
 * The priority queue is passed by reference.
 */
void createPQueue(const Map<int, int>& freqTable, PriorityQueue<HuffmanNode*>& pq) {
    for(int letter: freqTable) {
        HuffmanNode* temp = new HuffmanNode(letter, freqTable[letter]);
        pq.enqueue(temp, temp->count);
    }
}

/*
 * Accepts a pointer to the root of the HuffmanNode tree to build a map that contains a mapping from the character
 * to its binary representation. If the tree is empty (root is NULL), then an empty map will be returned.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    encodingHelper(encodingTree, encodingMap);
    return encodingMap;
}

/*
 * Recursive helper function for buildEncodingMap.
 * Traverses the entire tree to add the binary representation to the encodingMap once it reaches a leaf node.
 */
void encodingHelper(HuffmanNode* node, Map<int, string>& encodingMap, string code) {
    if (node != NULL) {
        if (node->isLeaf()) {
            encodingMap[node->character] = code;
        } else {
            encodingHelper(node->zero, encodingMap, code + "0");
            encodingHelper(node->one, encodingMap, code + "1");
        }
    }
}

/*
 * Builds a string (consisting of 0 and 1s) from input and the encodingMap.
 * Then writes this string back into bits to output.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    string binary = getBinaryString(input, encodingMap);
    writeToFile(binary, output);
}

/*
 * Writes the given binary string into the file as bits.
 */
void writeToFile (string binary, obitstream& output) {
    for (int i = 0; i < binary.length(); i++) {
        string character = binary.substr(i, 1);
        output.writeBit(stringToInteger(character));
    }
}

/*
 * Returns a string composed of 0's and 1's (that represents bits) that is built from reading one character
 * at a time from the file and using the encodingMap to find it's binary equivalent. The binary code for the PSEUDO_EOF
 * is added to the end of the string to denote the end of input.
 */
string getBinaryString (istream& input, const Map<int, string>& encodingMap) {
    string binary;
    while (!input.fail()) {
        int a = input.get();
        binary += encodingMap[a];
    }
    binary += encodingMap[PSEUDO_EOF];
    return binary;
}

/*
 * Decodes the data by recursively walking through the encodingTree while the character is not PSEUDO_EOF.
 * While the loop runs, the character is printed out to the file.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    if (encodingTree != NULL) {
        string bitString = "";
        while (true) {
            int character = decodeHelper(encodingTree, bitString, input);
            if (character == PSEUDO_EOF) {
                break;
            }
            output << (char) character;
            bitString = "";
        }
    }
}

/*
 * Recursive helper function for decodeData that takes in the bitString by reference. It continually reads in one
 * character at a time from input and goes to node->zero if the character is 0 and node->one if the character is 1.
 * The recursive function returns an int representation of the character when we have reached a leaf.
 */
int decodeHelper(HuffmanNode* node, string& bitString, ibitstream& input) {
    if (node->isLeaf()) {
        return node->character;
    } else {
       if (input.readBit() == 1) {
           decodeHelper(node->one, bitString += "1", input);
       } else {
           decodeHelper(node->zero, bitString += "0", input);
       }
    }
}

/*
 * Overall compression function that compresses the given file into the given output file.
 * The frequencyTable is read into the output file as a header.
 */
void compress(istream& input, obitstream& output) {
    if (!input.fail()) {
        Map<int, int> frequencyTable = buildFrequencyTable(input);
        HuffmanNode* root = buildEncodingTree(frequencyTable);
        output << frequencyTable;
        input.clear();
        input.seekg(0, ios::beg);
        Map<int, string> encodingMap = buildEncodingMap(root);
        encodeData(input, encodingMap, output);
        freeTree(root);
    }
}

/*
 * Overall decompression function that decompresses the given file by reading in the frequencyTable (header)
 * and then proceeding to decompress the rest of the file.
 */
void decompress(ibitstream& input, ostream& output) {
    if (!input.fail()) {
        Map <int, int> frequencyTable;
        input >> frequencyTable;
        HuffmanNode* root = buildEncodingTree(frequencyTable);
        decodeData(input, root, output);
        freeTree(root);
    }
}

/*
 * Frees the memory in the HuffmanNode encoding tree by traversing through all the nodes and deleting them.
 */
void freeTree(HuffmanNode* node) {
    if (node != NULL) {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
