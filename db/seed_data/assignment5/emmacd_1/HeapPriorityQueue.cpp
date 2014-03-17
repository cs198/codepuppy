/*
 * CS106B Assignment #5: Priority Queue - Heap Priority Queue
 * Emma Daugherty
 * Section Leader: Jimmy Lee
 *
 * This class implements a priority queue with a heap as its data storage structure. This is the fastest / most
 * efficient priority queue out of the three because it has fast enqueuing and dequeuing speeds by using
 * bubbling up and bubbling down.
 *
 * See VectorPriorityQueue.h for documentation of member functions
 */

#include "HeapPriorityQueue.h"

/*
 * Constructor for HeapPriorityQueue object
 */
HeapPriorityQueue::HeapPriorityQueue() {
    queue = new PQEntry[10]();
    capacity = 10;
    s = 0;
}

/*
 * Destructor for HeapPriorityQueue object
 */
HeapPriorityQueue::~HeapPriorityQueue() {
    clear();
}


void HeapPriorityQueue::changePriority(string value, int newPriority) {
    int count = 1;
    while (count <= size()) {
        if (queue[count].value == value) {
            if (queue[count].priority > newPriority) {
                queue[count].priority = newPriority;
            } else {
                throw "";
            }
            int child = count;
            int parent = child / 2;
            while (queue[parent] > queue[child]) {
                PQEntry temp = queue[child];
                queue[child] = queue[parent];
                queue[parent] = temp;
                child = parent;
                parent = child / 2;
            }
            return;
        }
        count++;
    }
    throw "";

}


void HeapPriorityQueue::clear() {
    delete queue;
}


string HeapPriorityQueue::dequeue() {
    if (isEmpty()) {
        throw "";
    }
    string name = queue[1].value;
    int parent = 1;
    int child1 = parent * 2;
    int child2 = (parent * 2) + 1;
    while (child1 <= size()) {
        if (child1 == size()) {
            queue[parent] = queue[child1];
            s--;
            break;
        }
        int index;
        if (queue[child1].priority <= queue[child2].priority) {
            queue[parent] = queue[child1];
            parent = child1;
            child1 = parent * 2;
            child2 = (parent * 2) + 1;
        } else {
            queue[parent] = queue[child2];
            queue[child2] = PQEntry();
            parent = child2;
            child1 = parent * 2;
            child2 = (parent * 2) + 1;
        }
    }

    return name;
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    if (size() == capacity) {
        PQEntry* trash = queue;
        queue = new PQEntry[capacity * 2];
        capacity *= 2;
        delete trash;
    }
    s++;
    queue[size()] = PQEntry(value, priority);
    int child = size();
    int parent = child / 2;
    while (queue[parent] > queue[child]) {
        PQEntry temp = queue[child];
        queue[child] = queue[parent];
        queue[parent] = temp;
        child = parent;
        parent = child / 2;
    }


}


bool HeapPriorityQueue::isEmpty() const {
    return (size() == 0);
}


string HeapPriorityQueue::peek() const {
    if (isEmpty()) {
        throw "";
    }
    return queue[1].value;
}


int HeapPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw "";
    }
    return queue[1].priority;
}


int HeapPriorityQueue::size() const {
    return s;
}


ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{" << queue.queue[1];
    int count = 2;
    while (count <= queue.size()) {
        if (queue.queue[count].value != "") {
            out << ", " << queue.queue[count];
        }
        count++;
    }
    out << "}";
    return out;
}
