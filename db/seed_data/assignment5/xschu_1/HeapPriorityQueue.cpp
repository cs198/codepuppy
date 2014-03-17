// Heap is more efficient than the other queues, as the big O for enqueue, dequeue and change value is only log(N).
// However, it is less straightforward than the other two method, and the algorithm is more complicated.

#include "HeapPriorityQueue.h"
#include "PQEntry.h";
#include "strlib.h"

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
HeapPriorityQueue::HeapPriorityQueue() {
    elements = new PQEntry[10]();
    m_size = 0;
    capacity = 10;
}

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] elements;
}


/*
// O(N)
int HeapPriorityQueue::findMostUrgentIndexGivenValue(string value) {
    int index = 0;
    int highestPriority;
    for (int i = 1; i <= m_size; i++) {
        if (elements[i].value == value) {
            if (index = -1) {
                index = i;
                highestPriority = elements[i].priority;
            } else {
                if (elements[i].priority < highestPriority) {
                    index = i;
                    highestPriority = elements[i].priority;
                }
            }
        }
    }
    return index;
}

// O(N)
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    int index = findMostUrgentIndexGivenValue(value);
    if (index == 0) {
        throw "Can not found such value!";
    } else {
        if (elements[index].priority > newPriority ) {
            elements[index].priority = newPriority;
            bubbleUp(index);
        } else {
            throw string("Use higher priority!");
        }
    }
}
*/

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
int HeapPriorityQueue::findValue(string value) {
    int index = 0;
    for (int i = 1; i <= m_size; i++) {
        if (elements[i].value == value) {
            index = i;
            return index;
        }
    }
    return index;
}

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    int index = findValue(value);
    if (index == 0) {
        throw "Can not found such value!";
    } else {
        if (elements[index].priority > newPriority ) {
            elements[index].priority = newPriority;
            bubbleUp(index);
        } else {
            throw string("Use higher priority!");
        }
    }
}

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
void HeapPriorityQueue::clear() {
    m_size = 0;
}

// O(log(N))
// See VectorPriorityQueue.h for documentation of each member function.
string HeapPriorityQueue::dequeue() {
    if (m_size == 0) {
        throw string ("Empty queue!");
    } else {
        PQEntry result =  elements[1];
        elements[1] = elements[m_size];
        m_size--;
        //int level =  1;
        bubbleDown();
        //bubbleDown(level);
        return result.value;
    }
}

/*
void HeapPriorityQueue::bubbleDown(int level) {
    if (level > m_size) {
        return;
    } else {
        if ()
    }
}
*/

// O(1)
// This function swaps the position of two entries.
void HeapPriorityQueue::swapElements(int a, int b) {
    PQEntry temp = elements[a];
    elements[a] = elements[b];
    elements[b] = temp;
}

// O(log(N))
// This function compares the father entry to the son entry.
// If the father entry has lower priority, it swaps the position of two entries.
void HeapPriorityQueue::bubbleDown() {
    int level = 1;
    while (2*level <= m_size) { // has next level && has left node
        if (2*level + 1 <= m_size) { // also has right node
            if (elements[level] > elements[2*level] && elements[level] > elements[2*level + 1]) {
                if (elements[2*level] > elements[2*level + 1]) { //swap with right node
                    swapElements(level, 2*level + 1);
                    level = 2*level + 1;
                } else { //swap with left node
                    swapElements(level, 2*level);
                    level = 2*level;
                }
            } else if (elements[level] > elements[2*level] && elements[level] < elements[2*level + 1] ) {
                swapElements(level, 2*level);
                level = 2*level;
            } else if (elements[level] < elements[2*level] && elements[level] > elements[2*level + 1] ) {
                swapElements(level, 2*level + 1);
                level = 2*level + 1;
            } else {
                return;
            }
        } else { // only has left node
            if (elements[level] > elements[2*level]) {
                swapElements(level, 2*level);
                level = 2*level;
            } else {
                return;
            }
        }
    }
}

// O(log(N))
// See VectorPriorityQueue.h for documentation of each member function.
void HeapPriorityQueue::enqueue(string value, int priority) {
    checkResize();
    elements[m_size + 1] = PQEntry(value, priority);
    int level = m_size + 1;
    bubbleUp(level);
    m_size++;
}

// O(log(N))
// This function compares the son entry to the father entry.
// If the son entry has higher priority, it swaps the position of two entries.
void HeapPriorityQueue::bubbleUp(int level) {
    while (level > 0) {
        if (elements[level/2] > elements[level]) {
            PQEntry temp = elements[level];
            elements[level] = elements[level/2];
            elements[level/2] = temp;
            level = level/2;
        } else {
            break;
        }
    }
}

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
bool HeapPriorityQueue::isEmpty() const {
    return (m_size == 0);
}

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
string HeapPriorityQueue::peek() const {
    if (m_size == 0) {
        throw string ("Empty queue!");
    } else {
        return elements[1].value;
    }
}

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
int HeapPriorityQueue::peekPriority() const {
    if (m_size == 0) {
        throw string ("Empty queue!");
    } else {
        return elements[1].priority;;
    }
}

// O(1)
// See VectorPriorityQueue.h for documentation of each member function.
int HeapPriorityQueue::size() const {
    return m_size;
}

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << queue.toString();
    return out;
}

// O(N)
// See VectorPriorityQueue.h for documentation of each member function.
string HeapPriorityQueue::toString() const {
    string result = "{";
    for (int i = 1; i <= m_size; i++) {
        result += "\"" + elements[i].value + "\":" + integerToString(elements[i].priority);
        if (i < m_size) {
            result += ", ";
        }
    }
    result += "}";
    return result;
}

// O(N)
// This function checks the size and the capacity of the array.
// If the size is full, it copies everything into the a new array with size twice as big.
void HeapPriorityQueue::checkResize() {
    if (m_size == capacity - 1) {
        PQEntry* oldElements = elements;
        elements = new PQEntry[2 * capacity]();
        for (int i = 0; i <= m_size; i++) {
            elements[i] = oldElements[i];
        }
        delete[] oldElements;
        capacity *= 2;
    }
}
