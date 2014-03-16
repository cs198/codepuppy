/*File: HeapPriorityQueue.h
 * Student: Chris Ponce de Leon
 * Section Leader: Jimmy Lee
 * -----------------------------------
 * The interface for a priority queue that uses a binary heap style array to
 * store its elements. Each value at index i in the heap has children at
 * indices 2*i and 2*i+1. The heap is ordered so that no child is more
 * urgent that its parent. This implementation makes getting the most urgent value very easy, as
 * the most urgent value is always at the start of the heap. Enqueueing and dequeueing
 * are more complicated with this implementation, however, because you have to "bubble up"
 * or "bubble down" your values until they are in the correct location.
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

    //O(1)
    HeapPriorityQueue();

    //O(1)
    ~HeapPriorityQueue();

    //O(N)
    void changePriority(string value, int newPriority);

    //O(1)
    void clear();

    //O(logN)
    string dequeue();

    //O(N)
    void enqueue(string value, int priority);

    //O(1)
    bool isEmpty() const;

    //O(1)
    string peek() const;

    //O(1)
    int peekPriority() const;

    //O(1)
    int size() const;

    //O(N)
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    //Dynamically allocated array that stores the PQEntries that make up the heap.
    PQEntry* elements;

    //Size of elements array
    int capacity;

    //Number of elements in the priority queue.
    int internalSize;

    /* Checks whether the elements array is completely filled and, if so,
     * it doubles the size of the elements array to create more room.
     */
    //O(N)
    void checkResize();

    /* Moves a PQEntry form its current location to the proper location in the heap
     * by comparing its priority to that of its parent, which is located at index/2 in elements.
     * If it has a greater priority than its parent, the two swap places and bubbleUp. If not, it stays where it is.
     */
    //O(logN)
    void bubbleUp(PQEntry entry, int index);

    /* Moves a PQEntry that starts at the position, index, in elements and then looks at that
     * entry's two children. If one of the children has a more urgent priority than the original entry,
     * the two swap places and then bubbleUp calls itself at the new index of the original entry.
     */
    //O(logN)
    void bubbleDown(PQEntry entry, int index);

    /*A predicate function that returns true if entry1 is more urgent than entry2 and false if
     * it is not.
     */
    //O(1)
    bool isPriorTo(PQEntry entry1, PQEntry entry2);
};

#endif
