// This is the CPP file for the Huffman Encoding Assignment 6 for CS106B.
// I used the handouts and slides from lecture and YEAH hours.
// I received help from the LAIR to fix a problem with the recursive calls in decodeData.
// I spoke to Tomoki Eto about the compress and decompress functions.

#include "encoding.h"
#include "pqueue.h"
#include "HuffmanNode.h"
#include "filelib.h"

void buildEncodingMapHelp(Map<int, string>& encodingMap, HuffmanNode* encodingTree, string huffocde);
bool decodeDataHelp(int bit, ibitstream& input, HuffmanNode* current, ostream& output);

/* The buildFrequencyTable function takes an input file or string and creates
 * a map of the characters in the file or string as keys to the number of
 * times the character appears in the file or string as values. When the file
 * or string is completed, it adds an 'EOF' character and stops reading
 * the file or string.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;   // this is just a placeholder so it will compile
    int byte;
    while(true){
        byte = input.get();
        if(byte==-1){
            freqTable.put(PSEUDO_EOF , 1);
            break;
        }
        if(!freqTable.containsKey(byte)){
            freqTable.put(byte, 1);
        }else{
            int count = freqTable.get(byte) + 1;
            freqTable.put(byte, count);
        }
    }
    return freqTable;          // this is just a placeholder so it will compile
}

/* The buildEncodingTree function first creates a priority queue containing
 * HuffmanNodes of all the characters with priority determined by their
 * count in the string or file. While the priority queue has a size greater
 * than 1, two nodes are dequeued from the queue, and a new node is created
 * with an empty character. This node is re-added to the queue with the added
 * priority of the dequeued nodes. The final, top node of the tree is returned.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;

    for(int character: freqTable){
        HuffmanNode* node = new HuffmanNode(character, freqTable.get(character));
        pq.enqueue(node, freqTable.get(character));
    }

    if(pq.size()==1){
            HuffmanNode* node = pq.dequeue();
            return node;
    }

    HuffmanNode* finalnode;

    while(pq.size() > 1){ //in case it's empty
        HuffmanNode* node1 = pq.dequeue();
        HuffmanNode* node2 = pq.dequeue();

        int newpriority = node1->count + node2->count;

        HuffmanNode* newnode = new HuffmanNode(NOT_A_CHAR, newpriority, node1, node2);

        pq.enqueue(newnode, newpriority);

        finalnode = newnode;
        cout << "char: " << finalnode-> character << "count: " << finalnode->count << endl;
    }

    return finalnode;   // this is just a placeholder so it will compile
}

/* The buildEncodingMap function creates an encoding map using the encoding tree.
 * The encoding map contains keys that correspond to characters and values that
 * correspond to each characters' huffman code. It calls buildEncodingMapHelp to
 * create the encoding map.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;   // this is just a placeholder so it will compile
    string huffcode;
    buildEncodingMapHelp(encodingMap, encodingTree, huffcode);
    return encodingMap;             // this is just a placeholder so it will compile
}

/* The buildEncodingMapHelp function is called by the buildEncodingMap. It traverses
 * the encoding Tree, creating each character's huffcode as it does. When the node is
 * not a leaf, it moves left, adding a zero each time its path is to the left, and it
 * adds a one each time its path is to the right. When the node is a leaf, it adds
 * the node's character and huffocde to the encoding map.
 */
void buildEncodingMapHelp(Map<int, string>& encodingMap, HuffmanNode* encodingTree, string huffcode){
    if(!encodingTree->isLeaf()){
        buildEncodingMapHelp(encodingMap, encodingTree->zero, huffcode +="0");
        buildEncodingMapHelp(encodingMap, encodingTree->one, huffcode.substr(0, huffcode.length()-1) +="1");
    }else{
        encodingMap.put(encodingTree->character, huffcode);
    }
}

/* The encodeData function reads the bytes from the input file and finds the string
 * code associated with the byte in the encoding map. It adds each bit of the string
 * code to the output.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int byte;
    while(true){
        byte = input.get();
        if(byte==-1){
            string code = encodingMap.get(PSEUDO_EOF);
            for(char c: code){
                int bit = c - '0';
                output.writeBit(bit);
            }
            break;
        }
        string code = encodingMap.get(byte);
        for(char c: code){
            int bit = c -'0';
            output.writeBit(bit);
        }
    }
}

/* The decodeDate function takes an input of bits from the user. It puts the bits of
 * the input into a string, which is passed to the decodeDataHelp function. With the
 * help of the decodeDataHelp function, which uses the encodingTree, decodeData creates
 * an output of characters based on the input.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
   int bit;
   while(true){
        bit = input.readBit();
        if(bit==-1){
            break;
        }

        bool tocontinue = decodeDataHelp(bit, input, encodingTree, output);
        if(!tocontinue) break;
    }
}

/* The decodeDataHelp function is responsible for actually creating the output in
 * decodeData. Depending on the current first bit of the input string, the current node
 * turns left or right on the encoding tree. When the current node is a leaf, its character
 * (if it is not EOF) is put in the output, and the process restarts at the root.
 * Otherwise, the function continues its path down the encoding tree depending on the
 * next bit.
 */
bool decodeDataHelp(int bit, ibitstream& input, HuffmanNode* current, ostream& output){
    if(bit==-1){
        return false;
    }

    if(bit==0){
        current = current->zero;
    }else if(bit==1){
        current = current->one;
    }

    if(current->isLeaf()){
        if(current->character != PSEUDO_EOF){
            output.put(current->character);
        }else{
            return false;
        }
        return true;
    }

    return decodeDataHelp(input.readBit(), input, current, output);
}

/* The compress function compresses an input stream into an output. It puts the frequency
 * table at the start of the output, and then includes the rest of the encoded input
 * in the compression.
 */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    output << freqTable;
    rewindStream(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap =  buildEncodingMap(encodingTree);
    freeTree(encodingTree);
    encodeData(input, encodingMap, output);
}

/* The decompress function takes the compressed output from compress as input and it
 * outputs the original unencoded file.
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

/* The freeTree function frees the nodes of an encoding tree.
 */
void freeTree(HuffmanNode* node) {
    if(node != NULL){
        if(node->isLeaf()){
            delete node;
            node = NULL;
        }else{
            freeTree(node->zero);
            freeTree(node->one);
        }
    }
}
