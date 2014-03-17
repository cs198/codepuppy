//File: HeapPriorityQueue.h
//Author: Mark Stauber
//Description: Contains the methods of HeapPriorityQueue class
//  that implements a heap priority queue using a binary heap
//  array.
#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    //Allocate pqueue as an array of 10 PQEntries to start.
    pqueue = new PQEntry[10]();
    m_size = 0;
    capacity = 10;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] pqueue;
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    //Flag to check if we find a matching entry.
    bool found = false;

    int index;
    for(index = 1; index < m_size + 1; index++){
        if(pqueue[index].value == value){
            if(pqueue[index].priority <= newPriority) throw("Already high priority");

            found = true;
            break;
        }
    }

    if(!found) throw("Value not contained in queue");

    //If it is found assign new priority and bubble up.
    pqueue[index].priority = newPriority;
    bubbleUp(index);
}

void HeapPriorityQueue::clear() {
    m_size = 0;
}

string HeapPriorityQueue::dequeue() {
    if(m_size < 1) throw("Empty Queue");

    string result = pqueue[1].value;
    pqueue[1] = pqueue[m_size];
    m_size--;
    bubbleDown();

    return result;
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    //Make sure there is room for a new entry in pqueue.
    checkResize();

    //Add on to the end of pqueue then bubbleUp.
    int index = m_size + 1;
    pqueue[index] = PQEntry(value, priority);
    bubbleUp(index);

    m_size++;
}

bool HeapPriorityQueue::isEmpty() const {
    return m_size < 1;
}

string HeapPriorityQueue::peek() const {
    if(m_size < 1) throw("Empty Queue");
    return pqueue[1].value;
}

int HeapPriorityQueue::peekPriority() const {
    if(m_size < 1) throw("Empty Queue");
    return pqueue[1].priority;
}

int HeapPriorityQueue::size() const {
    return m_size;
}

void HeapPriorityQueue::checkResize(){
    //If capacity is maxed out, double capacity
    if(m_size == capacity - 1){
        PQEntry* oldPqueue = pqueue;
        pqueue = new PQEntry[2 * capacity]();

        //Move old entries into new array.
        for(int i = 1; i < m_size + 1; i++)
            pqueue[i] = oldPqueue[i];

        delete[] oldPqueue;
        capacity *= 2;
    }
}

void HeapPriorityQueue::bubbleUp(int index){
    //Keep bubbling up until index is first entry or not more urgent than parent.
    while(index > 1 && pqueue[index] < pqueue[index / 2]){
        PQEntry temp = pqueue[index / 2];
        pqueue[index / 2] = pqueue[index];
        pqueue[index] = temp;
        index /= 2;
    }
}

void HeapPriorityQueue::bubbleDown(){
    //Keep bubbling down until index no more children
    //or no longer less urgent than children.
    int index = 1;
    while(index * 2 <= m_size){
        //If index has 2 children
        if(index * 2 + 1 <= m_size){
            PQEntry first = pqueue[index * 2];
            PQEntry second = pqueue[index * 2 + 1];
            //If first child is smaller than second and index.
            if(first < second && first < pqueue[index]){
                pqueue[index * 2] = pqueue[index];
                pqueue[index] = first;
                index = index * 2;
            }
            //else if second child is smaller than index.
           else if(second < pqueue[index]){
                pqueue[index * 2 + 1] = pqueue[index];
                pqueue[index] = second;
                index = index * 2 + 1;
            }
            //If neither is smaller than index then finished bubbledown.
            else break;
        }else{
            PQEntry child = pqueue[index * 2];
            if(child < pqueue[index]){
                pqueue[index * 2] = pqueue[index];
                pqueue[index] = child;
                index = index * 2;
            }else break;
        }
    }
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out<<"{";
    bool first = true; //Flag to handle first entry
    //Print out entries
    for(int i = 1; i < queue.m_size + 1; i++){
        if(first){
            out<<queue.pqueue[i];
            first = false;
        }else out<<", "<<queue.pqueue[i];
    }
    out<<"}";
    return out;
}
