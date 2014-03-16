/*
 * Filename: VectorPriorityQueue.cpp - Matt Herrero
 * -------------------------------------------------------------------------------
 * This is the .cpp file for the vector implementation of the priorityQueue. This
 * class provides the functionality of a priorityQueue with the behavior described
 * in the assignment. This implementation keeps track of each entry using the PQEntry
 * struct provided and a Vector<PQEntry> to house each entry in a data structure. The
 * elements inside the data structure are not sorted, making for convenient enqueuing,
 * but prolonged and more demanding dequeuing and peeking.
 */

#include "VectorPriorityQueue.h"

/*
 * This is the constructor for the priorityQueue. Once the user decides to use this implementation will
 * be used for the priorityQueue, this method is called to initiate the private member values in this
 * object. For the Vector implementation, no values need to be initiated. For the ListNode implementation,
 * the front pointer is set to NULL (empty queue). For the Heap implementation, a new arrary of 10 places
 * for PQEntries is created in memory, the capacity is set to 9, and the size is set to 0. In all cases,
 * the constructor has a runtime O(1).
 */
VectorPriorityQueue::VectorPriorityQueue() {}

/*
 * This is the destructor for the priorityQueue. Once the priorityQueue falls out of scope in the main
 * program, the destructor is called to free up the dynamically allocated memory used in the object. No
 * objects used in the Vector implementation are dynamically allocated, which means the destructor does
 * nothing in the Vector implementation. In the ListNode implementation, the program clears all ListNodes
 * from the queue (pointer falls out of scope). In the Heap implementation, the destructor deletes all
 * values in the array.
 *
 * This implementation's method has a runtime of O(1).
 */
VectorPriorityQueue::~VectorPriorityQueue() {}

/*
 * This method modifies the priority of a given existing value in the queue. The method finds the first
 * occurance of the value in the queue by searching the queue from the beginning. If the value in the
 * queue already has a higher urgency (lower priority number of equal priority number and first alphabetically)
 * then the method throws a string exception. If the value does not exist in the queue, the method throws
 * a string exception. In neither of exceptions throw, the method replaces the entry as a new entry with
 * a new priority. In the ListNode and Heap implementations, this then forces the values in the queue
 * to rearrange based on the value's new priority (ListNode uses enqueue(), Heap uses bubbleUp()).
 *
 * This implementation's method has a runtime of O(N).
 */
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    bool valueFound = false;
    for(int i; i < queueVector.size(); i++) {
        if(queueVector.get(i).value == value) {
            if(queueVector.get(i).priority <= newPriority) {
                throw "Error: new priority cannot be greater than old priority";
            }else{
                valueFound = true;
                PQEntry newEntry(value, newPriority);
                queueVector.set(i, newEntry);
            }
        }
    }
    if(!valueFound) throw "Error: the given value is not found in this priority queue";
}

/*
 * This method clears the queue of all its values. For the ListNode implementation, the method iterates
 * through the queue, deleting each node, then set's the front pointer to NULL. In the Heap implementation,
 * the method deletes the existing array and reinitializes all the private member values to their
 * original states.
 *
 * This implementation's method has a runtime of O(1).
 */
void VectorPriorityQueue::clear() {
    queueVector.clear();
}

/*
 * This method removes the most urgent value in the list and returns its value. If the queue is empty, then
 * a string exception is thrown. For the Vector implementation, the Vector is searched for the most urgent
 * entry, the index of this entry is found, the value at this index is stored, the entry is removed, and
 * the value is returned. For the ListNode implementation, the value of the first node is stored, the
 * node is removed from the queue and deleted, and the value is returned. For the Heap implementation, the
 * the value of the first entry is stored, the last entry replaces the first, the heap is sorted using the
 * bubbleDown method, and the value is returned.
 *
 * This implementation's method has a runtime of O(N).
 */
string VectorPriorityQueue::dequeue() {
    if(isEmpty()) throw "Error: priority queue is empty";
    int mostUrgentInd = getMostUrgentInd();
    string nextValue = queueVector.get(mostUrgentInd).value;
    queueVector.remove(mostUrgentInd);
    return nextValue;
}

/*
 * This method iterates over the VectorPriorityQueue and returns the index of the most urgent entry in
 * the queue.
 *
 * This method has a run time of O(N).
 */
int VectorPriorityQueue::getMostUrgentInd() const{
    int mostUrgentInd = 0;
    for(int i = 1; i < queueVector.size(); i++) {
        if(queueVector.get(i) < queueVector.get(mostUrgentInd)) {
            mostUrgentInd = i;
        }
    }
    return mostUrgentInd;
}

/*
 * This method places a new entry into the queue with the data "value" and "priority." In the vector
 * implementation, the new entry is simply added to the end of the vector. In the ListNode implementation,
 * the queue is searched to find the appropriate place for the entry (place where the queue will still be
 * in most to least urgent order) and is inserted there. In the heap implementation, the new entry is placed
 * at the end of the heap and the bubbleUp method is used to sort the contents of the heap.
 *
 * This implementation's runtime is O(1).
 */
void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry nextEntry(value, priority);
    queueVector.add(nextEntry);
}

/*
 * This method returns true if the queue is empty.
 *
 * This implementation's method has a runtime of O(1).
 */
bool VectorPriorityQueue::isEmpty() const {
    return queueVector.isEmpty();
}

/*
 * This method "peeks" at the value of the next entry that will be dequeued from the list. If the queue
 * is empty, this method throws an exception.
 *
 * This implementation's method has a runtime of O(N).
 */
string VectorPriorityQueue::peek() const {
    if(isEmpty()) throw "Error: priority queue is empty";
    int mostUrgentInd = getMostUrgentInd();
    return queueVector.get(mostUrgentInd).value;
}

/*
 * This method "peeks" at the priority of the next entry that will be dequeued from the list. If the queue
 * is empty, this method throws an exception.
 *
 * This implementation's method has a runtime of O(N).
 */
int VectorPriorityQueue::peekPriority() const {
    if(isEmpty()) throw "Error: priority queue is empty";
    int mostUrgentInd = getMostUrgentInd();
    return queueVector.get(mostUrgentInd).priority;
}

/*
 * This method returns the current size of the queue.
 *
 * This implementation's method has a runtime of O(1).
 */
int VectorPriorityQueue::size() const {
    return queueVector.size();
}

/*
 * This method overloads the "<<" operator to print a properly formated version of the queue. If the queue
 * is empty, a pair of opened and closed brackets are printed. Otherwise, the method prints the value of each
 * entry in quotes followed by the priority of that entry.
 *
 * This implementation's method has a runtime of O(N).
 */
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    if(!queue.isEmpty()) {
        out << queue.queueVector.get(0);
        for (int i = 1; i < queue.size(); i++) {
            out << ", " << queue.queueVector.get(i);
        }
    }
    out << "}";
    return out;
}
