/*
 * CS106B Assignment #5: Priority Queue - Vector Priority Queue
 * Emma Daugherty
 * Section Leader: Jimmy Lee
 *
 * This class implements a priority queue where the data is stored in a Vector. This priority queue has a fast
 * enqueuing time because the Vector is unsorted, but that also means it has a slow dequeue time and therefore overall
 * poor performance.
 */

#include "VectorPriorityQueue.h"

/*
 * This method constructs the VectorPriorityQueue object.
 *
 * O(1)
 */
VectorPriorityQueue::VectorPriorityQueue() {

}

/*
 * This method destructs the VectorPriorityQueue object.
 *
 * O(1)
 */
VectorPriorityQueue::~VectorPriorityQueue() {

}

void VectorPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 0; i < size(); i++) {
        if (queue[i].value == value) {
            if (newPriority >= queue[i].priority) {
                throw "";
            } else {
                queue[i].priority = newPriority;
                return;
            }
        }
    }

    throw "";

}


void VectorPriorityQueue::clear() {
    queue.clear();

}


string VectorPriorityQueue::dequeue() {
    if (!isEmpty()) {
        int index = 0;
        string name = queue[0].value;
        int urgent = queue[0].priority;
        for (int i = 1; i < size(); i++) {
            if (queue[i].priority < urgent) {
                if (queue[i].value < name) {
                    index = i;
                    name = queue[i].value;
                    urgent = queue[i].priority;
                }
            }
        }
        queue.remove(index);
        return name;
    }

}


void VectorPriorityQueue::enqueue(string value, int priority) {
    PQEntry p(value, priority);
    queue.add(p);

}


bool VectorPriorityQueue::isEmpty() const {
    return queue.isEmpty();
}


string VectorPriorityQueue::peek() const {
    if (!isEmpty()) {
        string name = queue[0].value;
        int urgent = queue[0].priority;
        for (int i = 1; i < size(); i++) {
            if (queue[i].priority < urgent) {
                if (queue[i].value < name) {
                    name = queue[i].value;
                    urgent = queue[i].priority;
                }
            }
        }
        return name;
    }
    throw "";

}


int VectorPriorityQueue::peekPriority() const {
    if (!isEmpty()) {
        int urgent = queue[0].priority;
        for (int i = 1; i < size(); i++) {
            if (queue[i].priority < urgent) {
                urgent = queue[i].priority;
            }
        }
        return urgent;
    }

    throw "";

}


int VectorPriorityQueue::size() const {
    return queue.size();
}


ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << queue.queue.toString() << endl;
    return out;
}
