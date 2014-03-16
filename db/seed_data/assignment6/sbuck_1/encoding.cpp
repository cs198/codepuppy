// This is a file compressor that uses Huffman
// encoding to shrink file size to much smaller
// sizes.

#include "encoding.h"
#include "strlib.h"
#include <fstream>
#include "pqueue.h"
#include "bitstream.h"
#include "filelib.h"

/**
 * @brief buildFrequencyTable
 * @param input
 * @return
 * buildFrequencyTable builds a map of each charactor and their
 * frequencies. It returns said map.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    freqTable.put(PSEUDO_EOF, 1);
    char c = input.get();
    while(c!= -1){
        if(freqTable.containsKey(c)){
            freqTable.put(c, freqTable.get(c) + 1);
        }else {
            freqTable.put(c, 1);
        }
        c = input.get();
    }
    return freqTable;
}

/**
 * @brief buildEncodingTree
 * @param freqTable
 * @return
 * buildEncodingTree is passed in a map of keys and frequencies and
 * returns a tree made out of HuffmanNodes.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pQ;
    for(int current: freqTable){
        HuffmanNode* entry = new HuffmanNode(current, freqTable.get(current), NULL, NULL);
        pQ.enqueue(entry, freqTable.get(current));
    }
    HuffmanNode* left;
    HuffmanNode* right;
    while(pQ.size() > 1){
        left = pQ.dequeue();
        right = pQ.dequeue();
        HuffmanNode* notAChar = new HuffmanNode(NOT_A_CHAR, left->count + right->count, left, right);
        pQ.enqueue(notAChar, notAChar->count);
    }
    return pQ.dequeue();
}

/**
 * @brief buildEncodingHelper
 * @param encodingTree
 * @param encodingMap
 * @param code
 * This is the helper method for buildEncodingMap.
 */
void buildEncodingHelper(HuffmanNode* encodingTree, Map<int, string>& encodingMap, string code){
    if(encodingTree->character == NOT_A_CHAR){
        buildEncodingHelper(encodingTree->zero, encodingMap, code + "0");
        buildEncodingHelper(encodingTree->one, encodingMap, code + "1");
    }else if(encodingTree->isLeaf()){
        encodingMap.put(encodingTree->character, code);
    }
}
/**
 * @brief buildEncodingMap
 * @param encodingTree
 * @return
 * buildEncodingMap uses a helper method to recursively build a map
 * that represents all of the appropriate positions for each node in the
 * tree passed in.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string code;
    buildEncodingHelper(encodingTree, encodingMap, code);
    return encodingMap;             // this is just a placeholder so it will compile
}

/**
 * @brief encodeData
 * @param input
 * @param encodingMap
 * @param output
 * encodeData takes in a input and outputs the bits into the obistream
 * according to the encodngMap passed in.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    char c = input.get();
    string code;
    while(c != -1){
        code = encodingMap.get(c);
        for(int i = 0; i < code.length(); i++){
            output.writeBit(stringToInteger(charToString(code[i])));
        }
        c = input.get();
    }
    code = encodingMap.get(PSEUDO_EOF);
    for(int i = 0; i < code.length(); i++){
        output.writeBit(stringToInteger(charToString(code[i])));
    }
}

/**
 * @brief decodeDataHelper
 * @param input
 * @param encodingTree
 * @return
 * This is the helper method for decodeData.
 * It returns the character that the input matches
 * accorind to the encoding Tree passed in.
 */
int decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree) {
    if(!encodingTree->isLeaf()){
        int current = input.readBit();
        if(current == 0){
            return decodeDataHelper(input, encodingTree->zero);
        }else if(current == 1){
            return decodeDataHelper(input, encodingTree->one);
        }
    }else {
        return encodingTree->character;
    }
}

/**
 * @brief decodeData
 * @param input
 * @param encodingTree
 * @param output
 * decodData uncompresses a stream that has been compressed according to the
 * encodingTree.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    while(true || input.peek() != -1){
        int current = decodeDataHelper(input, encodingTree);
        if(current != PSEUDO_EOF){
            output << charToString(current);
        }else break;
    }
}

/**
 * @brief compress
 * @param input
 * @param output
 * compress takes in an input and ouputs the Huffman encoded version.
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
 * @brief decompress
 * @param input
 * @param output
 * decompress takes in a stream and outputs the decoded version
 * of the file accoding to the map at the beggining of the bitstream.
 */
void decompress(ibitstream& input, ostream& output) {
    if(input.peek() != -1){
        Map<int, int> freqTable;
        input >> freqTable;
        HuffmanNode* encodingTree = buildEncodingTree(freqTable);
        decodeData(input, encodingTree, output);
        freeTree(encodingTree);
    }
}
/**
 * @brief freeTree
 * @param node
 * freeTree takes in a node and frees all of the memory attached
 * to the node.
 */
void freeTree(HuffmanNode* node) {
    if(node->zero!= NULL){
        freeTree(node->zero);
    }
    if(node->one!= NULL){
        freeTree(node->one);
    }
    delete node;
}
