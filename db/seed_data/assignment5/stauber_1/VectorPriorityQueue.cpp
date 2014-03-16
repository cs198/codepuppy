//File: VectorPriorityQueue.h
//Author: Mark Stauber
//Description: Contains the method definitions of VectorPriorityQueue
//  that implements a vector priority queue using an unsorted vector.

#include "VectorPriorityQueue.h"
#include "strlib.h"

VectorPriorityQueue::VectorPriorityQueue() {
}

VectorPriorityQueue::~VectorPriorityQueue() {
}

void VectorPriorityQueue::changePriority(string value, int newPriority) {
    if(pqueue.size() == 0) throw("Empty Queue");

    bool found = false; //Flag to see if entry with value is found.
    for(int i = 0; i < pqueue.size(); i++){
        if(pqueue[i].value == value){
            if(pqueue[i].priority <= newPriority) throw("Already high priority");

            pqueue[i].priority = newPriority;
            found = true;
            break;
        }
    }

    if(!found) throw("Value not contained in Queue");
}

void VectorPriorityQueue::clear() {
    pqueue.clear();
}

string VectorPriorityQueue::dequeue() {
    if(pqueue.size() == 0) throw("Empty Queue");

    //Get entry with lowest priority, save its value, then delete it.
    int first = getFirst();
    string result = pqueue[first].value;
    pqueue.remove(first);

    return result;
}

void VectorPriorityQueue::enqueue(string value, int priority) {
    //Add new entry to pqueue vector.
    PQEntry entry(value, priority);
    pqueue.add(entry);
}

bool VectorPriorityQueue::isEmpty() const {
    return pqueue.size() > 0;
}

string VectorPriorityQueue::peek() const {
    if(pqueue.size() < 1) throw("Empty Queue");

    int first = getFirst();
    return pqueue[first].value;
}

int VectorPriorityQueue::peekPriority() const {
    if(pqueue.size() == 0) throw("Empty Queue");

    int first = getFirst();
    return pqueue[first].priority;
}

int VectorPriorityQueue::size() const {
    return pqueue.size();
}

int VectorPriorityQueue::getFirst() const{
    //Start min as the first entry in pqueue vector
    int index = 0;
    PQEntry min = pqueue[index];

    //Search for index of min entry in pqueue vector.
    for(int i = 1; i < pqueue.size(); i++){
        if(pqueue[i] < min){
            min = pqueue[i];
            index = i;
        }
    }
    return index;
}

ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    bool first = true; //Flag to handle first entry.
    out<<"{";
    //Print all entries
    for(int i = 0; i < queue.size(); i++){
        if(first){
            out<<queue.pqueue[i];
            first = false;
        }else{
            out<<", ";
            out<<queue.pqueue[i];
        }
    }
    out<<"}";
    return out;
}
