/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * Vector Priority Queue cpp file
 * Stores its elements in an unsorted vector of PQEntries
 * Dequeuing and peeking are inefficient, and therefore
 * this data structure is inefficient for storing large amounts of data.
 */

#include "VectorPriorityQueue.h"
#include "PQEntry.h"
#include "vector.h"

// constructor
VectorPriorityQueue::VectorPriorityQueue() {
    /* Empty */
}

// destructor
VectorPriorityQueue::~VectorPriorityQueue() {
    /* Empty */
}

/*
 * Runs in O(N) time
 * parameter: string value of element to be changed, int new priority
 * changes the first occurance of the indicated value to the new priority
 * throws a string exception if the new priority is greater than or equal
 * to the old priority, or if the value doesn't exist in the queue.
 */
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 0; i < v.size(); i++) {
        if (v.get(i).value == value) {
            if (v.get(i).priority > newPriority) {
                PQEntry temp(value, newPriority);
                v.set(i, temp);
                return;
            }
            else {
                throw value;
            }
        }
    }
    throw value;
}

// Runs in O(1) time
// Removes all elements from the queue
void VectorPriorityQueue::clear() {
    v.clear();
}

/*
 * Runs in O(N) time
 * Removes the element with the most urgent priorty
 * returns: value of element with the most urgent priorty that was removed
 * throws a string exception if the queue is empty
 */
string VectorPriorityQueue::dequeue() {
    if (v.isEmpty()) {
        throw "Queue is empty";
    }
    int removeIndex = getLowestPriorityIndex();
    PQEntry minEntry = v.get(removeIndex);
    v.remove(removeIndex);
    return minEntry.value;
}

/*
 * Runs in O(N) time
 * returns: index of the highest urgency element in the queue
 */
int VectorPriorityQueue::getLowestPriorityIndex() const{
    PQEntry minEntry = v.get(0);
    int removeIndex = 0;
    for (int i = 1; i < v.size(); i++) {
        PQEntry entry = v.get(i);
        if (entry.priority < minEntry.priority ||
                (entry.priority == minEntry.priority && entry.value < minEntry.value)) {
            minEntry.priority = entry.priority;
            minEntry.value = entry.value;
            removeIndex = i;
        }
    }
    return removeIndex;
}

/*
 * Runs in O(1) time
 * parameter: new string value, new priority
 * adds the new entry to the end of the queue
 * precondition: parameters are all valid
 * duplicates are also added
 */
void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry temp(value, priority);
    v.add(temp);
}

// Runs in O(1) time
// returns: true if queue is empty, false if it is not empty
bool VectorPriorityQueue::isEmpty() const {
    if (v.isEmpty()) {
        return true;
    }
    return false;
}

/*
 * Runs in O(N) time
 * returns: string value of the element with the highest urgency
 * throws a string exception if queue is empty
 */
string VectorPriorityQueue::peek() const {
    if (v.isEmpty()) {
        throw "Queue is empty";
    }
    return v.get(getLowestPriorityIndex()).value;
}

/*
 * Runs in O(N) time
 * returns: int priority of the element with the highest urgency
 * throws a string exception if queue is empty
 */
int VectorPriorityQueue::peekPriority() const {
    if (v.isEmpty()) {
        throw "Queue is empty";
    }
    return v.get(getLowestPriorityIndex()).priority;
}

// Runs in O(1) time
// returns: size of queue
int VectorPriorityQueue::size() const {
    return v.size();
}

/*
 * helper function
 * Runs in O(1) time
 * parameter: desired int index
 * returns: PQEntry at the indicated index in the queue
 * If index is out of bounds, throws a string exception
 */
PQEntry VectorPriorityQueue::getEntryAt(int index) const {
    if (index < 0 || index >= v.size()) {
        throw "Index is out of Bounds";
    }
    return v.get(index);
}

/*
 * Runs in O(N) time
 * printing operator overload
 */
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    for (int i = 0; i < queue.size() - 1; i++) {
        out << queue.getEntryAt(i) << ", ";
    }
    if (queue.size() != 0) {
        out << queue.getEntryAt(queue.size()-1);
    }
    out << "}";
    return out;
}
