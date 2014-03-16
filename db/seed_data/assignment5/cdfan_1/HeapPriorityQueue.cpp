//This program implements priority queue using heap structure implemented in array.

#include "HeapPriorityQueue.h"

//constructor
HeapPriorityQueue::HeapPriorityQueue() {
    pQueue = new PQEntry[10];
    m_size = 0;
    capacity = 10;
}

//destructor
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] pQueue;
}

//search the matching value. If found and the new priority is smaller than its priority, change
//it into new priority, and bubble up. If not, throw exception. If the queue doesn't contain it, throw
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    bool found = false;
    for(int i=1; i<=m_size; i++) {
        if(pQueue[i].value == value && newPriority < pQueue[i].priority) {
            pQueue[i].priority = newPriority;
            bubbleUp(i);
            found = true;
            break;
        } else if(pQueue[i].value == value) {
            throw "Current priority is already as urgent as or more urgent than the new priority.";
        }
    }
    if(!found) throw "The priority queue does not contain such value.";
}

void HeapPriorityQueue::clear() {
    m_size = 0;
}

//dequeue the most urgent entry from the priority queue by returning the value of entry at index 1
// and replace index 1 with last value of array, and bubble down if necessary
string HeapPriorityQueue::dequeue() {
    if(m_size == 0) throw "The priority queue is empty.";
    PQEntry selected = pQueue[1];
    pQueue[1] = pQueue[m_size];
//    pQueue[size] = NULL;
    m_size--;
    bubbleDown(1);
    return selected.value;
}

void HeapPriorityQueue:: bubbleDown(int index) {
    if(m_size>1) {
        //while either child is smaller than the current entry, go into
        while((index*2 <= m_size && pQueue[index] > pQueue[index*2] )
              || (index*2+1 <= m_size && pQueue[index] > pQueue[index*2+1])) {

            //make sure this child exist within size,determine whether it's this child to bubble down
            //by comparing this child with the current entry and also with the other child.swap with
            //this child only when its priority is lower than the current and the other child.
            if(index*2 <= m_size &&pQueue[index] > pQueue[index*2]
                    && pQueue[index*2]< pQueue[index*2+1]) {
                swap(pQueue[index], pQueue[index*2]);
                index = index*2;

                //if the child at index*2+1 is smaller than the current and the index*2 child, swap
            } else if(index*2+1 <= m_size &&pQueue[index] > pQueue[index*2+1]) {
                swap(pQueue[index], pQueue[index*2+1]);
                index = index*2+1;
            }
        }
    }
}

//add an entry into the priority queue in the order of a heap structure.
void HeapPriorityQueue::enqueue(string value, int priority) {
    checkResize();
    pQueue[m_size+1] = PQEntry(value,priority);
    m_size++;
    bubbleUp(m_size);
}

//Given an index, it compares entry stored in index with entry stored in index/2. If the priority
//at index is smaller, swap with that with index/2, until such condition doesn't stand or it reaches
//index 1.
void HeapPriorityQueue:: bubbleUp(int index) {
    if(m_size>1) {
        while(index/2>0 && pQueue[index/2] > pQueue[index]) {
            swap(pQueue[index/2],pQueue[index]);
            index=index/2;
        }
    }
}

//Swap two entries on the array
void HeapPriorityQueue::swap(PQEntry & entry1, PQEntry & entry2) {
    PQEntry temp = entry1;
    entry1 = entry2;
    entry2 = temp;
}

bool HeapPriorityQueue::isEmpty() const {
    return m_size == 0;
}

//returns the most urgent value
string HeapPriorityQueue::peek() const {
    if(m_size == 0) throw "The priority queue is empty.";
    return pQueue[1].value;
}

//returns the most urgent priority
int HeapPriorityQueue::peekPriority() const {
    if(m_size == 0) throw "The priority queue is empty.";
    return pQueue[1].priority;
}

int HeapPriorityQueue::size() const {
    return m_size;
}

//check if the size reaches capacipty-1 due the empty slot at index 0. If so, double the capacity
void HeapPriorityQueue::checkResize() {
    if(m_size == capacity-1) {
        PQEntry *oldQueue = pQueue;
        pQueue = new PQEntry[capacity*2];
        for(int i=0; i<=m_size; i++) {
            pQueue[i] = oldQueue[i];
        }
        delete[] oldQueue;
        capacity *= 2;
    }
}

PQEntry* HeapPriorityQueue:: getArrayPointer() const {
    return pQueue;
}

//print out the priority queue in the heap struture order.
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if(queue.size()>0) {
        PQEntry *array = queue.getArrayPointer();
        out<< "\"" << array[1].value <<"\":"<< array[1].priority;
        for(int i=2; i<=queue.size(); i++) {
            out << ", " << "\"" <<array[i].value << "\":"<< array[i].priority;
        }
    }
    out << "}";
    return out;
}
