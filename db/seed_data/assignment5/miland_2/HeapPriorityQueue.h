/**
 * Name: Milan Doshi
 * Assignment: HeapPriority.h
 * TA: Jimmy Lee
 *
 * This file is the header file for HeapPriorityQueue.cpp
 */
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
    int capacity;
    int e_size;
    void bubbleUp(int index);
    void bubbleDown(int index);
    void checkResize();
};

#endif
