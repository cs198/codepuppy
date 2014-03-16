//Name: Benzheng Zhu
//CS 106B Assignment 5
//Citation: lecture slides

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    capacity = 10;
    m_size = 0;
    root = new PQEntry[10]();
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] root;
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    bool flag = true;
    for (int i = 1; i <= m_size; i++) {
        if (root[i].value == value && root[i].priority > newPriority) {
            root[i].priority = newPriority;
            BubbleUp(i);
            flag = false;
            break;
        }
    }
    if(flag) {
        throw "exception";
    }
}

void HeapPriorityQueue::clear() {
    m_size = 0;
}

string HeapPriorityQueue::dequeue() {
    string result = root[1].value;

    //replace the first node with the last one
    root[1]= root[m_size];
    m_size--;

    //bubble down
    BubbleDown(1);
    return result;
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    //add to the end
    checkResize();
    root[m_size+1].value = value;
    root[m_size+1].priority = priority;
    m_size++;

    //bubble up
    BubbleUp(m_size);
}

bool HeapPriorityQueue::isEmpty() const {
    return m_size == 0;
}

string HeapPriorityQueue::peek() const {
    if(m_size != 0) {
        return root[1].value;
    } else {
        throw "exception";
    }
}

int HeapPriorityQueue::peekPriority() const {
    if(m_size != 0) {
        return root[1].priority;
    } else {
        throw "exception";
    }
}

int HeapPriorityQueue::size() const {
    return m_size;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {
        out << queue.root[1];
        for (int i = 1; i < queue.size(); i++) {
            out << ", " << queue.root[i+1];
        }
    }
    out << "}";
    return out;
}

void HeapPriorityQueue::checkResize() {
    if (m_size == capacity-1) {
        PQEntry* oldroot = root;
        root = new PQEntry[2 * capacity]();
        for (int i = 0; i < m_size; i++) {
            root[i+1] = oldroot[i+1];
        }
        delete[] oldroot;
        capacity *= 2;
    }
}

int HeapPriorityQueue::childIndex(int parent) const {
    int child = 0;
    if (m_size < 2*parent) {
        throw "exception";
    } else if (m_size>=2*parent+1) {
        if (root[2*parent].priority < root[2*parent+1].priority ||
                (root[2*parent].priority == root[2*parent+1].priority && root[2*parent].value<root[2*parent+1].value)) {
            child = 2*parent;
        } else  {
            child = 2*parent+1;
        }
    } else {
        child = 2*parent;
    }
    return child;
}

void HeapPriorityQueue::BubbleUp(int child) const {
    if (child <= 1) {
        return;
    } else {
        int parent = (child - child%2)/2; //parent's index

        if (root[child].priority < root[parent].priority ||
                (root[child].priority==root[parent].priority && root[child].value<root[parent].value)) {
            PQEntry temp = root[parent];
            root[parent] = root[child];
            root[child] = temp;

            BubbleUp(parent);
        }
        return;
    }
}

void HeapPriorityQueue::BubbleDown(int parent) const {
    if (m_size < 2*parent) {
        return;
    } else {
        int child = childIndex(parent); //find the more urgent child

        if (root[child].priority < root[parent].priority ||
                (root[child].priority==root[parent].priority && root[child].value<root[parent].value)) {
            PQEntry temp = root[parent];
            root[parent] = root[child];
            root[child] = temp;

            BubbleDown(child);
        }
    }
}
