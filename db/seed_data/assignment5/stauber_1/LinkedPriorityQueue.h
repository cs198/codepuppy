//File: LinkedPriorityQueue.h
//Author: Mark Stauber
//Description: Contains the class of LinkedPriorityQueue
//  that implements a priority queue using a linked list.
//  Each ListNode data consists of a string value
//  and int priority. The linked list priority queue is efficient in
//  peek and dequeue are O(1), but enqueue is O(N).

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
class LinkedPriorityQueue {
public:
    //O(1)
    LinkedPriorityQueue();
    //O(1)
    ~LinkedPriorityQueue();
    //O(1)
    void changePriority(string value, int newPriority);
    //O(1)
    void clear();
    //O(1)
    string dequeue();
    //O(N)
    void enqueue(string value, int priority);
    //O(1)
    bool isEmpty() const;
    //O(1)
    string peek() const;
    //O(1)
    int peekPriority() const;
    //O(N)
    int size() const;
    //O(N)
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);

private:
    //Pointer to first entry which is also lowest priority.
    ListNode* front;
};

#endif
