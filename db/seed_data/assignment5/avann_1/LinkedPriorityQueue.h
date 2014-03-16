//This is the header file for the Linked List implementation of
//a priority queue for CS106B Assignment 5.

#ifndef _linkedpriorityqueue_h
#define _linkedpriorityqueue_h

#include <cstddef>    // for NULL
#include <iostream>
#include <string>
#include "ListNode.h"
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
//A pointer the front of the Linked List is the only private variable
//declared for the LinkedPriorityQueue class. There is a private fuction
//getFront to return the front pointer to allow the list to be printed.
class LinkedPriorityQueue {
public:
    LinkedPriorityQueue();
    ~LinkedPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);

private:
    ListNode* front;
    ListNode* getFront() const;
};

#endif
