// Vector queue is more straightforward when adding entries to the priority queue.
// But the big O for dequeueing and peekings are more larger, since it has to search through the vector to find the most urgent one.

#include "VectorPriorityQueue.h"
#include "PQEntry.h"
#include "strlib.h"
#include "console.h"

// O(1)
// Constructor
VectorPriorityQueue::VectorPriorityQueue() {

}

// O(1)
// Destructor
VectorPriorityQueue::~VectorPriorityQueue() {

}

// O(N)
// This function finds the most urgent PQEntry in the vector.
// And return the index of that entry.
int VectorPriorityQueue::findMostUrgentIndex() const {
    if (v.size() != 0) {
        int mostUrgentIndex = 0;
        for (int i = 1; i < v.size(); i++) {
            if (v[i] < v[mostUrgentIndex]) {
                mostUrgentIndex = i;
            }
        }
        return mostUrgentIndex;
    }
}

/*
// O(N)
int VectorPriorityQueue::findMostUrgentIndexGivenValue(string value) {
    int index = -1;
    int highestPriority;
    for (int i = 0; i < v.size(); i++) {
        if (v[i].value == value) {
            if (index = -1) {
                index = i;
                highestPriority = v[i].priority;
            } else {
                if (v[i].priority < highestPriority) {
                    index = i;
                    highestPriority = v[i].priority;
                }
            }
        }
    }
    return index;
}
*/

// O(N)
// This function find the first occurance of the entry with the given value.
// If the value is not found in the vector, it returns the index.
// If not, it returns -1.
int VectorPriorityQueue::findMostUrgentIndexGivenValue(string value) {
    int index = -1;
    for (int i = 0; i < v.size(); i++) {
        if (v[i].value == value) {
            index = i;
            return index;
        }
    }
    return index;
}

// O(N)
// If the value is found in the vector, this function changes the priority to the new value.
// If the new priority value is larger than the existing one, it throws an exception.
// If the value is not found, it throws an exception.
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    if (findMostUrgentIndexGivenValue(value) == -1) {
        throw "Can not found such value!";
    } else {
        if (v[findMostUrgentIndexGivenValue(value)].priority > newPriority ) {
            PQEntry newEntry(value, newPriority);
            v[findMostUrgentIndexGivenValue(value)] = newEntry;
        } else {
            throw string("Use higher priority!");
        }
    }
}

// O(1)
// This function delete all entries in the priority queue.
void VectorPriorityQueue::clear() {
    v.clear();
}

// O(N)
// This function delete the most urgent entry from the prior queue, and return the value of that entry.
// If the fucntion is not found in the queue, it throws an exception.
string VectorPriorityQueue::dequeue() {
    if (v.size() == 0) {
        throw string("Empty queue!");
    } else {
        int mostUrgentIndex = findMostUrgentIndex();
        PQEntry entry = v[mostUrgentIndex];
        v.remove(mostUrgentIndex);
        return entry.value;
    }

}

// O(1)
// This function adds an entry to the end of the vector.
void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry entry = PQEntry(value, priority);
    v.add(entry);
    //delete
}

// O(1)
// This function checks if the priority queue is empty.
bool VectorPriorityQueue::isEmpty() const {
    return (v.size() == 0);
}

// O(1)
// This function returns the value of the most urgent entry in the priority queue.
// It throws an exception if the queue is empty.
string VectorPriorityQueue::peek() const {
    if (v.size() == 0) {
        throw string("Empty queue!");
    } else {
        return v[findMostUrgentIndex()].value;
    }
}

// O(1)
// This function returns the priority of the most urgent entry in the priority queue.
// It throws an exception if the queue is empty.
int VectorPriorityQueue::peekPriority() const {
    if (v.size() == 0) {
        throw string("Empty queue!");
    } else {
        return v[findMostUrgentIndex()].priority;
    }
}

// O(1)
// This function returns the size of the queue.
int VectorPriorityQueue::size() const {
    return v.size();
}

// O(N)
// This function converts all the entries in the vector to a single string, and returns the string.
string VectorPriorityQueue::toString() const {
    string result = "{";
    for (int i = 0; i < v.size(); i++) {
        result = result + "\"" + v[i].value + "\":" + integerToString(v[i].priority);
        if (i < v.size() - 1) {
            result += ", ";
        }
    }
    result += "}";
    return result;
}

// O(N)
// This function converts a VectorPriorityQueue to a string and adds that to a ostream, and returns the ostream.
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << queue.toString();
    return out;
}



