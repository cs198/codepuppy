// LinkedPriorityQueue.h
// This file contains the implementation of the LinkedPriorityQueue structure.

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
    LinkedPriorityQueue(); //Constructor
    ~LinkedPriorityQueue(); //Destructor
    void changePriority(string value, int newPriority); //Changes the priority of an element in the queue
    void clear(); //Removes all elements
    string dequeue(); //Removes element with most urgent priority and returns the string value of that element
    void enqueue(string value, int priority); //Adds an element to the queue
    bool isEmpty() const; //Returns true if queue is empty
    string peek() const; //Returns string value of element with most urgent priority
    int peekPriority() const; //Returns integer priority of element with most urgent priority
    int size() const; //Returns number of elements in queue
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue); //Overrides << operator for printing to console

private:
    /*Private Instance Variables*/
    ListNode* front; //Points to the first element of the list

    /*Helper Methods*/
    ListNode* changedNode(string value, int newPriority); //Returns a pointer that points to the element that will be changed
    void changeFront(ListNode*& temp, int newPriority); //Helper function for changePriority - changes the front of the list
    bool changeMiddle(ListNode*& temp, ListNode*& cur, string value, int newPriority); //Helper function for changePriority - changes the middle of the list
    void changeEnd(ListNode*& temp, ListNode* cur, string value, int newPriority); //Helper function for changePriority - changes the end of the list
    void addToQueue(string value, int newPriority); //Adds elements to the queue (the middle and the end)
};

#endif
