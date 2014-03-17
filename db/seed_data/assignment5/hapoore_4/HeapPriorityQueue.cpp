#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    queue = new PQEntry[10];
    length = 0;
    capacity = 9;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] queue;
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    int index = -1;
    for(int i = 1; i <= length; i++) {
        if(queue[i].value == value) {
            queue[i].priority = newPriority;
            index = i;
            break;
        }
    }
    if(index == -1) {
        throw "That element does not exist with a priority worse than the desired priority.";
    }
    bubbleUp(index);
}

void HeapPriorityQueue::clear() {
    delete[] queue;
    queue = new PQEntry[10];
    length = 0;
    capacity = 9;
}

string HeapPriorityQueue::dequeue() {
    if(length == 0) {
        throw "Empty queue.";
    }
    PQEntry front = queue[1];
    if(length != 1) {
        queue[1] = queue[length];
        length -= 1;
        bubbleDown();
    } else {
        length -= 1;
    }
    return front.value;
}

void HeapPriorityQueue::bubbleDown(int index) {
    PQEntry parent = queue[index];
    if (2*index < length) {
        PQEntry child1 = queue[2*index];
        PQEntry child2 = queue[2*index + 1];
        if (child1.priority < child2.priority || (child1.priority == child2.priority && child1.value <= child2.value)) {
            if (child1.priority < queue[index].priority || (child1.priority == queue[index].priority && child1.value <= queue[index].value)) {
                queue[index] = child1;
                queue[2*index] = parent;
                bubbleDown(2*index);
            }
        } else {
            if (child2.priority < queue[index].priority || (child2.priority == queue[index].priority && child2.value <= queue[index].value)) {
                queue[index] = child2;
                queue[2*index + 1] = parent;
                bubbleDown(2*index + 1);
            }
        }
    } else if (2*index == length){
        PQEntry child1 = queue[2*index];
        if (child1.priority < queue[index].priority || (child1.priority == queue[index].priority && child1.value <= queue[index].value)) {
            queue[index] = child1;
            queue[2*index] = parent;
        }
    }
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    PQEntry newEntry(value, priority);
    if(length == capacity) {
        resize();
    }
    queue[length + 1] = newEntry;
    length += 1;
    bubbleUp(length);
}

void HeapPriorityQueue::resize() {
    PQEntry* largerQueue = new PQEntry[2*capacity];
    for (int i = 1; i <= length; i++) {
        largerQueue[i] = queue[i];
    }
    delete[] queue;
    queue = largerQueue;
    capacity *= 2;
}

void HeapPriorityQueue::bubbleUp(int currIndex) {
    if(currIndex != 1) {
        int parent = currIndex/2;
        if(queue[parent].priority > queue[currIndex].priority || (queue[parent].priority == queue[currIndex].priority && queue[parent].value > queue[currIndex].value)) {
            PQEntry temp = queue[parent];
            queue[parent] = queue[currIndex];
            queue[currIndex] = temp;
            bubbleUp(parent);
        }
    }
}

bool HeapPriorityQueue::isEmpty() const {
    return (length == 0);
}

string HeapPriorityQueue::peek() const {
    if (length == 0) {
        throw "Empty queue.";
    }
    return queue[1].value;
}

int HeapPriorityQueue::peekPriority() const {
    if (length == 0) {
        throw "Empty queue.";
    }
    return queue[1].priority;
}

int HeapPriorityQueue::size() const {
    return length;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    if (queue.isEmpty()) {
        out << "{}";
    } else {
        out << "{";
        for(int i = 1; i < queue.length; i++) {
            out << queue.queue[i];
            out << ", ";
        }
        out << queue.queue[queue.length];
        out << "}";
    }
    return out;
}
