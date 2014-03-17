// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "VectorPriorityQueue.h"

/*
 * Creates a new VectorPriorityQueue.
 * O(1)
 */
VectorPriorityQueue::VectorPriorityQueue() {
    entries;
}

/*
 * Deletes the queue.
 * O(1)
 */
VectorPriorityQueue::~VectorPriorityQueue() {
    entries.clear();
}

/*
 * Changes the priority of a value in the queue.
 * O(N)
 */
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    int i = 0;
    while (value != entries.get(i).value && i < entries.size()) i++;
    if (value == entries.get(i) && newPriority < entries.get(i).priority) {
        entries.set(i,PQEntry(value,newPriority));
    }
    else {
        throw "exception";
    }
}

/*
 * Deletes the queue.
 * O(1)
 */
void VectorPriorityQueue::clear() {
    entries.clear();
}

/*
 * Removes the value with the highest
 * priority in the queue.
 * O(N)
 */
string VectorPriorityQueue::dequeue() {
    int i = 0;
    int lowest = 0;
    if (entries.size() == 0) return "";
    PQEntry entry = entries.get(i);
    string v = entry.value;
    int p = entry.priority;
    if (entries.size() == 1) {
        lowest = i;
        entries.remove(i);
        return v;
    }
    for (i = 1; i < entries.size(); i++) {
        if (entries.get(i).priority < p) {
            p = entries.get(i).priority;
            v = entries.get(i).value;
            lowest = i;
        }
    }
    entries.remove(lowest);
    return v;
}

/*
 * Adds an entry to the end of the
 * queue.
 * O(1)
 */
void VectorPriorityQueue::enqueue(string value, int priority) {
    entries.add(PQEntry(value, priority));
}

/*
 * Tells you if the queue is empty.
 * O(1)
 */
bool VectorPriorityQueue::isEmpty() const {
    if (entries.size() == 0) return true;
    return false;
}

/*
 * Returns the value of the highest priority
 * entry in the queue.
 * O(N)
 */
string VectorPriorityQueue::peek() const {
    if (entries.size() == 0) return "";
    if (entries.size() == 1) {
        PQEntry entry = entries.get(0);
        return entry.value;
    }
    PQEntry entry = entries.get(0);
    string v = entry.value;
    int p = entry.priority;
    for (int i = 1; i < entries.size(); i++) {
        if (entries.get(i).priority < p) {
            p = entries.get(i).priority;
            v = entries.get(i).value;
        }
    }
    return v;
}

/*
 * Outputs the priority of the highest
 * priority entry in the queue.
 * O(N)
 */
int VectorPriorityQueue::peekPriority() const {
    if (entries.size() == 0) return 0;
    if (entries.size() == 1) {
        PQEntry entry = entries.get(0);
        return entry.priority;
    }
    PQEntry entry = entries.get(0);
    int p = entry.priority;
    for (int i = 1; i < entries.size(); i++) {
        if (entries.get(i).priority < p) p = entries.get(i).priority;
    }
    return p;
}

/*
 * Outputs the size of the
 * queue.
 * O(1)
 */
int VectorPriorityQueue::size() const {
    return entries.size();
}

/*
 * Outputs the priority and
 * value of each entry in the queue.
 * O(N)
 */
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    if (queue.size() != 0) {
        int size = queue.size();
        for (int i = 0; i < size; i++) {
            out << "\"" << queue.entries.get(i).value << "\":" << queue.entries.get(i).priority;
            if (i < size - 1) out << ", ";
        }
    }
    out << "}";
    return out;
    return out;
}
