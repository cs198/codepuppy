/* File: VectorPriorityQueue.cpp
 * Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * --------------------------------
 * The implementation for the VectorPriorityQeueue class.
 * This class creates a priority queue by storing the PQEntries in an
 * unordered Vector. This makes enqueueing very easy but it is difficult to
 * dequeue or peek because you have to search through the vector to
 * find the most urgent priority.
 */

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
    //Do nothing

}

VectorPriorityQueue::~VectorPriorityQueue() {
    data.clear();

}

void VectorPriorityQueue::changePriority(string desiredValue, int newPriority) {
    for (int i = 0 ; i < data.size() ; i++) {
        PQEntry entry = data.get(i);
        if (entry.value == desiredValue) {
            if (entry.priority < newPriority) {
                throw "The new priority was too large.";
            }
            data[i].priority = newPriority;
            return;
        }
    }
    throw "The value was never found.";
}

void VectorPriorityQueue::clear() {
    data.clear();
}

string VectorPriorityQueue::dequeue() {
    if (data.isEmpty()) {
        throw "The queue does not contain any elements.";
    }
    int min = INT_MAX;
    string minValue;
    int index = -1;
    for (int i = 0 ; i < data.size() ; i++) {
        if (data.get(i).priority < min || (data.get(i).priority == min && minValue > data.get(i).value)) {
            min = data.get(i).priority;
            minValue = data.get(i).value;
            index = i;
        }
    }
    data.remove(index);
    return minValue;
}

void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry entry(value,priority);
    data.add(entry);
}

bool VectorPriorityQueue::isEmpty() const {
    return data.isEmpty();
}

string VectorPriorityQueue::peek() const {
    if (data.isEmpty()) {
        throw "The queue does not contain any elements.";
    }
    int min = INT_MAX;
    string minValue;
    for (int i = 0 ; i < data.size() ; i++) {
        if (data.get(i).priority < min || (data.get(i).priority == min && minValue > data.get(i).value)) {
            min = data.get(i).priority;
            minValue = data.get(i).value;
        }
    }
    return minValue;
}

int VectorPriorityQueue::peekPriority() const {
    if (data.isEmpty()) {
        throw "The queue does not contain any elements.";
    }
    int min = INT_MAX;
    string minValue;
    for (int i = 0 ; i < data.size() ; i++) {
        if (data.get(i).priority < min || (data.get(i).priority == min && minValue > data.get(i).value)) {
            min = data.get(i).priority;
            minValue = data.get(i).value;
        }
    }
    return min;
}

int VectorPriorityQueue::size() const {
    return data.size();
}

ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << queue.data.toString();
    return out;
}
