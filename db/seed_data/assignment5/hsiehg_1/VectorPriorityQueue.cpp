// VectorPriorityQueue.cpp
// This file contains the implementation of the VectorPriorityQueue structure.

#include "VectorPriorityQueue.h"
#include "PQEntry.h"

/*
 * Vector Priority Queue constructor - empty
 */
VectorPriorityQueue::VectorPriorityQueue() {
}

/*
 * Empty - Not necessary since no memory was dynamically allocated in this class
 */
VectorPriorityQueue::~VectorPriorityQueue() {
}

/*
 * Changes the priority of a given string value to the newPriority.
 * Loops through the vector to check if the value exists. If the value exists and the original priority is more urgent than or equal
 * to the new priority, then the element's priority will be changed to the new priority. However, a string exception will be thrown
 * if the new priority is least urgent than the original priority.
 */
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 0; i < priorityVec.size(); i++) {
        if(priorityVec.get(i).value == value) {
            if(newPriority <= priorityVec.get(i).priority) {
                priorityVec.set(i, PQEntry(value, newPriority));
                break;
            } else {
                throw "Error: new priority cannot be greater than old priority";
            }
        }
    }
}

/*
 * Removes all elements in the queue.
 */
void VectorPriorityQueue::clear() {
    while(priorityVec.size() > 0) {
        priorityVec.remove(0);
    }
}

/*
 * Returns the index of the most urgent entry in the queue and sets maxEntry (passed by reference) equal to the most urgent entry.
 * If the priority queue is empty, function throws a string exception.
 */
int VectorPriorityQueue::getMaxEntry(PQEntry& maxEntry) const {
    if (priorityVec.size() > 0) {
        maxEntry = priorityVec.get(0);
        int index = 0;
        if(priorityVec.size() > 1) {
            for (int i = 1; i < priorityVec.size(); i++){
                if(priorityVec.get(i) < maxEntry) {
                    maxEntry = priorityVec.get(i);
                    index = i;
                }
            }
        }
        return index;
    } else {
        throw "Error: priority queue is empty";
    }
}

/*
 * Removes the entry with the highest priority number (highest urgency) and returns the string value of that entry.
 */
string VectorPriorityQueue::dequeue() {
    if(priorityVec.size() > 0) {
        PQEntry maxEntry ("", 0);
        int index = getMaxEntry(maxEntry);
        priorityVec.remove(index);
        return maxEntry.value;
    } else {
        throw "Error: priority queue is empty";
    }
}

/*
 * Adds a new entry into the queue by adding a PQEntry given the string value and int priority to the end of the vector.
 */
void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry newEntry (value, priority);
    priorityVec.add(newEntry);
}

/*
 * Returns true if the queue does not contain any entries.
 */
bool VectorPriorityQueue::isEmpty() const {
   return priorityVec.size() == 0;
}

/*
 * Returns the string value of the most urgent entry in the queue.
 */
string VectorPriorityQueue::peek() const {
    if(priorityVec.size() == 0) {
        throw "Error: priority queue is empty";
    }
    PQEntry maxEntry("", 0);
    getMaxEntry(maxEntry);
    return maxEntry.value;
}

/*
 * Returns the priority of the most urgent entry in the queue.
 */
int VectorPriorityQueue::peekPriority() const {
    if(priorityVec.size() == 0) {
        throw "Error: priority queue is empty";
    }
    PQEntry maxEntry("", 0);
    getMaxEntry(maxEntry);
    return maxEntry.priority;}

/*
 * Returns the number of elements inside the queue (size of the vector).
 */
int VectorPriorityQueue::size() const {
    return priorityVec.size();
}

/*
 * Overrides the << operator so that VectorPriorityQueue can be printed onto the console.
 */
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << queue.priorityVec.toString();
    return out;
}
