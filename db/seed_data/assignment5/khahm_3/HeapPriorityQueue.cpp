/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * Heap Priority Queue cpp file
 * Stores its elements in an binary sorted array of PQEntries
 * All methods are efficient
 * Therefore, this is the most efficient data structure in this assignment.
 */

#include "HeapPriorityQueue.h"
#include "PQEntry.h"

// constructor
HeapPriorityQueue::HeapPriorityQueue() {
    listCapacity = 10;
    elements = new PQEntry[listCapacity + 1];
    listSize = 0;
}

// destructor
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] elements;
}

/*
 * Runs in O(N log N) time
 * parameter: string value of element to be changed, int new priority
 * changes the first occurance of the indicated value to the new priority
 * throws a string exception if the new priority is greater than or equal
 * to the old priority, or if the value doesn't exist in the queue.
 */
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    if (listSize == 0) {
        throw "Queue is empty";
    }
    for (int i = 1; i <= listSize; i++) {
        if (elements[i].value == value) {
            if (elements[i].priority > newPriority) {
                elements[i].priority = newPriority;
                bubbleUp(i);
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
void HeapPriorityQueue::clear() {
    listSize = 0;
}

/*
 * Runs in O(log N) time
 * Removes the element with the most urgent priorty
 * returns: value of element with the most urgent priorty that was removed
 * throws a string exception if the queue is empty
 */
string HeapPriorityQueue::dequeue() {
    if (listSize == 0) {
        throw "Queue is Empty";
    }
    string dequeueValue = elements[1].value;
    elements[1] = elements[listSize];
    listSize--;
    bubbleDown(1);
    return dequeueValue;
}

/*
 * helper function
 * Runs in O(1) time
 * Compares the entries at the two indeces.
 * If the first index is more urgent than the second index,
 * return true. Otherwise, return false.
 */
bool HeapPriorityQueue::isPriority(int index1, int index2) const {
    if (elements[index1].priority < elements[index2].priority) {
        return true;
    }
    else if (elements[index1].priority == elements[index2].priority) {
        if (elements[index1].value <= elements[index2].value) {
            return true;
        }
    }
    return false;
}

/*
 * helper function
 * Runs in O(1) time
 * Swaps the entries at the given indeces
 */
void HeapPriorityQueue::swapEntries(int index1, int index2) {
    PQEntry temp = elements[index1];
    elements[index1] = elements[index2];
    elements[index2] = temp;
}

/*
 * helper function
 * Runs in O(log N) time
 * Starting at the given index,
 * it bubbles down the entry at the index until its children are less urgent than itself
 */
void HeapPriorityQueue::bubbleDown(int index) {
    int child1 = 2 * index;
    int child2 = 2 * index + 1;
    bool child1Exists = (child1 <= listSize && isPriority(child1, index));
    bool child2Exists = (child2 <= listSize && isPriority(child2, index));
    if (child1Exists && child2Exists) {
        if (isPriority(child1, child2)) {
            swapEntries(child1, index);
            bubbleDown(child1);
        }
        else {
            swapEntries(child2, index);
            bubbleDown(child2);
        }
    }
    else if (child1Exists && !child2Exists) {
        swapEntries(child1, index);
        bubbleDown(child1);
    }
    else if (!child1Exists && child2Exists) {
        swapEntries(child2, index);
        bubbleDown(child2);
    }
}

/*
 * Runs in O(log N) time
 * parameter: new string value, new priority
 * adds the new entry to the end of the queue
 * precondition: parameters are all valid
 * duplicates are also added
 */
void HeapPriorityQueue::enqueue(string value, int priority) {
    if (listSize == listCapacity) {
        PQEntry* oldArray = elements;
        elements = new PQEntry[2 * (listCapacity + 1)]();
        for (int i = 1; i <= listSize; i++) {
            elements[i] = oldArray[i];
        }
        delete[]oldArray;
        listCapacity *= 2;
    }
    listSize++;
    elements[listSize] = PQEntry(value, priority);
    bubbleUp(listSize);
}

/*
 * helper function
 * Runs in O(log N) time
 * Starting at the given index,
 * it bubbles up the entry at the index until its parent is more urgent than itself
 */
void HeapPriorityQueue::bubbleUp(int index) {
    int parent = index / 2;
    if (parent > 0) {
        if (isPriority(index, parent)) {
            swapEntries(parent, index);
            bubbleUp(parent);
        }
    }
}

// Runs in O(1) time
// returns: true if queue is empty, false if it is not empty
bool HeapPriorityQueue::isEmpty() const {
    return (listSize == 0);
}

/*
 * Runs in O(1) time
 * returns: string value of the element with the highest urgency
 * throws a string exception if queue is empty
 */
string HeapPriorityQueue::peek() const {
    if (listSize == 0) {
        throw "Queue is empty";
    }
    return elements[1].value;
}

/*
 * Runs in O(1) time
 * returns: int priority of the element with the highest urgency
 * throws a string exception if queue is empty
 */
int HeapPriorityQueue::peekPriority() const {
    if (listSize == 0) {
        throw "Queue is empty";
    }
    return elements[1].priority;
}

// Runs in O(1) time
// returns: size of queue
int HeapPriorityQueue::size() const {
    return listSize;
}

/*
 * helper function
 * Runs in O(1) time
 * parameter: desired index
 * returns: entry at the desired index
 * throws an exception if the index is out of bounds
 */
PQEntry HeapPriorityQueue::getElementAt(int index) const {
    if (index > listSize || index < 0) {
        throw "Index is out of bounds";
    }
    return elements[index];
}

/*
 * Runs in O(N) time
 * printing operator overload
 */
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if (queue.size() != 0) {
        for (int i = 1; i < queue.size(); i++) {
            out << queue.getElementAt(i) << ", ";
        }
        out << queue.getElementAt(queue.size());
    }
    out << "}";
    return out;
}
