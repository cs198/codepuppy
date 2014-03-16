//Min Cheol Kim
//Jimmy Lee SL
//Priority Queue

/* This is the vector implementation of the priority queue. Enqueing elements is very fast, since
 * the elements are simply added to the end of the vector. Dequeing and peeking however requires
 * a search through the vector each time the functions are called. */

#include "VectorPriorityQueue.h"
#include "PQEntry.h"

/* Constructor */
VectorPriorityQueue::VectorPriorityQueue() {

}

/*Destructor, frees up any memory used by the priority queue. */
VectorPriorityQueue::~VectorPriorityQueue() {

}

/* Function takes in a value and a priority and changes the priority of the first
 * occurance of the value in the given data structure. If the value is not found or
 * the previous priority is already more urgent, string exceptions are thrown.
 * O(N) */
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    for (int n = 0; n < elements.size(); n++) {
        if (elements[n].value == value) {
            if (elements[n].priority >= newPriority)
                elements[n].priority = newPriority;
            else
                throw "Error: new priority cannot be greater than old priority";
            return;
        }
    }
    throw "Error: The given value is not found in this priority queue";

}

/* Function removes all elements from the priority queue
   O(N) */

void VectorPriorityQueue::clear() {
   elements.clear();
}

/* Function iterates through and finds the minimum priority, removes
 * the element, and returns the value of the removed element.
 * O(N)*/
string VectorPriorityQueue::dequeue() {
    int min = findMinIndex();
    string result = elements[min].value;
    elements.remove(min);
    return result;
}

/* Function takes in a string value and priority and adds a priority queue entry to the
 * appropriate data structure.
 * O(N) */
void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry entry(value, priority);
    elements.add(entry);
}

/* Function returns whether the priority queue is empty or not.
 * O(1) */
bool VectorPriorityQueue::isEmpty() const {
    return elements.size() == 0;
}

/* Function returns the value of the lowest priority element in the current state of
 * the priority queue. */
string VectorPriorityQueue::peek() const {
    int min = findMinIndex();
    return elements[min].value;

}

/* Function returns the priority of the lowest priority element in the current state of
 * the priority queue.
 * O(N) */
int VectorPriorityQueue::peekPriority() const {
    int min = findMinIndex();
    return elements[min].priority;
}

/* Function returns the size of the priority queue.
 * O(1) */
int VectorPriorityQueue::size() const {
    return elements.size();
}

/* Overloads the << operator to be able to be printed to the console. */
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << queue.elements;
    return out;
}

/* Helper function to find the lowest priority element in the vector. If the
 * priorities of two elements are equal, their values are compared. */
int VectorPriorityQueue::findMinIndex() const {
    if (elements.size() == 0)
        throw "Error: priority queue is empty";
    if (elements.size() !=0) {
        int min = 0;
        for (int n = 0; n < elements.size(); n++) {
            if (elements[n].priority < elements[min].priority)
                min = n;
            else if(elements[n].priority == elements[min].priority && elements[n].value
                    < elements[min].value)
                    min = n;
        }
        return min;
    }
    else
        return 0;
}
