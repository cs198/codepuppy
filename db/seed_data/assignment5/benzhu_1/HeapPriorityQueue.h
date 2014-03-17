//Name: Benzheng Zhu
//CS 106B Assignment 5
//Citation: lecture slides


#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

//A key benefit of using a binary heap to represent a priority queue is efficiency. The common operations of enqueue
//and dequeue take only O(log N) time to perform, since the "bubbling" jumps by powers of 2 every time. The peek
//operation takes only O(1) time since the most urgent-priority element's location is always at index 1.

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
public:
    HeapPriorityQueue();//O(1)
    ~HeapPriorityQueue(); //O(1)
    void changePriority(string value, int newPriority); //O(Nlog N)
    void clear(); //O(1)
    string dequeue(); //O(log N)
    void enqueue(string value, int priority); //O(log N)
    bool isEmpty() const; //O(1)
    string peek() const; //O(1)
    int peekPriority() const; //O(1)
    int size() const; //O(1)
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue); //O(N)

private:
    int capacity;
    int m_size;
    PQEntry* root;

    void checkResize(); //O(N)
    int childIndex(int parent) const; //O(1)
    void BubbleUp(int child) const; //O(log N)
    void BubbleDown(int parent) const; //O(log N)

};

#endif
