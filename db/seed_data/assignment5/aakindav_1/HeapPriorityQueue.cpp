// HeapPriorityQueue.cpp implements the HeapPriorityQueue
// class behavior declared in HeapPriorityQueue.h
// See HeapPriorityQueue.h for documentation of each member

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    m_array = new PQEntry[10]();
    array_size = 0;
    capacity = 10;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] m_array;
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    if (!isEmpty()) {
        int index;
        bool flag = false;
        for (int i=1; i<=array_size; i++) {
            if (m_array[i].value == value) {
                index = i;
                flag = true;
                break;
            }
        }
        if (!flag) throw "Value is not contained in Priority Queue!";
        if (m_array[index].priority > newPriority) {
            m_array[index].priority = newPriority;
        } else throw "Must upgrade priority!";
        int parent = index/2;
        while (m_array[index] <= m_array[parent]) {
            if (m_array[index] == m_array[parent]) {
                if (m_array[index].value > m_array[parent].value) break;
            }
            PQEntry temp = m_array[parent];
            m_array[parent] = m_array[index];
            m_array[index] = temp;
            parent /= 2;
        }
    } else throw "Empty Priority Queue!";
}

void HeapPriorityQueue::clear() {
    array_size = 0;
}

string HeapPriorityQueue::dequeue() {
    if (!isEmpty()) {
        string result = m_array[1].value;
        m_array[1] = m_array[array_size];
        array_size--;
        for (int i=1; i<=array_size; i++) {
            if (i*2 <= array_size) {
                // if the first child is less than the second child
                if (m_array[i*2] < m_array[i*2+1]) {
                    PQEntry child = m_array[i*2];
                    m_array[i*2] = m_array[i];
                    m_array[i] = child;
                    i *= 2;
                    //if both childs are equal
                } else if (m_array[i*2] == m_array[i*2+1]) {
                    // if first child has a lesser value than the second child
                    if (m_array[i*2].value < m_array[i*2+1].value) {
                        // if the parent has a leser value than the first chid's
                        if (m_array[i].value < m_array[i*2].value) break;
                        else {
                            // if the first child's value is lesser than the parents
                            PQEntry child = m_array[i*2];
                            m_array[i*2] = m_array[i];
                            m_array[i] = child;
                            i *= 2;
                        }
                    }
                    // if the second child has a lesser value than the first child
                    else {
                        // if the parents value is less than the second child
                        if (m_array[i].value < m_array[i*2+1].value) break;
                        else {
                            // if the second child has a lesser value than the parent
                            PQEntry child = m_array[i*2+1];
                            m_array[i*2+1] = m_array[i];
                            m_array[i] = child;
                            i = i*2+1;
                        }
                    }
                } else {
                    // if the second child is less than the first child
                    if (i*2+1 <= array_size) {
                        PQEntry child = m_array[i*2+1];
                        m_array[i*2+1] = m_array[i];
                        m_array[i] = child;
                        i = i*2+1;
                    }
                }
            }
        }
        return result;
    } else throw "Empty Priority Queue!";
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    int child = array_size+1;
    checkResize();
    m_array[child] = PQEntry (value, priority);
    array_size++;
    while (m_array[child] <= m_array[child/2]) {
        if (m_array[child] == m_array[child/2]) {
            if (m_array[child].value > m_array[child/2].value) break;
        }
        PQEntry parent = m_array[child/2];
        m_array[child/2] = m_array[child];
        m_array[child] = parent;
        child /= 2;
    }
}

bool HeapPriorityQueue::isEmpty() const {
    return array_size == 0;
}

string HeapPriorityQueue::peek() const {
    if (!isEmpty()) return m_array[1].value;
    else throw "Empty Priority Queue!";
}

int HeapPriorityQueue::peekPriority() const {
    if (!isEmpty()) return m_array[1].priority;
    else throw "Empty Priority Queue!";
}

int HeapPriorityQueue::size() const {
    return array_size;
}

void HeapPriorityQueue::checkResize() {
    if (array_size == capacity-1) {
        PQEntry* oldArray = m_array;
        m_array = new PQEntry[2 * capacity]();
        for (int i=1; i<=array_size; i++) {
            m_array[i] = oldArray[i];
        }
        delete[] oldArray;
        capacity *= 2;
    }
}

PQEntry* HeapPriorityQueue::getArray() const {
    return m_array;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    for (int i=1; i<=queue.size(); i++) {
        out << queue.getArray()[i];
        if (i != queue.size()) out << ", ";
    }
    out << "}";
    return out;
}
