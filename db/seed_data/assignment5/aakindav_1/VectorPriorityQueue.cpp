// VectorPriorityQueue.cpp implements the VectorPriorityQueue
// class behavior declared in VectorPriorityQueue.h
// See VectorPriorityQueue.h for documentation of each member

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
}

VectorPriorityQueue::~VectorPriorityQueue() {
}

void VectorPriorityQueue::changePriority(string value, int newPriority) {
    if (!vec.isEmpty()) {
        bool flag = false;
        for (int i=0; i<vec.size(); i++) {
            if (vec[i].value == value) {
                flag = true;
                if (vec[i].priority > newPriority) {
                    vec[i].priority = newPriority;
                } else {
                    throw "Must upgrade priority!";
                }
            }
        }
        if (!flag) throw "Value is not contained in Priority Queue!";
    } else throw "Queue is empty!";
}

void VectorPriorityQueue::clear() {
    vec.clear();
}

string VectorPriorityQueue::dequeue() {
    if (!vec.isEmpty()) {
        PQEntry min = vec[0];
        string lowest = min.value;
        int lowIndex;
        for (int i=1; i<vec.size(); i++) {
            if (vec[i] == min) {
                if (vec[i].value < min.value) {
                    min = vec[i];
                }
            }
            if (vec[i] < min ) {
                min = vec[i];
                lowest = min.value;
                lowIndex = i;
            }
        }
        vec.remove(lowIndex);
        return lowest;
    } else {
        throw "This Queue is empty!";
    }
}

void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry val (value,priority);
    vec.add(val);
}

bool VectorPriorityQueue::isEmpty() const {
    return vec.size() == 0;
}

string VectorPriorityQueue::peek() const {
    if (!vec.isEmpty()) {
        PQEntry min = vec[0];
        string lowest = min.value;
        for (int i=1; i<vec.size(); i++) {
            if (vec[i] == min) {
                if (vec[i].value < min.value) {
                    min = vec[i];
                }
            }
            if (vec[i] < min ) {
                min = vec[i];
                lowest = min.value;
            }
        }
        return lowest;
    } else {
        throw "This Queue is empty!";
    }
}

int VectorPriorityQueue::peekPriority() const {
    if (!vec.isEmpty()) {
        PQEntry min = vec[0];
        for (int i=1; i<vec.size(); i++) {
            if (vec[i] < min ) {
                min = vec[i];
            }
        }
        return min.priority;
    } else {
        throw "This Queue is empty!";
    }
}

int VectorPriorityQueue::size() const {
    return vec.size();
}

PQEntry VectorPriorityQueue::getEntry(int index) const {
    return vec[index];
}

ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    for (int i=0; i<queue.size(); i++) {
        out << queue.getEntry(i);
        if (i != queue.size()-1) out << ", ";
    }
    out << "}" << endl;
    return out;
}
