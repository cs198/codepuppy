//Min Cheol Kim
//Jimmy Lee SL
//Priority Queue

/* This is the heap implementation of the priority queue. This implementation has the consistent
 * efficiencies ( O(logN) )for enqueing and dequeing, as well as very fast O(1) for peeking. Better
 * performance overall than the vector/linked list implementations. */

//See VectorPriorityQueue.h for documentation member functions.

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    capacity = 10;
    elements = new PQEntry[capacity+1];
    m_size = 0;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] elements;
}

// O(NlogN)
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    for (int n = 1; n <= m_size; n++) {
        if (elements[n].value == value) {
            if (elements[n].priority >= newPriority) {
                elements[n].priority = newPriority;
                enqueueHelper(n);
            }
            else
                throw "Error: new priority cannot be greater than old priority";
            return;
        }
    }
    throw "Error: The given value is not found in this priority queue";
}

// O(1)
void HeapPriorityQueue::clear() {
    m_size = 0;
}

// O(logN)
string HeapPriorityQueue::dequeue() {
    string result;
    if (m_size == 0)
        throw "Error: priority queue is empty";
    else {
        result = elements[1].value;
        elements[1] = elements[m_size];
        m_size--;
        dequeueHelper(1);
    }
    return result;
}

void HeapPriorityQueue::dequeueHelper(int pos) {
    if (2*pos > m_size)
        return;
    else if (elements[pos].priority < elements[2*pos].priority
             && elements[pos].priority < elements[2*pos+1].priority)
        return;
    else {
        int newPos = returnMoreUrgent(2*pos, 2*pos+1);
        swap (pos, newPos);
        dequeueHelper(newPos);
    }
}
// O(logN)
void HeapPriorityQueue::enqueue(string value, int priority) {
    checkResize();
    PQEntry temp(value, priority);
    elements[m_size+1] = temp;
    enqueueHelper(m_size+1);
    m_size++;
    cout << "Enqueued \"" << value << "\"" << " with priority " << priority << endl;
}

void HeapPriorityQueue::enqueueHelper(int pos) {
    if (pos == 1)
        return;
    else if (returnMoreUrgent(pos, pos/2) == pos/2)
        return;
    else {
        swap(pos, pos/2);
        enqueueHelper(pos/2);
    }
}

// O(1)
bool HeapPriorityQueue::isEmpty() const {
    return m_size == 0;
}

// O(1)
string HeapPriorityQueue::peek() const {
    if (m_size == 0)
        throw "Error: priority queue is empty";
    return elements[1].value;
}

// O(1)
int HeapPriorityQueue::peekPriority() const {
    if (m_size == 0)
        throw "Error: priority queue is empty";
    return elements[1].priority;
}

// O(1)
int HeapPriorityQueue::size() const {
    return m_size;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    for (int n = 1; n < queue.m_size+1; n++) {
        if (n != queue.m_size)
            out << queue.elements[n] << ", ";
        else
            out << queue.elements[n];
    }
    out << "}";
    return out;
}

void HeapPriorityQueue::swap(int a, int b) {
    PQEntry temp = elements[a];
    elements[a] = elements[b];
    elements[b] = temp;
}

void HeapPriorityQueue::checkResize() {
    if (m_size == capacity - 1) {
        PQEntry* bigger = new PQEntry[2*capacity + 1]();
        for (int n = 1; n < m_size+1; n++) {
            bigger[n] = elements[n];
        }
        delete[] elements;
        elements = bigger;
        capacity *= 2;
    }
}

int HeapPriorityQueue::returnMoreUrgent(int a, int b) const {
    if (elements[a].priority > elements[b].priority)
        return b;
    if (elements[b].priority > elements[a].priority)
        return a;
    else {
        if (elements[a].value >= elements[b].value)
            return b;
        else
            return a;
    }
}


