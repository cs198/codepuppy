/* Connie Zeng
 * CS 106B, Winter 2014
 * -----------------------------
 * Assignment 5: Priority Queue
 * This file implements the HeapPriorityQueue class described in
 * HeapPriorityQueue.h.
 *
 * (1 late day)
 */

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    heapSize = 0;
    capacity = 10;
    PQHeap = new PQEntry[capacity]();   // empty array
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] PQHeap;
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    bool found = false;
    for(int i = 1; i <= heapSize; i++){
        if(PQHeap[i].value == value){
            found = true;

            // Changes priority of value and bubbles up to proper location
            if(PQHeap[i].priority > newPriority){
                PQHeap[i].priority = newPriority;
                bubbleUp(i);
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

void HeapPriorityQueue::clear() {
    heapSize = 0;
}

string HeapPriorityQueue::dequeue() {
    checkEmpty();

    // Replaces first element with last element and bubbles down to proper location
    string val = PQHeap[1].value;
    PQHeap[1] = PQHeap[heapSize];
    PQHeap[heapSize] = PQEntry();   // empty index
    heapSize--;
    bubbleDown(1);
    return val;
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    checkResize();
    heapSize++;

    // Adds element to end and bubbles up to proper location
    PQEntry elem(value, priority);
    PQHeap[heapSize] = elem;
    bubbleUp(heapSize);
}

bool HeapPriorityQueue::isEmpty() const {
    return heapSize == 0;
}

string HeapPriorityQueue::peek() const {
    checkEmpty();
    return PQHeap[1].value;
}

int HeapPriorityQueue::peekPriority() const {
    checkEmpty();
    return PQHeap[1].priority;
}

int HeapPriorityQueue::size() const {
    return heapSize;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if(!queue.isEmpty()){
        out << queue.PQHeap[1];
        for(int i = 2; i <= queue.size(); i++){
            out << ", " << queue.PQHeap[i];
        }
    }
    out << "}";
    return out;
}

void HeapPriorityQueue::bubbleUp(int index){
    if(index > 1 && PQHeap[index] < PQHeap[index / 2]){
        swap(PQHeap[index], PQHeap[index / 2]);
        bubbleUp(index / 2);
    }
}

void HeapPriorityQueue::bubbleDown(int index){
    if(index <= heapSize / 2){
        int child = 2 * index;
        if(child < heapSize && PQHeap[child + 1] < PQHeap[child]){
            child++;
        }
        if(PQHeap[index] > PQHeap[child]){
            swap(PQHeap[index], PQHeap[child]);
            bubbleDown(child);
        }
    }
}

void HeapPriorityQueue::checkEmpty() const{
    if(isEmpty()){
        throw "Error: priority queue is empty";
    }
}

void HeapPriorityQueue::checkResize(){
    if(heapSize == (capacity - 1)){
        capacity *= 2;
        PQEntry* old = PQHeap;
        PQHeap = new PQEntry[capacity]();
        for(int i = 1; i <= heapSize; i++){
            PQHeap[i] = old[i];
        }
        delete[] old;
    }
}
