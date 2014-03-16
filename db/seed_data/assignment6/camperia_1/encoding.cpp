/*
 * Huffman file encoding
 * Andrés Camperi
 * SL: Jimmy Lee
 */

#include "encoding.h"
#include "istream.h"
#include "pqueue.h"
#include "HuffmanNode.h"
#include "filelib.h"

void getChars(string , Map<int, int>&);
void encodingHelper(Map<int , string>& , HuffmanNode* , string);
int decodeHelper(int , ibitstream& , HuffmanNode*);


//method that build the frequency table
Map<int, int> buildFrequencyTable(istream& input) {

    //these next few lines get all the characters form the input file and put them into the frequency map
    Map<int, int> freqTable;
    int ch = input.get();
    while(ch != EOF){
        //gets all the chars from the file and puts them into the map
        freqTable[ch] += 1;
        ch = input.get();
    }
    freqTable[PSEUDO_EOF] = 1; //adds the end of file char
    return freqTable; //returns the map
}

//builds the encoding tree with the given frequency table
//there's a problem with the output here: it prints it all one one line but marty said that was an error with his code in class...
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {

    PriorityQueue<HuffmanNode*> queue; //initiates the priority queue
    //iterates over all the keys in the map (all the chars)
    for (int key : freqTable.keys()){
        //enqueues a new node with the char and the frequency as priority
        HuffmanNode* node = new HuffmanNode(key , freqTable[key]);
        queue.enqueue(node , freqTable[key]);
    }

    //dequeues the two most urgent nodes and creates a new one with both as children
    //re-enqueues the new one with a priority of the sum of theirs
    while (queue.size() > 1){
        HuffmanNode* left = queue.dequeue();
        HuffmanNode* right = queue.dequeue();
        HuffmanNode* node = new HuffmanNode(NOT_A_CHAR , (left->count+right->count) , left , right);
        queue.enqueue(node , node->count);
    }
    return queue.dequeue(); //returns a pointer to the only node: the root of the encoding tree
}

//builds the encoding map with the given pointer to the tree
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    string code = ""; //initiates an empty string
    Map<int, string> encodingMap;
    encodingHelper(encodingMap , encodingTree , code); //recursive helper function
    return encodingMap; //returns the map
}

//recursive helper function
void encodingHelper(Map<int , string>& encodingMap , HuffmanNode* encodingTree , string code){
    if (encodingTree->isLeaf()){ //if we've gotten to a leaf
        encodingMap[encodingTree->character] = code; //adds the character and code pairing to the tree
    } else{
        encodingHelper(encodingMap , encodingTree->zero , code+"0"); //recurses to the left
        encodingHelper(encodingMap , encodingTree->one , code+"1"); //recurses to the right
    }
}

//encodes the data to the output with the given encoding map
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {

    string encodedData; //initializes a string to store all the encoded characters

    //the next few lines get each byte from the input, search for it in the map and add the encoded letter into the string
    int letter = input.get();
    while (letter != -1){ //while we haven't reached the end of the input
        encodedData += encodingMap[letter];
        letter = input.get();
    }
    encodedData += encodingMap[PSEUDO_EOF]; //adds an EOF char after reading everything

    //the next few lines write each bit into the output
    for (int i = 0 ; i < encodedData.size() ; i++){
        output.writeBit(encodedData[i] - '0');
    }
}

//method to decode the data from an input and write it to the output
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {

    //reads each bit from the input
    int bit = input.readBit();
    while (bit != -1){ //while we haven't reached the end of the file
        int letter = decodeHelper(bit , input , encodingTree); //recursive helper - gets each letter
        if (letter != PSEUDO_EOF) output.put(letter); //writes it to the output if it isn't an EOF char
        bit = input.readBit(); //goes to the next bit
    }
}

//recursive helper function for decoding
int decodeHelper(int bit , ibitstream& input , HuffmanNode* encodingTree){
    if (bit == 0){ //if the bit is 0 we check the left child
        if (encodingTree->zero->isLeaf()){ //if it's a leaf return the character
            return encodingTree->zero->character;
        } else{ //calls itself on the left child and the next bit
            return decodeHelper(input.readBit() , input , encodingTree->zero);
        }
    } else if (bit == 1){ //if the bit is 1 we check the right child
        if (encodingTree->one->isLeaf()){ //if it's a leaf return it
            return encodingTree->one->character;
        } else{ //calls itself on the right child and the next bit
            return decodeHelper(input.readBit() , input , encodingTree->one);
        }
    } else{ //else returns the EOF char (for the end of the file with bit = -1)
        return PSEUDO_EOF;
    }
}

//global compress function
void compress(istream& input, obitstream& output) {
    Map <int , int> table = buildFrequencyTable(input); //creates the table
    HuffmanNode* front = buildEncodingTree(table); //pointer to the front of the created tree
    Map <int , string> encodingMap = buildEncodingMap(front); //makes the map
    output << table; //writes it to the output
    rewindStream(input); //rewinds the input to the beginning
    encodeData(input , encodingMap , output); //encodes it
}

//global decompress function
void decompress(ibitstream& input, ostream& output) {
    Map <int , int> table; //initializes the table
    input >> table; //gets it from the input
    HuffmanNode* front = buildEncodingTree(table); //builds the encoding tree with the table
    decodeData(input , front , output); //decodes the data and writes it to the output
}

//frees the memory associated with the tree
void freeTree(HuffmanNode* node) {
    //recursively runs through the tree calling itself on the two children before deleting itself
    if (node != NULL){ //only runs if the current node isn't null (so it stops)
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
