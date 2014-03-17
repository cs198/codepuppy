// This program implements multipe essential functions of conpressing
//and decompressing files, using Maps and trees.

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"


//Prototypes:
void pqToTree(PriorityQueue<HuffmanNode *> &pq, HuffmanNode * &root);
void buildEncodingMapHelper(HuffmanNode* node, Map<int, string> &encodingMap, string & code);
int decodeHelper(ibitstream& input, HuffmanNode* encodingTree);

//Read from an input file, and store each charater and its frequency into
//a Map<int, int> frequency table.
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while(true) {
        int ch = input.get();
        if(ch==-1) break;
        freqTable[ch]++;
    }
    freqTable.put(PSEUDO_EOF,1);
    return freqTable;
}

//Based on a frequency table, it builds a huffman encoding tree, by euqueueing
//each character into a priority queue with their frequency as priority.
//Dequeueing two at a time, put a parent node for these two dequeued, and
//add the parent back to the priority queue with the sum of the children's
//frequencies as priority. This process perceeds until one node left in the queue.
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    for(int ch: freqTable) {
        HuffmanNode* node = new HuffmanNode(ch,freqTable[ch]);
        pq.enqueue(node,freqTable[ch]);
    }
    HuffmanNode* root;
    pqToTree(pq, root);
    return root;
}

//A helper function to build the encoding tree. It takes care of the dequeuing two, put
//one parent and dequeue the parent back to the priority queue.After this function, the
//root will be the last node in the queue and is the root of the tree.
void pqToTree(PriorityQueue<HuffmanNode*> & pq, HuffmanNode* & root) {
    if(pq.size() <= 1) {
        HuffmanNode* lastNode = pq.dequeue();
        root = lastNode;
        return;
    } else {
        HuffmanNode* node1 = pq.dequeue();
        HuffmanNode* node2 = pq.dequeue();
        int parentFreq = node1->count + node2->count;
        HuffmanNode* parent = new HuffmanNode(NOT_A_CHAR, parentFreq, node1, node2);
        pq.enqueue(parent,parentFreq);
        return pqToTree(pq,root);
    }
}

//Given a huffman encoding tree, it translate the coding into a Map of each charater
//associeted with its huffman codes.
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {    
    Map<int, string> encodingMap;
    string code;
    buildEncodingMapHelper(encodingTree,encodingMap,code);
    return encodingMap;
}

//Helper function of building encoding map. It recursively traversing the tree. If it's a
//leaf, the charater in the leaf is stored into the encoding map and the path it traversed
// is the codes for this charater stored as a string. If the code is not a leaf, continue
// traversing recursively while recording the path by going left as adding '0' and going
//right as adding'1' into the string code.
void buildEncodingMapHelper(HuffmanNode* node, Map<int, string> &encodingMap, string & code) {
    if(node == NULL) return;
    if(node->isLeaf()) {
        encodingMap[node->character] = code;
        return;
    }else {
        code += "0";
        buildEncodingMapHelper(node->zero,encodingMap,code);
        code = code.substr(0,code.length()-1);
        code += "1";
        buildEncodingMapHelper(node->one,encodingMap,code);
        code = code.substr(0,code.length()-1);
    }
}

//Using the encoding map to encode a input file and write it out bit by bit into a output stream
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    rewindStream(input);
    while(true) {
        int ch = input.get();
        if(ch==-1) break;
        string code = encodingMap[ch];
        for (int i=0; i<code.length(); i++) {
            output.writeBit(code[i]-'0');
        }
    }
    //adding the PSEUDO_EOF at the end.
    string eof = encodingMap[PSEUDO_EOF];
    for (int i=0; i<eof.length(); i++) {
        output.writeBit(eof[i]-'0');
    }
}

//Read from a huffman encoded input file bit by bit, and translate into ASCII charaters and
//put them into a output. If it reads PSEUDO_EOF, it stops.
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    while(true) {
        int ch = decodeHelper(input, encodingTree);
        if(ch == PSEUDO_EOF) break;
        output.put(ch);
    }
}
//Helper function for decoding. This function is responsible for translating one charater by
//traversing the encoding tree based on the bit read from the input. If it's '0' go to zero;
//if it's '1' go to one of the huffmanNode. Until it reaches a leaf, and return the charater
// in that leaf back.
int decodeHelper(ibitstream& input, HuffmanNode* encodingTree) {
    if(encodingTree->isLeaf()) return encodingTree->character;
    int bit = input.readBit();
    if(bit == 0) return decodeHelper(input, encodingTree->zero);
    if(bit == 1) return decodeHelper(input, encodingTree->one);
}

//It reads an input of ASCII content, and conpress it into a huffman coded output. Frees
//the memory used for the encoding tree in the process.
void compress(istream& input, obitstream& output) {
    if(input.fail()) return;
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    output << freqTable;
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

//It reads a huffman coded input and decompress it into ASCII content output
// Frees the memory used for the encoding tree in the process.
void decompress(ibitstream& input, ostream& output) {
    if(input.fail()) return;
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree,output);
    freeTree(encodingTree);
}

//It frees the memory used for a tree given the root, by post-order traversing the tree.
void freeTree(HuffmanNode *node) {
    if(node != NULL) {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}

