// This program implements priority queue using vector.

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
    vector = {};

}

VectorPriorityQueue::~VectorPriorityQueue() {
    clear();
}

void VectorPriorityQueue::changePriority(string value, int newPriority) {
    int index = -1;
    int min = NULL;
    //Loop through the vector to find the pqentry with the same value and the least existing priority
    for(int i=0; i<vector.size(); i++){
        if(vector[i].value == value) {
            if(min == NULL || min > vector[i].priority){
                min = vector[i].priority;
                index = i;
            }
        }
    }
    //if index is still -1 means there is no such value found, throw exception. If the pqentry found already
    //contains lower or equal priority, throw exception. otherwise, change the pqentry's priority to new one.
    if(index == -1) {
        throw "The priority queue does not contain such value.";
    } else if(min <= newPriority) {
        throw "Current priority is already as urgent as or more urgent than the new priority.";
    } else {
        vector[index].priority = newPriority;
    }
}

void VectorPriorityQueue::clear() {
    vector.clear();

}

string VectorPriorityQueue::dequeue() {
    if(vector.isEmpty()) throw "The priority queue is empty.";
    int index = findUrgentest();
    PQEntry urgentest = vector[index];
    vector.remove(index);
    return urgentest.value;
}

//Finds the most urgent item and return its index in the vector. If the vector is empty, it returns -1.
int VectorPriorityQueue:: findUrgentest() const {
    if(!vector.isEmpty()) {
        int index = 0;
        PQEntry urgentest = vector[0];
        for(int i=1; i<vector.size(); i++){
            if(vector[i] < urgentest) {
                index = i;
                urgentest = vector[i];
            }
        }
        return index;
    }
    return -1;
}

void VectorPriorityQueue::enqueue(string value, int priority) {
    vector.add(PQEntry(value,priority));

}

bool VectorPriorityQueue::isEmpty() const {
    return vector.isEmpty();
}

string VectorPriorityQueue::peek() const {
    if(vector.isEmpty()) throw "The priority queue is empty.";
    return vector[findUrgentest()].value;
}

int VectorPriorityQueue::peekPriority() const {
    if(vector.isEmpty()) throw "The priority queue is empty.";
    return vector[findUrgentest()].priority;
}

int VectorPriorityQueue::size() const {
    return vector.size();
}

Vector<PQEntry> VectorPriorityQueue:: getQueue() const {
    return vector;
}

//Print the vector that carries the priority queue. It will not display order.
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    Vector<PQEntry> vector = queue.getQueue();
    if(!vector.isEmpty()) {
        out<< "\"" << vector[0].value <<"\":"<< vector[0].priority;
        for(int i=1; i<vector.size(); i++) {
            out << ", " << "\"" <<vector[i].value << "\":"<< vector[i].priority;
        }
    }
    out << "}";
    return out;
}
