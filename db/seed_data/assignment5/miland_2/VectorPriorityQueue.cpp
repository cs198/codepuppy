/**
 * Name: Milan Doshi
 * Assignment: VectorPriorityQueue.cpp
 * TA: Jimmy Lee
 *
 * This file uses a Vector to input, store, and organize information. The vector is not as efficient as the linked list or the heap implementation.
 */

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {

}

VectorPriorityQueue::~VectorPriorityQueue() {

}

//Big-Oh: N
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    PQEntry current;
    int index;
    for(int i = 0; i < entries.size(); i++) {
        if(trim(this->entries.get(i).value) == trim(value)) {
            current = entries.get(i);
            index = i;
        }
    }
    if (current.value == "") {
        throw "No such value exists in the queue.";
        return;
    } else if (newPriority > entries.get(index).priority) {
        throw "The priority must be higher than previously.";
        return;
    } else {
        entries[index].priority = newPriority;
        PQEntry newEntry = PQEntry(value, newPriority);
        entries.remove(index);
        entries.insert(index, newEntry);
    }
}

//Big-Oh: N
void VectorPriorityQueue::clear() {
    for(int i = entries.size() - 1; i >= 0; i--) {
        entries.remove(i);
    }

}

//Big-Oh: N
string VectorPriorityQueue::dequeue() {
    if(entries.isEmpty()) {
        throw "The queue is empty.";
    }
    int index = 0;
    PQEntry min = entries.get(0);
    for(int i = 1; i < entries.size(); i++) {
        PQEntry currentPriority = entries.get(i);
        if(currentPriority < min) {
            min = currentPriority;
            index = i;
        }
    }
    entries.remove(index);
    return min.value;
}

//Big-Oh: 1
void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry newEntry = PQEntry(value, priority);
    entries.add(newEntry);
    cout << "Enqueued " << "\"" << value << "\" with priority " << priority << endl;

}

//Big-Oh: 1
bool VectorPriorityQueue::isEmpty() const {
    return entries.isEmpty();
}

//Big-Oh: N
string VectorPriorityQueue::peek() const {
    return trim(getPriorityEntry().value);
}

//Big-Oh: N
int VectorPriorityQueue::peekPriority() const {
    return getPriorityEntry().priority;
}

//Big-Oh: N
int VectorPriorityQueue::size() const {
    int size = 0;
    for(int i = 0; i < entries.size(); i++) {
        size++;
    }
    return size;
}

//Big-Oh: N
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {

    return out << queue.entries;
}

//Big-Oh: N
PQEntry VectorPriorityQueue:: getPriorityEntry() const {

    if(entries.isEmpty()) {
        throw "The queue is empty.";
    }
    int index = 0;
    //string value = "";
    PQEntry min = entries.get(0);
    for(int i = 1; i < entries.size(); i++) {
        PQEntry currentPriority = entries.get(i);
        if(currentPriority < min) {
            min = currentPriority;
            index = i;
        }
    }
    return min;
}
