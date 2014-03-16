/*
 * Filename: HeapPriorityQueue.h - Matt Herrero
 * -------------------------------------------------------------------------------
 * This is the .h file for the Heap implementation of the priorityQueue. This
 * class provides the functionality of a priorityQueue with the behavior described
 * in the assignment. This implementation keeps track of each entry using the PQEntry
 * struct provided. In each place in the heap, a PQEntry exists with an urgency no more
 * urgent than it's parent entry (located at index/2), but no less urgent than its
 * 2 children entries (at index*2 ane index*2+1). When a new entry is added or an
 * entry is dequeued, the object uses a bubble up or bubble down sorting algorithm
 * to swap parent and child entries that have the incorrect urgency relationship
 * (parent less urgent than child) until the proper heap organization is found.
 * Because very similar algorithms are used in dequeuing and enqueuing, both methods
 * have similar runtimes and the same Big-Oh value.
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
    //This is the array that stores each entry.
    PQEntry* elements;
    //This stores the capacity of the array
    int capacity;
    //This stores the size of the array
    int queueSize;
    /*
     * This method goes through the sorting process of "bubbling up" an entry that may be at an lower urgency
     * index than it should be. It does this by checking its parent to see if it is less urgent. If it is, it
     * switches the parent with the child, then checks to see if the parent and child have both reached a final
     * spot by recursively calling the function in both these places.
     *
     * This recursive method has a runtime of O(N).
     */
    void bubbleUp(int index);
    /*
     * This method goes through the sorting process of "bubbling down" an entry that may be at an higher urgency
     * index than it should be. It does this by checking its children to see if either of them is more urgent.
     * If either is, it switches the parent with that child, then checks to see if the parent and child have
     * both reached a final spot by recursively calling the function in both these places.
     *
     * This recursive method has a runtime of O(N).
     */
    void bubbleDown(int index);
    /*
     * This method doubles the capacity of the array. It first creates a new array with double the capacity+1
     * (since the actual size of the array is capcity+1 bc index 0 not used, the actual sizes of the array will
     * be multiples of 10, but the capacity will always be one less than this). Then, it transfers all the values
     * to the new array. Finally, it deletes the old array.
     *
     * This method has a runtime of O(N).
     */
    void doubleCapacity();
};

#endif
