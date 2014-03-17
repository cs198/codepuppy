//.h file for HeapPriorityQueue.h

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    PQEntry* elements;
    int m_size;
    int capacity;

    void checkResize();
    string toString() const;
    void bubbleUp(int level);
    void bubbleDown();
    //void bubbleDown(int level);
    void swapElements(int a, int b);
    int findValue(string value);
};

#endif
