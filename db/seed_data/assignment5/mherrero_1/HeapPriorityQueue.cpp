/*
 * Filename: HeapPriorityQueue.cpp - Matt Herrero
 * -------------------------------------------------------------------------------
 * This is the .cpp file for the Heap implementation of the priorityQueue. This
 * class provides the functionality of a priorityQueue with the behavior described
 * in the assignment. This implementation keeps track of each entry using the PQEntry
 * struct provided. In each place in the heap, a PQEntry exists with an urgency no more
 * urgent than it's parent entry (located at index/2), but no less urgent than its
 * 2 children entries (at index*2 ane index*2+1). When a new entry is added or an
 * entry is dequeued, the object uses a bubble up or bubble down sorting algorithm
 * to swap parent and child entries that have the incorrect urgency relationship
 * (parent less urgent than child) until the proper heap organization is found.
 * Because very similar algorithms are used in dequeuing and enqueuing, both methods
 * have similar runtimes and the same Big-Oh value.
 */

#include "HeapPriorityQueue.h"

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
HeapPriorityQueue::HeapPriorityQueue() {
    elements = new PQEntry[10];
    capacity = 9;
    queueSize = 0;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] elements;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    int queueInd = 1;
    while(elements[queueInd].value != value) {
        queueInd++;
        if(queueInd > queueSize) throw "Error: the given value is not found in this priority queue";
    }
    PQEntry newEntry(value, newPriority);
    if(elements[queueInd] > newEntry) {
        elements[queueInd] = newEntry;
        bubbleUp(queueInd);
    } else {
        throw "Error: new priority cannot be greater than old priority";
    }
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
void HeapPriorityQueue::clear() {
    delete[] elements;
    elements = new PQEntry[10];
    capacity = 9;
    queueSize = 0;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
string HeapPriorityQueue::dequeue() {
    if(isEmpty()) throw "Error: priority queue is empty";
    string returnValue = elements[1].value;
    elements[1] = elements[queueSize];
    queueSize--;
    bubbleDown(1);
    return returnValue;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
void HeapPriorityQueue::enqueue(string value, int priority) {
    if(queueSize==capacity) {
        doubleCapacity();
    }
    PQEntry newEntry(value, priority);
    elements[queueSize+1] = newEntry;
    bubbleUp(queueSize+1);
    queueSize++;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(1).
 */
bool HeapPriorityQueue::isEmpty() const {
    return queueSize==0;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(1).
 */
string HeapPriorityQueue::peek() const {
    if(isEmpty()) throw "Error: priority queue is empty";
    return elements[1].value;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(1).
 */
int HeapPriorityQueue::peekPriority() const {
    if(isEmpty()) throw "Error: priority queue is empty";
    return elements[1].priority;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(1).
 */
int HeapPriorityQueue::size() const {
    return queueSize;
}

/*
 * This method goes through the sorting process of "bubbling up" an entry that may be at an lower urgency
 * index than it should be. It does this by checking its parent to see if it is less urgent. If it is, it
 * switches the parent with the child, then checks to see if the parent and child have both reached a final
 * spot by recursively calling the function in both these places.
 *
 * This recursive method has a runtime of O(N).
 */
void HeapPriorityQueue::bubbleUp(int index) {
    if(index/2 >= 1 && elements[index/2] > elements[index]) {
        PQEntry temp = elements[index];
        elements[index] = elements[index/2];
        elements[index/2] = temp;
        bubbleUp(index/2);
        bubbleUp(index);
    }
}

/*
 * This method goes through the sorting process of "bubbling down" an entry that may be at an higher urgency
 * index than it should be. It does this by checking its children to see if either of them is more urgent.
 * If either is, it switches the parent with that child, then checks to see if the parent and child have
 * both reached a final spot by recursively calling the function in both these places.
 *
 * This recursive method has a runtime of O(N).
 */
void HeapPriorityQueue::bubbleDown(int index) {
    if(index*2 <= queueSize && elements[index*2] < elements[index]) {
        PQEntry temp = elements[index];
        elements[index] = elements[index*2];
        elements[index*2] = temp;
        bubbleDown(index*2);
        bubbleDown(index);
    } else if(index*2+1 <= queueSize && elements[index*2+1] < elements[index]) {
        PQEntry temp = elements[index];
        elements[index] = elements[index*2+1];
        elements[index*2+1] = temp;
        bubbleDown(index*2+1);
        bubbleDown(index);
    }
}

/*
 * This method doubles the capacity of the array. It first creates a new array with double the capacity+1
 * (since the actual size of the array is capcity+1 bc index 0 not used, the actual sizes of the array will
 * be multiples of 10, but the capacity will always be one less than this). Then, it transfers all the values
 * to the new array. Finally, it deletes the old array.
 *
 * This method has a runtime of O(N).
 */
void HeapPriorityQueue::doubleCapacity() {
    PQEntry* newElements = new PQEntry[capacity*2+2];
    capacity = capacity*2+1;
    for(int i = 1; i <= queueSize; i++) {
        newElements[i] = elements[i];
    }
    delete[] elements;
    elements = newElements;
}

/*
 * See "VectorPriorityQueue.h" for documentation member functions.
 *
 * This implementation's method has a runtime of O(N).
 */
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if(!queue.isEmpty()) {
        out << "\"" << queue.elements[1].value << "\":" << queue.elements[1].priority;
        for(int i = 2; i <= queue.queueSize; i++) {
            out << ", \"" << queue.elements[i].value << "\":" << queue.elements[i].priority;
        }
    }
    out << "}";
    return out;
}
