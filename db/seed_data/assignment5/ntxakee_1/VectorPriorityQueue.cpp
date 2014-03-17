/* Name: Lindsey Txakeeyang
 * Course: CS106B, Wednesday 4:15 Section
 * Assignment: Priority Queue
 * This program implements a priority queue using a vector.
 */

#include "VectorPriorityQueue.h"
#include "vector.h"

VectorPriorityQueue::VectorPriorityQueue() {
    priorityQueue = {};
}

VectorPriorityQueue::~VectorPriorityQueue() {
    delete[] &priorityQueue;
}

void VectorPriorityQueue::changePriority(string value, int newPriority) {
    int index = -1;
    for (int i = 0; i < priorityQueue.size(); i++) {
        PQEntry current;
        current = priorityQueue.get(i);
        if (current.value == value) {
            if (current.priority < newPriority) {
                string msg = "Current priority is already more urgent.";
                throw msg;
            } else {
                current.priority = newPriority;
                index = i;
                break;
            }
        }
    }
    if (index < 0) {
        string msg = "Value can't be found.";
        throw msg;
    }
}

void VectorPriorityQueue::clear() {
    for (int i = 0; i < priorityQueue.size(); i++) {
        priorityQueue.remove(i);
    }
}

string VectorPriorityQueue::dequeue() {
    if (priorityQueue.isEmpty()) {
        string msg = "Queue does not contain any elements.";
        throw msg;
    }
    int mostUrgentP = priorityQueue[0].priority;
    PQEntry answer;
    int index = 0;
    for (int i = 0; i < priorityQueue.size(); i++) {
        PQEntry current;
        current = priorityQueue.get(i);
        int currentP = current.priority;
        if (currentP < mostUrgentP) {
            mostUrgentP = currentP;
            answer = current;
            index = i;
        }
    }
    priorityQueue.remove(index);
    return answer.value;
}

void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry entry;
    entry.value = value;
    entry.priority = priority;
    priorityQueue.add(entry);
}

bool VectorPriorityQueue::isEmpty() const {
    if (priorityQueue.isEmpty()) return true;
    else return false;
}

string VectorPriorityQueue::peek() const {
    if (priorityQueue.isEmpty()) {
        string msg = "Queue does not contain any elements.";
        throw msg;
    }
    int mostUrgentP = priorityQueue[0].priority;
    PQEntry answer;
    for (int i = 0; i < priorityQueue.size(); i++) {
        PQEntry current;
        current = priorityQueue.get(i);
        int currentP = current.priority;
        if (currentP < mostUrgentP) {
            mostUrgentP = currentP;
            answer = current;
        }
    }
    return answer.value;
}

int VectorPriorityQueue::peekPriority() const {
    if (priorityQueue.isEmpty()) {
        string msg = "Queue does not contain any elements.";
        throw msg;
    }
    int mostUrgentP = priorityQueue[0].priority;
    for (int i = 0; i < priorityQueue.size(); i++) {
        PQEntry current;
        current = priorityQueue.get(i);
        int currentP = current.priority;
        if (currentP < mostUrgentP) {
            mostUrgentP = currentP;
        }
    }
    return mostUrgentP;
}

int VectorPriorityQueue::size() const {
    int size = priorityQueue.size();
    return size;
}

ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {
        out << queue.priorityQueue[0];
        for (int i = 1; i < queue.size(); i ++) {
            out << ", " << queue.priorityQueue[i];
        }
    }
    out << "}";
    return out;
}
