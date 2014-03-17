/* Michelle S. Lee
   CS 106B

This .cpp file implements a priority queue using a vector. A priority queue is a sorted queue
where the elements are sorted by priority, breaking ties by comparing the string elements themselves.*/

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
    //No constructor needed
}

VectorPriorityQueue::~VectorPriorityQueue() {
    //No destructor needed
}

/* Takes an input of value and an integer, searches the queue for the value,
 * and updates the priority with the given integer. */
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    bool found = false;
    if (!isEmpty()) {
        //Iterates over vector to find the value
        for (int i = 0; i < v.size(); i++) {
               if (!found && v[i].value == value) {
                   //Update the priority
                    PQEntry newPQ = PQEntry(value, newPriority);
                    v.set(i, newPQ);
                    found = true;
                }
        }
    }
    //If the value is not in the queue, throws a string exception
    if (!found) {
        throw ("Queue does not contain the given value.");
    }
}

void VectorPriorityQueue::clear() {
    // Clears the queue by removing all elements of the vector
    v.clear();
}

/* Returns the element with the minimum (most urgent) priority */
string VectorPriorityQueue::dequeue() {
    if (v.isEmpty()) {
        //String exception if queue is empty
        throw ("The queue is empty.");
    } else {
        //Find the minimum priority by looping through the vector
        int npos = 0;
        PQEntry minPriority = v[0];
        for (int i = 1; i < v.size(); i++) {
            if (v[i] < minPriority) {
                minPriority = v[i];
                npos = i;
            }
        }
        //Remove the item of minimum priority from the vector
        v.remove(npos);

        //Return the value with the minimum priority
        return minPriority.value;
    }
}

/* Adds the given input of string and priority value to the queue */
void VectorPriorityQueue::enqueue(string value, int priority) {
    v.add(PQEntry(value, priority));
}

/* Returns true if the queue is empty and false otherwise */
bool VectorPriorityQueue::isEmpty() const {
    // TODO: implement
    return (v.isEmpty());
}

/* Returns the value assigned to most urgent priority without removing it from the queue */
string VectorPriorityQueue::peek() const {
    if (isEmpty()) {
        //String exception if empty queue
        throw ("The queue is empty.");
    }

    //Calls the function to get the entry with the most urgent priority
    PQEntry minPriority = getMinPriority();

    //Returns the associated value
    return minPriority.value;
}

/* Returns the integer value of the most urgent priority without removing from queue */
int VectorPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw ("The queue is empty.");
    }

    //Calls the function to get the entry
    PQEntry minPriority = getMinPriority();

    //Returns the priority
    return minPriority.priority;
}

/* Returns a type PQEntry assigned to the most urgent priority */
PQEntry VectorPriorityQueue::getMinPriority() const {
    if (isEmpty()) {
        //String exception if empty queue
        throw ("The queue is empty.");
    }

    if (!v.isEmpty()) {
        PQEntry minPriority = v[0];
        //Loop through the vector to find the item with lowest priority value
        for (int i = 1; i < v.size(); i++) {
            if (v[i] < minPriority) {
                minPriority = v[i];
            }
        }

        //Return PQEntry with lowest priority value
        return minPriority;
    }
}

/* Returns an integer of the size of the queue by measuring the size of the vector */
int VectorPriorityQueue::size() const {
    return v.size();
}

/* << operator for printing priority queue to the console */
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    //Loop through vector and print the value and priority
    for (int i = 0; i < queue.v.size(); i++) {
        out << queue.v[i].value << ":" << queue.v[i].priority;
        if (i < queue.v.size()-1) out << ", ";
    }
    out << "}";
    return out;
}
