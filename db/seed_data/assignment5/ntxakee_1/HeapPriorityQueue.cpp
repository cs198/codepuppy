/* Name: Lindsey Txakeeyang
 * Course: CS106B, Wednesday 4:15 Section
 * Assignment: Priority Queue
 * This program implements a priority queue using a binary heap.
 */

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    priorityQueue = new PQEntry[10]();
    m_size = 0;
    capacity = 10;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] priorityQueue;
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    int index = 0;
    for (int i = 0; i <= m_size; i++) {
        if (index == 0) {
            if (priorityQueue[i].value == value) {
                if (priorityQueue[i].priority > newPriority) {
                    priorityQueue[i].priority = newPriority;
                    index = i;
                } else {
                    string msg = "Already has a more urgent priority.";
                    throw msg;
                }
            }
        }
    }
    if (index == 0) {
        string msg = "Value could not be found.";
        throw msg;
    } else {
        index = bubbleUp(index, priorityQueue[index], priorityQueue);
    }
}

void HeapPriorityQueue::clear() {
    m_size = 0;
}

string HeapPriorityQueue::dequeue() {
    if (m_size == 0) {
        string msg = "Priority Queue contains no elements.";
        throw msg;
    }

    string answer = priorityQueue[1].value;

    priorityQueue[1] = priorityQueue[m_size];
    int newIndex = bubbleDown(1, priorityQueue[1], priorityQueue);

    priorityQueue[m_size].value = "";
    priorityQueue[m_size].priority = 0;

    m_size--;
    return answer;
}

int HeapPriorityQueue::bubbleDown(int index, PQEntry entry, PQEntry* priorityQueue) {
    int childIndex1 = index * 2;
    int childIndex2 = index * 2 + 1;
    if (entry.priority > priorityQueue[childIndex1].priority && entry.priority > priorityQueue[childIndex2].priority) {
        return index;
    }
    else if (priorityQueue[childIndex1].priority <= priorityQueue[childIndex2].priority) {
        if (priorityQueue[childIndex1].priority < entry.priority) {
            bubbleDownHelper(index, childIndex1, entry, priorityQueue);
            return bubbleDown(childIndex1, entry, priorityQueue);
        } else if (priorityQueue[childIndex1].priority == entry.priority) {
            if (priorityQueue[childIndex1].value < entry.value) {
                bubbleDownHelper(index, childIndex1, entry, priorityQueue);
                return bubbleDown(childIndex1, entry, priorityQueue);
            } else if (priorityQueue[childIndex1].value > entry.value) {
                return index;
            }
        }
    } else {
        if (priorityQueue[childIndex2].priority < entry.priority) {
            bubbleDownHelper(index, childIndex2, entry, priorityQueue);
            return bubbleDown(childIndex2, entry, priorityQueue);
        } else if (priorityQueue[childIndex2].priority == entry.priority) {
            if (priorityQueue[childIndex2].value < entry.value) {
                bubbleDownHelper(index, childIndex2, entry, priorityQueue);
                return bubbleDown(childIndex2, entry, priorityQueue);
            } else if (priorityQueue[childIndex2].value > entry.value) {
                return index;
            }
        }
    }
    return index;
}

void HeapPriorityQueue::bubbleDownHelper(int index, int childIndex, PQEntry entry, PQEntry* priorityQueue) {
    PQEntry temp = priorityQueue[index];
    priorityQueue[index] = priorityQueue[childIndex];
    priorityQueue[childIndex] = temp;
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    resize();

    PQEntry entry;
    entry.value = value;
    entry.priority = priority;
    priorityQueue[m_size + 1] = entry;

    int newIndex = bubbleUp(m_size + 1, entry, priorityQueue);
    m_size++;
}

int HeapPriorityQueue::bubbleUp(int index, PQEntry entry, PQEntry* priorityQueue) {
    int parentIndex = index / 2;
    if (entry.priority > priorityQueue[parentIndex].priority) {
        return index;
    } else if (entry.priority < priorityQueue[parentIndex].priority) {
        bubbleUpHelper(index, parentIndex, entry, priorityQueue);
        return bubbleUp(parentIndex, entry, priorityQueue);
    } else if (entry.priority == priorityQueue[parentIndex].priority) {
        if (entry.value > priorityQueue[parentIndex].value) {
            return index;
        } else if (entry.value < priorityQueue[parentIndex].value) {
            bubbleUpHelper(index, parentIndex, entry, priorityQueue);
            return bubbleUp(index / 2, entry, priorityQueue);
        }
    }
    return index;
}

void HeapPriorityQueue::bubbleUpHelper(int index, int parentIndex, PQEntry entry, PQEntry* priorityQueue) {
    PQEntry temp = priorityQueue[parentIndex];
    priorityQueue[parentIndex] = entry;
    priorityQueue[index] = temp;
}

bool HeapPriorityQueue::isEmpty() const {
    if (m_size == 0) return true;
    else return false;
}

string HeapPriorityQueue::peek() const {
    if (m_size == 0) {
        string msg = "Priority Queue contains no elements.";
        throw msg;
    }
    string answer;
    answer = priorityQueue[1].value;
    return answer;
}

int HeapPriorityQueue::peekPriority() const {
    if (m_size == 0) {
        string msg = "Priority Queue contains no elements.";
        throw msg;
    }
    int priority = priorityQueue[1].priority;
    return priority;
}

int HeapPriorityQueue::size() const {
    return m_size;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    int size = queue.size();
    if (size != 0) {
        out << queue.priorityQueue[1];
        if (size >= 2) {
            for (int i = 2; i <= size; i++) {
                out << ", " << queue.priorityQueue[i];
            }
        }
    }
    out << "}";
    return out;
}

void HeapPriorityQueue::resize() {
    if (m_size == capacity) {
        PQEntry* old = priorityQueue;
        priorityQueue = new PQEntry[2 * capacity]();
        for (int i = 0; i < m_size; i++) {
            priorityQueue[i] = old[i];
        }
        delete[] old;
        capacity *= 2;
    }
}
