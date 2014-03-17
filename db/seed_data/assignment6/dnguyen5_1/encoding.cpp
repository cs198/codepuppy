// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include "pqueue.h"
#include "HuffmanNode.h"
#include "map.h"
#include "filelib.h"
//#include "ListNode.h"
//#include "PQEntry.h"
//#include "PQEntry.h"
// TODO: include any other headers you need


//Builds a frequency table. Looks at a given file/string and for each unique character, creates a map with the character (as an int) as a key
//and its frequency as its value.
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int,int> freqTable;
    while (!input.fail()) {
        char c = input.get();
        int key = c;
        if (key == -1) break;
        if (!freqTable.containsKey(key)) {
            freqTable.put(key, 1);
        }
        else if (freqTable.containsKey(key)) {
            int freq = freqTable[key];
            freq++;
            freqTable[key] = freq++;
        }
    }
    freqTable[PSEUDO_EOF] = 1;
    return freqTable;          // this is just a placeholder so it will compile
}


//Uses frequency table to create an encoding tree. Uses a priority queue of Huffman Nodes to ensure that the most commonly used
//characters are higher in the tree. Uses helper function below to return a pointer to the node representing the root of the tree.
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    for (int key : freqTable) {
        HuffmanNode* newNode = new HuffmanNode(key, freqTable[key], NULL, NULL);// = HuffmanNode(key, freqTable[key]);
        pq.enqueue(newNode, newNode->count);
    }
    return buildEncodingTreeHelper(pq);
}

//Encoding tree's recursive helper. Returns a pointer to the node representing the root of the tree.
HuffmanNode* buildEncodingTreeHelper(PriorityQueue<HuffmanNode*> pq) {
    if (pq.size() == 1) {
        return pq.dequeue();
    }
    else {
        HuffmanNode* first = pq.dequeue();
        HuffmanNode* second = pq.dequeue();
        HuffmanNode* newNode = new HuffmanNode(NOT_A_CHAR, first->count + second->count, first, second);
        pq.enqueue(newNode, newNode->count);
        return buildEncodingTreeHelper(pq);
    }
}

//Recursive helper function for buildEncodingMap method. Traverses through tree using
//recursive backtracking.
void buildMapHelper(HuffmanNode* currentNode, string s, Map<int, string>& map) {
    if (!currentNode->isLeaf()) {
        if (currentNode->zero != NULL) {
            s += '0';
            buildMapHelper(currentNode->zero, s, map);
            s = s.substr(0, s.length() - 1);
        }
        if (currentNode->one != NULL) {
            s += '1';
            buildMapHelper(currentNode->one, s, map);
            s = s.substr(0, s.length() - 1);
        }
    } else map.put(currentNode->character, s);
}

//Accepts a pointer to the root node of a Huffman tree and uses it to create and return a
//Huffman encoding map based on the trees structure. If the encoding tree is NULL, returns an empty map.
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    if (encodingTree == NULL) return encodingMap;
    string s;
    buildMapHelper(encodingTree, s, encodingMap);
    return encodingMap;
}

//Reads one character at a time from a given input file,
//and uses the provided encoding map to encode each character to binary, then writes the character's encoded binary bits
//to the given bit output bit stream.
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while (!input.fail()) {
        int c = input.get();
        string huffmanValue = encodingMap[c];
        for (int i = 0; i < huffmanValue.size(); i++) {
            output.writeBit(huffmanValue[i] -'0');
        }
    }
    string huffmanValue = encodingMap[PSEUDO_EOF];
    for (int i = 0; i < huffmanValue.size(); i++) {
        output.writeBit(huffmanValue[i] -'0');
    }
}

//Reads bits from the given input file one at a time, and recursively walks through the specified
//decoding tree to write the original uncompressed contents of that file to the given output stream.
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* current = encodingTree;
    int i = input.readBit();
    if (i != -1) {
        decodeDataHelper(input, encodingTree, output, i, current);
    }
}

//recursive helper function for decodeData.
void decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output, int i, HuffmanNode* current) {
    if (i != -1) {
        if (current->isLeaf()) {
            if (current->character == PSEUDO_EOF) return;
            output.put(current->character);
            current = encodingTree;
        }
        if (i == 0) {
            i = input.readBit();
            current = current->zero;
            decodeDataHelper(input, encodingTree, output, i, current);
        }
        else if (i == 1) {
            i = input.readBit();
            current = current->one;
            decodeDataHelper(input, encodingTree, output, i, current);
        }
    }
}

//Reads the input file one character at a time, builds an encoding of its contents, and writes a compressed version
//of that input file, including a header, to the specified output file.
void compress(istream& input, obitstream& output) {
    Map<int, int> map = buildFrequencyTable(input);
    output << map;
    rewindStream(input); //rewinds stream to beginning of file
    HuffmanNode* root = buildEncodingTree(map);
    Map<int, string> encodingMap = buildEncodingMap(root);
    encodeData(input, encodingMap, output);
    freeTree(root);
}

//Reads the bits from the given input file one at a time, including the header packed
//inside the start of the file, to write the original contents of that file to the file specified by the output parameter.
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> map;
    input >> map;
    HuffmanNode* root = buildEncodingTree(map);
    decodeData(input, root, output);
    freeTree(root);
}

//Recursive function. Free the memory associated with the tree whose root
//node is represented by the given pointer.
void freeTree(HuffmanNode* node) {
    if (node == NULL) return;
    freeTree(node->zero);
    freeTree(node->one);
    delete node;
}
