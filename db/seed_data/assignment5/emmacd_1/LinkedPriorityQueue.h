/*
 * Header file for LinkedPriorityQueue
 */

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
    LinkedPriorityQueue();
    ~LinkedPriorityQueue();

    // O(N)
    void changePriority(string value, int newPriority);

    // O(N)
    void clear();

    // O(1)
    string dequeue();

    // O(N)
    void enqueue(string value, int priority);

    // O(1)
    bool isEmpty() const;

    // O(1)
    string peek() const;

    // O(1)
    int peekPriority() const;

    // O(N)
    int size() const;

    // O(N)
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);

private:
    ListNode* front = NULL;

};

#endif
