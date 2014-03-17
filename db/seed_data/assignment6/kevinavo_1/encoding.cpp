/* Name: Kevin Vo
 * TA: Jimmy Lee
   This program implements a file compression algorithm known
   as Huffman encoding. The idea of Huffman encoding is to leverage
   the frequency of characters, where more frequent characters get mapped
   to few byte size, while less frequent characters get mapped to greater
   byte size. The overall result is a file that takes up fewer space.
   Citations:
   http://www.siggraph.org/education/materials/HyperGraph/video/mpeg/mpegfaq/huffman_tutorial.html
   http://mitpress.mit.edu/sicp/full-text/sicp/book/node41.html
   http://en.wikipedia.org/wiki/Huffman_coding
*/

#include "encoding.h"
#include "pqueue.h"
#include "bitstream.h"
#include "strlib.h"
#include "filelib.h"


/*This function creates a mapping of each character to the frequency
of the character in the file size. The function returns the frequency
map of ints to ints.*/
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while (true){
        int next_char = input.get();
        if (next_char == EOF) break;
        if (freqTable.containsKey(next_char)){
            int value = freqTable.get(next_char);
            value++;
            freqTable.put(next_char,value);
        }else{
            freqTable.put(next_char,1);
        }
    }
    freqTable.put(PSEUDO_EOF,1);
    return freqTable;
}
/*This function takes in the frequency map and creates an encoding tree.
It can be broken into two steps.
1) Loop over the map and add HuffmanNode pointers to a priority queue
2) Iterate over the priority queue. Take the two highest priority nodes
and combine them with a parent node "NOT_A_CHAR" that has a priority
equal to the sum of the children's priority. It returns a pointer to the
root of the tree.*/
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> PQ;
    for (int character : freqTable){
        int priority = freqTable.get(character);
        HuffmanNode* newNode = new HuffmanNode();
        newNode->character = character;
        newNode->count = priority;
        PQ.enqueue(newNode,priority);
    }
    while(PQ.size() > 1){
        HuffmanNode* firstEntry = PQ.dequeue();
        HuffmanNode* secondEntry = PQ.dequeue();

        int sum = firstEntry->count + secondEntry->count;

        HuffmanNode* combination = new HuffmanNode;

        combination->character = NOT_A_CHAR;
        combination->count = sum;

        combination->zero = firstEntry;
        combination->one = secondEntry;

        PQ.enqueue(combination,sum);
    }
    return (PQ.peek());
}
/*This takes in the encoding map and uses it to create another mapping of characters (represented
as ints) and binary representation. I use recursive backtracking to implement DFS. I assume that characters
are at leaf nodes. I use a string to represent the path taken. The string becomes the binary encoding.
0 represents the left path and a 1 represents a right path. This is the helper function.*/
void buildEncodingMapHelper(HuffmanNode* encodingTree, Map<int,string>& encodingMap, string& path){
    if (encodingTree->isLeaf()){
        encodingMap.put(encodingTree->character,path);
    }else{
        path += "0";
        buildEncodingMapHelper(encodingTree->zero, encodingMap, path);
        path = path.substr(0,path.size()-1);
        path += "1";
        buildEncodingMapHelper(encodingTree->one, encodingMap, path);
        path = path.substr(0,path.size()-1);
    }
}
/*This is the actual function that builds the map that maps characters to the string representation
of the binary encoding.*/
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string path;
    buildEncodingMapHelper(encodingTree,encodingMap,path);
    return encodingMap;
}
/*This function takes in the encoding map of characters to binary representations and writes
the bit to the output stream. There are two loops. The first takes care of all the characters.
the last for loop takes care of the pseudo_eof case.*/
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while (true){
        int next_char = input.get();
        if (next_char == EOF) break;
        string binary = encodingMap.get(next_char);
        for (int i = 0 ; i < binary.length(); i++){
            int bNumber = stringToInteger(binary.substr(i,1));
            output.writeBit(bNumber);
        }        
    }
    //EOF case
    string ending = encodingMap.get(PSEUDO_EOF);
    for (int i = 0; i < ending.length(); i++){
        int bNumber = stringToInteger(ending.substr(i,1));
        output.writeBit(bNumber);
    }
}
/*The following two functions represent the decoding portion of the program. It uses the input sream
as a "map" to recursively go through the tree to print out the characters. The characters are written
to output stream. The bitString represents the pathway to get to a character. I store the root node
and pass it by copy to reset the pointer back to the root of the tree when the search reaches a character.
The helper function returns an integer. So every time the helper function reaches a leaf it exits and then
the while loop retriggers the helper function until the input stream hits EOF*/
int decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output, string& bitString, HuffmanNode* root){
    if (encodingTree->isLeaf()){
        int eChar = encodingTree->character;
        if (eChar != PSEUDO_EOF){
            output.put(encodingTree->character);
        }
        bitString = "";
        encodingTree = root;
        return eChar;
    }else{
        if (input.readBit() == 0){
            bitString += "0";
            return decodeDataHelper(input,encodingTree->zero,output,bitString, root);
        }else{
            bitString += "1";
            return decodeDataHelper(input,encodingTree->one, output, bitString, root);
        }
    }
}
/*This is the actual function which utlizes an empty while loop to retrigger the helper function
until the EOF pattern is read on the input stream.*/
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    string bitString;
    while (decodeDataHelper(input, encodingTree, output, bitString, encodingTree) != PSEUDO_EOF);
}
/*This compression function reads in the frequency map, which is the header of the
file. It then utiliizes the functions written previously to encode the data. Rewinding
the stream is important because teh buildFrequencyTable table iterates through the stream.
We want the pointer to be back in the beginning when encoding the data.*/
void compress(istream& input, obitstream& output){
    Map <int, int> frequencyTable = buildFrequencyTable(input);
    output << frequencyTable;
    HuffmanNode* node = buildEncodingTree(frequencyTable);
    Map<int, string> map = buildEncodingMap(node);
    rewindStream(input);
    encodeData(input, map, output);
}
/*This function reads the map from the file, which is represented
by input >> map. It then takes what is in the output stream and
decodes it, using the decodeData function which writes the output
stream.*/
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> map;
    input >> map;
    HuffmanNode* node = buildEncodingTree(map);
    decodeData(input, node, output);
}
/*This freeTree function takes in the node which is the root of the
tree. My strategy was to recurse down the right side of the tree and delete
every node there. Then go down the left side of the tree and delete everything
there. The last step was to delete the root of the tree.*/
void freeTree(HuffmanNode* node) {
    while (true){
        if (node == NULL) break;
        if (node->one != NULL){
            freeTree(node->one);
            node->one = NULL;
        } else if (node->zero != NULL){
            freeTree(node->zero);
            node->zero = NULL;
        }else{
            node = NULL;
            return;
        }
    }
}
