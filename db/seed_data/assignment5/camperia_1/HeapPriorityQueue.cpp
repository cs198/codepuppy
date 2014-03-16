/*
 * heap implementation of priority queue
 * faster enqueuing than list because only log(n)
 * dequeuing is slightly longer because we need to move values but still fast
 * changing is still long because we need to iterate over the whle heap to find the value
 * peeking very fast
 * overall more efficient
 * basically a binary search tree
 */

#include "HeapPriorityQueue.h"
#include "PQEntry.h"

HeapPriorityQueue::HeapPriorityQueue() {
    capacity = 10;
    elements = new PQEntry[capacity];
    Size = 0;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[]elements;
}

//0(Nlog(N)) because we iterate over the whole vector and then bubble it up
void HeapPriorityQueue::changePriority(string value, int newPriority) {

    int exists = 0; //counter to see if it's present in the vector
    for (int i = 1; i <= Size; i++) { //iterates over the vectors
        if (elements[i].value == value) { //if we found the value
            if (elements[i].priority <= newPriority){
                throw "you have to change to a higher priority";
            } else {
                elements[i].priority = newPriority;
                bubbleUp(i); //buubles the value up with the new priority
                exists ++;
            }
        }
    }
    if (exists == 0){
        throw "the value does not exist in the heap";
    }

}

//0(N)
void HeapPriorityQueue::clear() {

    for (int i = 1 ; i <= Size ; i++){ //need to iterate over the whole vector
        elements[i] = PQEntry();
    }
    Size = 0;

}

//0(log(N))
string HeapPriorityQueue::dequeue() {
    if (Size == 0) throw "the heap is empty";
    PQEntry entry = elements[1]; //gets the most urgent entry
    elements[1] = elements[Size]; //brings down the lowest priority elements
    elements[Size] = PQEntry(); //deletes the last entry
    Size --;
    bubbleDown(1); //bubbles the new first entry down
    return entry.value;
}

//0(logN)
void HeapPriorityQueue::bubbleDown(int location){

    //various tests to see where it will go (2i or 2i+1)
    if (location*2+1 <= Size && elements[location] > elements[location*2] && elements[location*2] <= elements[location*2+1]){

        PQEntry temp = elements[location*2];
        //swaps them
        elements[location*2] = elements[location];
        elements[location] = temp;
        bubbleUp(location*2); //recursively checks if needs to move down more

    } else if (location*2+1 <= Size && elements[location] > elements[location*2+1] && elements[location*2+1] < elements[location*2]){

        PQEntry temp = elements[location*2+1];
        //swaps them
        elements[location*2+1] = elements[location];
        elements[location] = temp;
        bubbleUp(location*2+1); //recursively checks if needs to move down more

    }
}

//0(logN)
void HeapPriorityQueue::enqueue(string value, int priority) {

    PQEntry entry(value , priority); //initializes new entry

    //if we need to make the capacity bigger
    if (Size == capacity-1){

        capacity *= 2; //multiply the capacity by 2
        PQEntry* oldElements = elements; //set a new heap equal to the old
        elements = new PQEntry[capacity]; //changes the size of the elements heap to twice the size (and it's empty now)
        for (int i = 0 ; i <= Size ; i++){
            //add each old element to larger heap
            elements[i] = oldElements[i];
        }
        delete[] oldElements; //delete the old element set
    }

    elements[Size+1] = entry;
    Size += 1;
    if (Size > 1){ //only need to bubble if it's bigger than 1
        int location = Size;
        bubbleUp(location); //bubbles it up
    }

}

//0(logN)
void HeapPriorityQueue::bubbleUp(int location){

    if (elements[location] < elements[location/2] && location > 1){ //if it needs to be bubbled

        PQEntry temp = elements[location/2];
        //swaps them
        elements[location/2] = elements[location];
        elements[location] = temp;
        bubbleUp(location/2); //recursively checks if needs to move up more

    }
}

//0(1)
bool HeapPriorityQueue::isEmpty() const {
    return Size == 0;
}

//0(1)
string HeapPriorityQueue::peek() const {
    if (Size == 0) throw "heap is empty";
    return elements[1].value;
}

//0(1)
int HeapPriorityQueue::peekPriority() const {
    if (Size == 0) throw "heap is empty";
    return elements[1].priority;
}

//0(1)
int HeapPriorityQueue::size() const {
    return Size;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {
        out << queue.elements[0];
        for (int i = 1; i <= queue.Size; i++) {
            out << " , " << queue.elements[i];
        }
    }
    out << "}";
    return out;
}
