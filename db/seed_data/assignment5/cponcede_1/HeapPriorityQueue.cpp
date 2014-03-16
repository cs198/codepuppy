/*File: HeapPriorityQueue.cpp
 * Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * -----------------------------------
 * The implementation for a priority queue that uses a binary heap style array to
 * store its elements. Each value at index i in the heap has children at
 * indices 2*i and 2*i+1. The heap is ordered so that no child is more
 * urgent that its parent. This implementation makes getting the most urgent value very easy, as
 * the most urgent value is always at the start of the heap. Enqueueing and dequeueing
 * are more complicated with this implementation, however, because you have to "bubble up"
 * or "bubble down" your values until they are in the correct location.
 */

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    capacity = 10;
    internalSize = 0;
    elements = new PQEntry[capacity];

}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] elements;

}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    PQEntry entry(value,newPriority);
    int index = -1;
    for (int i = 1 ; i <= internalSize ; i++) {
        if (value == elements[i].value) {
            if (newPriority >= elements[i].priority) {
                throw "Error: The new priority was not smaller than the old one.";
            }
            elements[i] = entry;
            index = i;
            break;
        }
    }
    if (index == -1) {
        throw "Error: The value was not present in the queue.";
    }
    bubbleUp(entry,index);

}

void HeapPriorityQueue::clear() {
    PQEntry* temp = elements;
    delete[] temp;
    internalSize = 0;
    capacity = 10;
    elements = new PQEntry[capacity];
}

string HeapPriorityQueue::dequeue() {
    if (internalSize == 0) {
        throw "The queue is empty.";
    }
    string result = elements[1].value;
    if (internalSize == 1) {
        clear();
    } else {
        elements[1] = elements[internalSize];
        internalSize--;
        bubbleDown(elements[1],1);
    }
    return result;
}

void HeapPriorityQueue::bubbleDown(PQEntry entry, int index) {
    if (index*2 > internalSize) {
        //Do nothing
        return;
    }
    PQEntry child1 = elements[index*2];
    if (index*2+1 > internalSize && isPriorTo(child1,entry)) {
        elements[index] = child1;
        elements[index*2] = entry;
        return;
    }
    PQEntry child2 = elements[index*2+1];
    if (isPriorTo(child1,entry) || isPriorTo(child2,entry)) {
        if (isPriorTo(child1,child2)) {
            elements[index] = child1;
            elements[index*2] = entry;
            bubbleDown(entry,index*2);
        } else {
            elements[index] = child2;
            elements[index*2+1] = entry;
            bubbleDown(entry,index*2+1);
        }
    }

}

bool HeapPriorityQueue::isPriorTo(PQEntry entry1, PQEntry entry2) {
    return entry1.priority < entry2.priority || (entry1.priority==entry2.priority && entry1.value < entry2.value);
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    checkResize();
    PQEntry entry(value,priority);
    elements[1+internalSize] = entry;
    bubbleUp(entry,1+internalSize);
    internalSize++;

}

void HeapPriorityQueue::bubbleUp(PQEntry entry,int index) {
    if (index == 1) {
        //Do nothing
    } else if (isPriorTo(entry,elements[index/2])) {
        elements[index] = elements[index/2];
        elements[index/2] = entry;
        bubbleUp(entry,index/2);
    }
}

bool HeapPriorityQueue::isEmpty() const {
    return (internalSize == 0);
}

string HeapPriorityQueue::peek() const {
    if (isEmpty()) {
        throw "Error: The queue is empty.";
    }
    return elements[1].value;
}

int HeapPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw "Error: The queue is empty.";
    }
    return elements[1].priority;
}

int HeapPriorityQueue::size() const {
    return internalSize;
}

void HeapPriorityQueue::checkResize() {
    //Adapted from ArrayList lecture
    if (internalSize+1 == capacity) {
        PQEntry* bigger;
        bigger = new PQEntry[2 * capacity]();
        for (int i = 0; i <= internalSize; i++) {
            bigger[i] = elements[i];
        }
        delete[] elements;
        elements = bigger;
        capacity *= 2;
    }
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    for (int i = 1 ; i <= queue.internalSize ; i++) {
        out << queue.elements[i];
        if (i != queue.internalSize) {
            out << ", ";
        }
    }
    out << "}";
    return out;
}
