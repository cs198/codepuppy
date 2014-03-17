// Maintains a priority queue using a binary heap.


#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
    Vector<PQEntry> priorityQueue;
}

VectorPriorityQueue::~VectorPriorityQueue() {
    // Don't need to do anything. Did not allocate any new memory.
}

/* Changes priority of a given value IF newPriority is more urgent than the value's old priority.
 * If the given value is present in the queue and already has a more
 * urgent priority to the given new priority, or if the given value is not already in the
 * queue, the function throws a string exception.*/
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    if (isEmpty()) {
        throw "Error: priority queue is empty"; //string exception
    }
    bool priorityChanged = false;
    bool valuePresent = false;
    for (int i = 0; i < size(); i++) {
        if (priorityQueue[i].value == value) {
            valuePresent = true;
        }
        if (priorityQueue[i].value == value && priorityQueue[i].priority > newPriority && priorityChanged ==false) {
            priorityQueue[i].priority = newPriority;
            priorityChanged = true;
        }
    }
    if (!priorityChanged && valuePresent) {
        throw "Error: new priority cannot be greater than old priority";
    }
    if (!valuePresent) {
        throw "Error: The given value is not found in this priority queue";
    }

}

//clears priority queue.
void VectorPriorityQueue::clear() {
    while (!priorityQueue.isEmpty()) {
        priorityQueue.remove(0);
    }
}

//Removes and returns most urgent element from the queue. Throws a string exception if the queue
//does not contain any elements.
string VectorPriorityQueue::dequeue() {
    if (priorityQueue.isEmpty()) {
        throw "Error: priority queue is empty";
    }
    else {
        PQEntry highestPriorityEntry = priorityQueue[0];
        int highestPriorityIndex = 0;
        for (int i = 0; i < size(); i++) {
            PQEntry entry = priorityQueue[i];
            if (entry < highestPriorityEntry) {
                highestPriorityEntry = entry;
                highestPriorityIndex = i;
                }
            }
        priorityQueue.remove(highestPriorityIndex);
        return highestPriorityEntry.value;
        }
    }


//adds an entry to the priority queue.
void VectorPriorityQueue::enqueue(string value, int priority) { //done
    PQEntry newEntry(value, priority);
    priorityQueue.add(newEntry);
    cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
}

//returns true if queue is empty, returns false if not.
bool VectorPriorityQueue::isEmpty() const { //done
    if (priorityQueue.size() == 0) return true;
    else return false;
    // TODO: implement
    return false;   // remove this
}

//returns the string element with the highest priority. Throws string exception is queue is empty.
string VectorPriorityQueue::peek() const { //done
    if (priorityQueue.isEmpty()) {
        throw "Error: priority queue is empty";
    }
    else {
        PQEntry highestPriority = priorityQueue[0];
        for (int i = 0; i < size(); i++) {
            PQEntry entry = priorityQueue[i];
            if (entry < highestPriority) {
                highestPriority = entry;
            }
        }
        return highestPriority.value;
    }
}

//returns the most urgent integer priority. Throws string exception is queue is empty.
int VectorPriorityQueue::peekPriority() const { //done
    if (priorityQueue.isEmpty()) {
        throw "Error: priority queue is empty";
    }
    else {
        PQEntry highestPriority = priorityQueue[0];
        for (int i = 0; i < priorityQueue.size(); i++) {
            PQEntry entry = priorityQueue[i];
            if (entry < highestPriority) {
                highestPriority = entry;
            }
        }
        return highestPriority.priority;
    }
}

//returns the number of elements in the array.
int VectorPriorityQueue::size() const { //done
    return priorityQueue.size();
}

//prints priority queue to console.
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    Vector<PQEntry> priorityQueue = queue.priorityQueue;
    out << "{";
    for (int i = 0; i < priorityQueue.size(); i++) {
        out << priorityQueue[i];
        if (i != priorityQueue.size()-1 )
            out << ", ";
    }
    out << "}";
    return out;
}
