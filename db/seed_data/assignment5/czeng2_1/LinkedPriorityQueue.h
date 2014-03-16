/* Connie Zeng
 * CS 106B, Winter 2014
 * -----------------------------
 * Assignment 5: Priority Queue
 * This program implements a priority queue of strings, using a linked list for
 * internal storage.
 *
 * (1 late day)
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
 * A piority queue is a collection that enqueues elements with priority ratings,
 * so that the elements are dequeued in order of priority. A LinkedPriorityQueue
 * stores elements in a sorted linked list, from most urgent to least urgent.
 * Elements are inserted in the proper order as they are enqueued. This results
 * in slower enqueuing but efficient dequeuing/peeking.
 *
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class LinkedPriorityQueue {
public:
    /* O(1) */
    LinkedPriorityQueue();

    /* O(N) */
    ~LinkedPriorityQueue();

    /* O(N) */
    void changePriority(string value, int newPriority);

    /* O(N) */
    void clear();

    /* O(1) */
    string dequeue();

    /* O(N) */
    void enqueue(string value, int priority);

    /* O(1) */
    bool isEmpty() const;

    /* O(1) */
    string peek() const;

    /* O(1) */
    int peekPriority() const;

    /* O(N) */
    int size() const;

    /* O(N) */
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);

private:
    ListNode* front;    // pointer to front of list

    /*
     * Throws an exception if the priority queue is empty. Called in
     * changePriority, dequeue, peek, and peekPriority methods.
     *
     * O(1)
     */
    void checkEmpty() const;
};

#endif
