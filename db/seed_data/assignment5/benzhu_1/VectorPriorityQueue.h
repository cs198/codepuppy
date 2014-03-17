//Name: Benzheng Zhu
//CS 106B Assignment 5
//Citation: lecture slides


#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;

//This implementation is simple to write and optimized for fast enqueuing
//but has slow dequeue/peeking and poor overall performance.
class VectorPriorityQueue {
public:
    VectorPriorityQueue();//O(1)
    ~VectorPriorityQueue();
    void changePriority(string value, int newPriority); //O(N)
    void clear();//O(1)
    string dequeue(); //O(N)
    void enqueue(string value, int priority);
    bool isEmpty() const; //O(1)
    string peek() const; //O(N)
    int peekPriority() const; //O(N)
    int size() const;//O(1)
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue); //O(N)

private:
    Vector<PQEntry> vector;
    int peekHelper() const;

};

#endif
