//Name: Benzheng Zhu
//CS 106B Assignment 5
//Citation: lecture slides

#ifndef _linkedpriorityqueue_h
#define _linkedpriorityqueue_h

#include <cstddef>    // for NULL
#include <iostream>
#include <string>
#include "ListNode.h"
#include "PQEntry.h"
using namespace std;

//This implementation is harder to write than the vector implementation, and enqueuing to it is slower
//but it is much faster for dequeue/peeking and has better overall performance.

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class LinkedPriorityQueue {
public:
    LinkedPriorityQueue(); //O(1)
    ~LinkedPriorityQueue(); //O(1)
    void changePriority(string value, int newPriority); //O(N)
    void clear();//O(N)
    string dequeue();//O(1)
    void enqueue(string value, int priority);//O(N)
    bool isEmpty() const;//O(1)
    string peek() const; //O(1)
    int peekPriority() const;//O(1)
    int size() const; //O(N)
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);//O(N)

private:
    ListNode* front;

};

#endif
