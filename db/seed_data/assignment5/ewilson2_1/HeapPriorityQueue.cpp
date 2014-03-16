// This file contains the heap implementation of a priority queue

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    queue=new PQEntry[capacity];
    queueSize=0;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    PQEntry* p=queue;
    for(int i=0; i<capacity; i++) {
        delete p;
        p++;
    }
}

//This method changes the priority of a given entry, and reorders the array to accomodate
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    for(int i=1; i<queueSize+1; i++) {
        PQEntry entry=queue[i];
        if(entry.value==value) {
            PQEntry newEntry=PQEntry(value, newPriority);
            queue[i]=newEntry;
            bubbleUp(i);
            break;
        }
    }
}

//This method resets the contents of the array
void HeapPriorityQueue::clear() {
    queueSize=0;
}

//This method returns the value of the most urgent entry, and deletes it from the queue, reordering appropriately
string HeapPriorityQueue::dequeue() {
    if(queueSize==0) {
        throw "This queue is empty";
    } else {
        PQEntry entry=queue[1];
        queue[1]=queue[queueSize];
        queue[queueSize]=PQEntry();
        queueSize--;
        int parentIndex=1;
        bubbleDown(parentIndex);
        return entry.value;
    }
}

//This method implements "bubble down" ordering, by ensuring that no parent is less urgent than its children
void HeapPriorityQueue::bubbleDown(int index) {
    int childIndex2;
    int childIndex;
    PQEntry parent=queue[index];
    if(index*2<=queueSize) {
        childIndex=index*2;
    }
    if(index*2+1<=queueSize) {
        childIndex2=index*2+1;
        PQEntry child2=queue[childIndex2];
        PQEntry child=queue[childIndex];
        if(child2.priority<child.priority) {
            childIndex=childIndex2;
        } else if (child2.priority==child.priority) {
            if(child2.value<child.value) {
                childIndex=childIndex2;
            }
        }
    }
    if(index*2<=queueSize) {
        PQEntry parent=queue[index];
        PQEntry child=queue[childIndex];
        if(parent.priority>child.priority) {
            queue[index]=child;
            queue[childIndex]=parent;
            bubbleDown(childIndex);
        }
    }
}

//This method adds an entry to the queue, and ensures that it is in the proper location
void HeapPriorityQueue::enqueue(string value, int priority) {
    if(queueSize==capacity-1) {
        capacity=2*capacity;
        PQEntry* queue2;
        queue2=new PQEntry[capacity];
        for(int i=1; i<=queueSize; i++) {
            queue2[i]=queue[i];
        }
        queue=queue2;
    }
    PQEntry entry=PQEntry(value, priority);
    if(queueSize==0) {
       queue[1]=entry;
       queueSize++;
    } else {
       queueSize++;
       int index=queueSize;
       queue[index]=entry;
       bubbleUp(index);
    }
}

//This method implements "bubble up" ordering, by ensuring that no entry is of greater urgency than its parent
void HeapPriorityQueue::bubbleUp(int index) {
    PQEntry child=queue[index];
    int parentIndex=index/2;
    PQEntry parent=queue[parentIndex];
    if(child.priority==parent.priority) {
        if(child.value<parent.value) {
            queue[parentIndex]=child;
            queue[index]=parent;
            bubbleUp(parentIndex);
        }
    } else if(child.priority<parent.priority) {
        queue[parentIndex]=child;
        queue[index]=parent;
        bubbleUp(parentIndex);
    }
}

//This method returns true if the queue is empty, and false if it contains any entries
bool HeapPriorityQueue::isEmpty() const {
    if(queueSize==0) {
        return true;
    }
    return false;
}

//This method returns the value of the most urgent entry, without removing it from the queue
string HeapPriorityQueue::peek() const {
    if(queueSize==0) {
        throw "This queue is empty";
    } else {
        PQEntry entry=queue[1];
        return entry.value;
    }
}

//This method returns the priority of the most urgent entry, without removing from the queue
int HeapPriorityQueue::peekPriority() const {
    if(queueSize==0) {
        throw "This queue is empty";
    } else {
        PQEntry entry=queue[1];
        return entry.priority;
    }
}

//This method returns the size of the queue
int HeapPriorityQueue::size() const {
    return queueSize;
}

//This method overloads the "<<" operator to output the contents of the queue
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    cout << "{";
    for(int i=1; i<queue.queueSize+1; i++) {
        PQEntry entry=queue.queue[i];
        cout << "\"";
        cout << entry.value;
        cout << "\":";
        cout << entry.priority;
        cout << " ";
    }
    cout << "}" << endl;
    return out;
}