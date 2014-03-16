//Name: Benzheng Zhu
//CS 106B Assignment 5
//Citation: lecture slides

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
    vector.clear();
}

VectorPriorityQueue::~VectorPriorityQueue() {
    // TODO: implement

}

void VectorPriorityQueue::changePriority(string value, int newPriority) {
    bool flag = true;
    for (int i=0; i<vector.size();i++) {
        if (vector[i].value == value && vector[i].priority > newPriority) {
            vector[i].priority = newPriority;
            flag = false;
            break;
        }
    }
    if(flag) {
        throw "exception";
    }
}

void VectorPriorityQueue::clear() {
    vector.clear();
}

string VectorPriorityQueue::dequeue() {
    int index = peekHelper();
    string result = vector[index].value;
    vector.remove(index);
    return result;
}

void VectorPriorityQueue::enqueue(string value, int priority) {
    vector.add(PQEntry(value,priority));
    //    cout<<"Enqueued "<<"\""<<value<<"\""<<" with priority "<<priority<<endl;
}

bool VectorPriorityQueue::isEmpty() const {
    return vector.isEmpty();
}

string VectorPriorityQueue::peek() const {
    int index = peekHelper();
    return vector[index].value;
}

int VectorPriorityQueue::peekPriority() const {
    int index = peekHelper();
    return vector[index].priority;
}

int VectorPriorityQueue::size() const {
    return vector.size();
}

ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out<<"{";
    if (!queue.isEmpty()) {
        for (int i=0; i<queue.size()-1; i++) {
            out<<queue.vector[i]<<", ";
        }
        out<<queue.vector[queue.size()-1];
    }
    out<<"}";
    return out;
}

int VectorPriorityQueue::peekHelper() const {
    if (vector.isEmpty()) {
        throw "exception";
    } else {
        int temp = vector[0].priority;
        int index = 0;
        for (int i=0; i<vector.size(); i++) {
            if ((vector[i].priority <temp)|| //if priority is higher
                (vector[i].priority == temp && vector[i].value<vector[index].value) ) {
                //or priority is the same, but in alphabetical order
                temp = vector[i].priority;
                index = i;
            }
        }
        return index;
    }
}
