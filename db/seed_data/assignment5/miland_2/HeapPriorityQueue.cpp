/**
 * Name: Milan Doshi
 * Assignment: HeapPriority.cpp
 * TA: Jimmy Lee
 *
 * This file uses a Heap to input, store, and organize information. All of the elements are stored in a dynamic array. This is the most efficient to go about this process.
 */

#include "HeapPriorityQueue.h"

const int startingSize = 10;

HeapPriorityQueue::HeapPriorityQueue() {
    capacity = startingSize;
    e_size = 0;
    elements = new PQEntry[startingSize];

}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete [] elements;

}

//Big-Oh: N
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    for(int i = 1; i < e_size + 1; i++){
        if(elements[i].value == value){
            if(newPriority > elements[i].priority) {
                throw "The priority must be higher than previously.";
            }
            elements[i].priority = newPriority;
            bubbleUp(i);
            return;
        }
    }
    throw "No such value exists in the queue.";
}

//Big-Oh: N
void HeapPriorityQueue::clear() {
    int sizeUsed = e_size;
    for(int i = 1; i < sizeUsed + 1; i++){
        elements[i] = PQEntry();
        e_size--;
    }

}

//Big-Oh: 1
string HeapPriorityQueue::dequeue() {
    if(e_size == 0) throw "The queue is empty.";
    string value = elements[1].value;
    elements[1] = elements[e_size];
    elements[e_size] = PQEntry();
    e_size--;
    bubbleDown(1);
    return value;
}

//Big-Oh: 1
void HeapPriorityQueue::enqueue(string value, int priority) {
    HeapPriorityQueue::checkResize();
    PQEntry entry = PQEntry(value, priority);
    elements[e_size+1] = entry;
    cout << "Enqueued " << "\"" << value << "\" with priority " << priority << endl;
    bubbleUp(e_size);
    e_size++;
    bubbleUp(e_size);
}

//Big-Oh: 1
bool HeapPriorityQueue::isEmpty() const {
    return e_size == 0;
}

//Big-Oh: 1
string HeapPriorityQueue::peek() const {
    if(e_size == 0) throw "The queue is empty.";
    return elements[1].value;
}

//Big-Oh: 1
int HeapPriorityQueue::peekPriority() const {
    if(e_size == 0) throw "The queue is empty.";
    return elements[1].priority;
}

//Big-Oh: 1
int HeapPriorityQueue::size() const {
    return e_size;
}

//Big-Oh: N
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    for(int i = 1; i < queue.e_size + 1; i++){
        if(i == queue.e_size ) {
            out << "\"";
            out << queue.elements[i].value;
            out << "\"";
            out << ":";
            out << queue.elements[i].priority;
        } else {
            out << "\"";
            out << queue.elements[i].value;
            out << "\"";
            out << ":";
            out << queue.elements[i].priority;
            out << ", ";
        }
     }
     out << "}";
     return out;
}

//Big-Oh: 1
void HeapPriorityQueue::bubbleUp(int index) {
    if(elements[index] < elements[(index)/2]) {
        PQEntry temp = elements[index];
        elements[index] = elements[(index)/2];
        elements[(index)/2] = temp;
        bubbleUp(index/2);
    }
}

//Big-Oh: 1
void HeapPriorityQueue::bubbleDown(int index) {
    if(e_size < 2*index+1 || (elements[index] < elements[2*index] && elements[index] < elements[2*index + 1])) {
        return;
    } else {
        if(elements[index] > elements[2*index] || elements[index] > elements[2*index + 1]) {
            if((elements[index] > elements[2*index]) && (elements[2*index] < elements[2*index + 1])) {
                PQEntry temp = elements[2*index];
                elements[2*index] = elements[index];
                elements[index] = temp;
                bubbleDown(2*index);
             } else if((elements[index] > elements[2*index + 1]) && (elements[2*index + 1] < elements[2*index])) {
                PQEntry temp = elements[2*index + 1];
                elements[2*index + 1] = elements[index];
                elements[index] = temp;
                bubbleDown(2*index+1);
             }
            return;
        }
    }
}

//Big-Oh: N
void HeapPriorityQueue::checkResize() {
    if(e_size == capacity) {
        PQEntry* bigger = new PQEntry[2 * capacity]();
        for(int i = 0; i < e_size ; i++) {
            bigger[i] = elements[i];
        }
        delete[] elements;
        elements = bigger;
        capacity *= 2;
    }
}
