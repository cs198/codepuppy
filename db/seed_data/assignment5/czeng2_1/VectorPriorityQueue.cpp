/* Connie Zeng
 * CS 106B, Winter 2014
 * -----------------------------
 * Assignment 5: Priority Queue
 * This file implements the VectorPriorityQueue class described in
 * VectorPriorityQueue.h.
 *
 * (1 late day)
 */

#include "VectorPriorityQueue.h"


VectorPriorityQueue::VectorPriorityQueue() {
    // empty
}

VectorPriorityQueue::~VectorPriorityQueue() {
    // empty
}

void VectorPriorityQueue::changePriority(string value, int newPriority) {
    bool found = false;
    for(PQEntry& elem : PQVector){
        if(elem.value == value){
            found = true;

            // Changes priority of value
            if(elem.priority > newPriority){
                elem.priority = newPriority;
                return;
            }
        }
    }
    if(found == true){
        throw "Error: new priority cannot be greater than old priority";
    }
    else{
        throw "Error: The given value is not found in this priority queue";
    }
}

void VectorPriorityQueue::clear() {
    PQVector.clear();
}

string VectorPriorityQueue::dequeue() {
    checkEmpty();
    int index = findLowest();
    string val = PQVector[index].value;
    PQVector.remove(index);
    return val;
}

void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry elem(value, priority);
    PQVector.add(elem);
}

bool VectorPriorityQueue::isEmpty() const {
    return PQVector.isEmpty();
}

string VectorPriorityQueue::peek() const {
    checkEmpty();
    int index = findLowest();
    return PQVector[index].value;
}

int VectorPriorityQueue::peekPriority() const {
    checkEmpty();
    int index = findLowest();
    return PQVector[index].priority;
}

int VectorPriorityQueue::size() const {
    return PQVector.size();
}

ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << queue.PQVector;
    return out;
}

int VectorPriorityQueue::findLowest() const {
    int index = 0;
    PQEntry min = PQVector[0];
    for(int i = 0; i < PQVector.size(); i++){
        PQEntry curr = PQVector[i];
        if(curr < min){
            min = curr;
            index = i;
       }
    }
    return index;
}

void VectorPriorityQueue::checkEmpty() const {
    if(isEmpty()){
        throw "Error: priority queue is empty";
    }
}

