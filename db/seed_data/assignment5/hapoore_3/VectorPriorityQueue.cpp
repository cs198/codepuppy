#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {

}

VectorPriorityQueue::~VectorPriorityQueue() {

}

void VectorPriorityQueue::changePriority(string value, int newPriority) {
    int index = -1;
    for(int i = 0; i < unsorted.size(); i++) {
        PQEntry current = unsorted[i];
        if((current.value == value) && (current.priority > newPriority)) {
            current.priority = newPriority;
            unsorted[i] = current;
            index = i;
            break;
        }
    }
    if (index == -1) {
        throw "That element does not exist with a priority worse than the desired priority.";
    }
}

void VectorPriorityQueue::clear() {
    unsorted.clear();
}

string VectorPriorityQueue::dequeue() {
    if(unsorted.isEmpty()) {
        throw "Empty queue.";
    }
    PQEntry mostUrgent;
    int urgentIndex = findMostUrgent(mostUrgent);
    unsorted.remove(urgentIndex);
    return mostUrgent.value;
}

int VectorPriorityQueue::findMostUrgent(PQEntry& mostUrgent, int urgentIndex) const {
    mostUrgent = unsorted[0];
    for (int i = 1; i < unsorted.size(); i++) {
        PQEntry next = unsorted[i];
        if(next < mostUrgent) {
            mostUrgent = next;
            urgentIndex = i;
        }
    }
    return urgentIndex;
}

void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry newEntry(value, priority);
    unsorted.add(newEntry);
}

bool VectorPriorityQueue::isEmpty() const {
    return unsorted.isEmpty();
}

string VectorPriorityQueue::peek() const {
    if(unsorted.isEmpty()) {
        throw "Empty queue.";
    }
    PQEntry mostUrgent;
    findMostUrgent(mostUrgent);
    return mostUrgent.value;
}

int VectorPriorityQueue::peekPriority() const {
    if(unsorted.isEmpty()) {
        throw "Empty queue.";
    }
    PQEntry mostUrgent;
    findMostUrgent(mostUrgent);
    return mostUrgent.priority;
}

int VectorPriorityQueue::size() const {
    return unsorted.size();
}

ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    if (queue.isEmpty()) {
        out << "{}";
    } else {
        out << "{";
        for(int i = 0; i < queue.unsorted.size() - 1; i++) {
            out << queue.unsorted[i];
            out << ", ";
        }
        out << queue.unsorted[queue.unsorted.size() - 1];
        out << "}";
    }
    return out;
}
