/*
 * This file implements the VectorPriorityQueue.h interface.
 */

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
}

VectorPriorityQueue::~VectorPriorityQueue() {
    v.clear();
}

void VectorPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 0; i < v.size(); i++) {
        PQEntry entry = v[i];
        if (value == v[i].value) {
            if (newPriority > v[i].priority) throw "cannot downgrade a priority!";
            v[i].priority = newPriority;
        }
    }
}

void VectorPriorityQueue::clear() {
    v.clear();
}

string VectorPriorityQueue::dequeue() {
    if (v.isEmpty()) return "Empty vector, cannot dequeue.";
    PQEntry min = v[0];
    int index = 0;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] < min) {
            min = v[i];
            index = i;
        }
    }
    v.remove(index);
    return min.value;
}

void VectorPriorityQueue::enqueue(string value, int priority) {
    v.add(PQEntry(value, priority));
    cout << "Enqueued \"" << value << "\" with priority " << integerToString(priority) << endl;
}

bool VectorPriorityQueue::isEmpty() const {
    if (v.isEmpty()) {
        return true;
    } else return false;
}

string VectorPriorityQueue::peek() const {
    if (v.isEmpty()) return "Empty vector, nothing to see.";
    PQEntry min = v[0];
    for (int i = 0; i < v.size(); i++) {
        if (v[i] < min) {
            min = v[i];
        }
    }
   return min.value;
}

int VectorPriorityQueue::peekPriority() const {
    if (v.isEmpty()) throw "Empty vector, nothing to see.";
    PQEntry min = v[0];
    for (int i = 0; i < v.size(); i++) {
        if (v[i] < min) {
            min = v[i];
        }
    }
   return min.priority;
}

int VectorPriorityQueue::size() const {
    return v.size();
}

ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    for (int i = 0; i < queue.v.size(); i++) {
        out << queue.v[i];
        if (i < queue.v.size() - 1) out << ", ";
    }
    cout << "}";
    return out;
}
