/*
 * This is the linked list implementation of the priority queue. It uses listNodes stored
 * in sorted order to keep track of entries and priorities. This makes dequeueing and peeking
 * efficient, but changing priority and enqueueing inefficient. This implementation outperforms
 * the Vector priority queue.
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
    // O(1)
    LinkedPriorityQueue();

    // O(N)
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
    ListNode* front;

    /*
     * This is a helper for the changePriority function. It moves the node to the proper
     * position in the linked list after its priority has been updated.
     *
     * O(N)
     */
    void rearrangeNodes(ListNode* curr);

};

#endif
