/*
 * this is the implementation of a vector priority queue
 * unqueuing is extremely fast, but dequeuing and changing prioroty and peeking take much longer
 * because we need to pass over the whole vector
 */

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
    Vector<PQEntry> queue;
}

VectorPriorityQueue::~VectorPriorityQueue() {
    queue.clear();
}

//0(N)
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    int exists = 0; //starts a counter to see if the value is in the vector

    //iterates over the vector
    for (int i = 0 ; i < size() ; i++){
        if (queue[i].value == value){ //if we found the value
            if (queue[i].priority > newPriority){ //if the new prioroty is more urgent
                queue[i].priority = newPriority; //change it
                return;
            } else{
                throw "new priority entered is lower";
            }
            exists++;
        }
    }
    if (exists == 0){
        throw "doesn't exists please give me a real value";
    }
}

//0(1)
void VectorPriorityQueue::clear() {
    queue.clear();
}

//0(N)
string VectorPriorityQueue::dequeue() {
    PQEntry min = queue[0]; //initalizes the min value
    int index = 0;
    int entryIndex;
    //iterates over the vector
    for (PQEntry entry: queue){
        if (entry <= min){ //if the entry is inferior or equal
            min = entry; //sets it to min
            entryIndex = index; //remembers the index (to delete later)
        }
        index++;
    }

    queue.remove(entryIndex); //dequues at remembered index

    return min.value;
}

//0(1)
void VectorPriorityQueue::enqueue(string value, int priority) {

    //initializes it
    PQEntry entry;
    entry.priority = priority;
    entry.value = value;
    queue.add(entry); //adds it
}

//0(1)
bool VectorPriorityQueue::isEmpty() const {
    return queue.isEmpty();
}

//0(N)
string VectorPriorityQueue::peek() const {
    PQEntry min = queue[1];
    //iterates over the vector
    for (int i = 0 ; i < queue.size() ; i++){
        if (queue[i] < min){
            min = queue[i]; //remembers the min value
        }
    }
    return min.value;
}

//0(N)
int VectorPriorityQueue::peekPriority() const {
    PQEntry min = queue[1];
    //iterates over the vector
    for (int i = 0 ; i < queue.size() ; i++){
        if (queue[i] < min){
            min = queue[i]; //gets the min entry
        }
    }
    return min.priority;
}

//0(1)
int VectorPriorityQueue::size() const {
    return queue.size();
}

ostream& operator<<(ostream& out, const VectorPriorityQueue& Queue) {
    out << "{";
    if (!Queue.isEmpty()) {
        out << Queue.queue[0];
        for (int i = 1; i < Queue.size(); i++) {
            out << " , " << Queue.queue[i];
        }
    }
    out << "}";
    return out;
}
