/*
 * This file implements the HeapPriorityQueue.h interface.
 */

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    h = new PQEntry[10];
    m_size = 0;
    capacity = 10;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] h;
}

void HeapPriorityQueue::checkResize() {
    if (m_size == capacity) {
        PQEntry* oldH = h;
        h = new PQEntry[2 * capacity]();
        for (int i = 0; i < m_size; i++) {
            h[i] = oldH[i];
        }
        delete[] oldH;
        capacity *= 2;
    }
}

int HeapPriorityQueue::search(string value, int newPriority) {
    for (int i = 1; i <= m_size; i++) {
        if (h[i].value == value) {
            if (h[i].priority < newPriority) throw "can't downgrade priority!";
            h[i].priority = newPriority;
            return i;
        }
    }
    return 0;
}

void HeapPriorityQueue::swap(int i, int j) {
    PQEntry temp = h[i];
    h[i] = h[j];
    h[j] = temp;
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    //find entry
    int i = search(value, newPriority);
    if (i == 0) return;
    //bubble up
    while (h[i / 2] > h[i]) {
        swap(i, i / 2);
        i /= 2;
    }
}

void HeapPriorityQueue::clear() {
    m_size = 0;
}

void HeapPriorityQueue::dequeueHelper(int n) {
    int loIndex;
    if (2 * n <= m_size) {
        if (2 * n + 1 > m_size) loIndex = 2 * n;
        else if (h[2 * n] < h[2 * n + 1]) loIndex = 2 * n;
        else loIndex = 2 * n + 1;
        if (h[n] > h[loIndex]) { //bubble down
            swap(n, loIndex);
            dequeueHelper(loIndex);
        }
    }
}

string HeapPriorityQueue::dequeue() {
    if (m_size == 0) throw "Empty queue, nothing to dequeue";
    string s = h[1].value;
    h[1] = h[m_size];
    m_size--; //reduces size of array by 1
    dequeueHelper(1);
    return s;
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    m_size++;
    checkResize();
    h[m_size] = PQEntry(value, priority);
    cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
    if (m_size == 1) return;
    int index = m_size;
    while (h[index / 2] > h[index]) { //bubble up
        swap(index, index / 2);
        index /= 2;
    }
}

bool HeapPriorityQueue::isEmpty() const {
    return (m_size == 0);
}

string HeapPriorityQueue::peek() const {
    if (m_size == 0) throw "Empty queue, nothing to see";
    return h[1].value;
}

int HeapPriorityQueue::peekPriority() const {
    if (m_size == 0) throw "Empty queue, nothing to see";
    return h[1].priority;
}

int HeapPriorityQueue::size() const {
    return m_size;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    for (int i = 1; i <= queue.size(); i++) {
        out << queue.h[i];
        if (i != queue.size()) out << ", ";
    }
    out << "}";
    return out;
}
