// This is the Heap Priority Queue file
// that uses a heap to keep track of the
// vector.

#include "HeapPriorityQueue.h"

/*
 * Creates a new Heap Priority Queue.
 * BigO: O(1)
 */
HeapPriorityQueue::HeapPriorityQueue() {
    m_array = new PQEntry[10]();
    m_size = 0;
    m_capacity = 10;
}

/*
 * Deletes the queue.
 * O(1)
 */
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] m_array;

}

/*
 * This changes the priority of a specific value in the queue.
 * If it needs to switch with a higher parent, then it will do that
 * until it has reached the correct spot and no longer needs to bubble
 * up.
 * O(log N)
 */
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    int current = 1;
    while (m_array[current].value != value)
        current++;
    m_array[current] = PQEntry(value,newPriority);
    int current_size = current;
    string current_value = value;
    int current_priority = newPriority;
    while (m_array[current_size/2].priority > current_priority) {
        m_array[current_size] = PQEntry(m_array[current_size/2].value,m_array[current_size/2].priority);
        m_array[current_size/2] = PQEntry(current_value,current_priority);
        current_size = current_size/2;
        current_value = m_array[current_size/2].value;
        current_priority = m_array[current_size/2].priority;
    }
}

/*
 * Clears the priority queue.
 * O(1)
 */
void HeapPriorityQueue::clear() {
    m_size = 0;
}

/*
 * This dequeues a value and then replaces the top value with the
 * bottom value, and then keeps moving the bubble until it reaches
 * the appropriate spot and it has switched enough with its children.
 * O(log N)
 */
string HeapPriorityQueue::dequeue() {
    string value = m_array[1].value;
    m_array[1] = m_array[m_size];
    m_size--;
    string temp_v = "";
    int temp_p = 0;
    int current_size = 1;
    int replace = 0;
    while (((current_size*2 < m_size) && m_array[current_size*2].priority < m_array[current_size].priority) || ((current_size*2+1 < m_size) && m_array[current_size*2 + 1].priority < m_array[current_size].priority)) {
        if (m_array[current_size*2].priority < m_array[current_size*2+1].priority) {
            replace = current_size * 2;
        } else {
            replace = current_size * 2 + 1;
        }
        temp_v = m_array[replace].value;
        temp_p = m_array[replace].priority;
        m_array[replace] = PQEntry(m_array[current_size].value,m_array[current_size].priority);
        m_array[current_size] = PQEntry(temp_v,temp_p);
        current_size = replace;
    }
    return value;
}

/*
 * This enqueues a value onto the queue. It first makes sure that
 * the array has the capacity to hold the new value; if not, it makes
 * a copy of the old array into a new array with a higher capacity.
 * The function then adds the value and checks the parents
 * to see if the entry needs to be bubbled up into a higher index.
 * O(log N)
 */
void HeapPriorityQueue::enqueue(string value, int priority) {
    m_size++;
    if (m_size >= m_capacity) {
        m_capacity++;
        PQEntry* new_m_array = new PQEntry[m_capacity]();
        for (int i = 1; i <= size(); i++) {
            new_m_array[i] = m_array[i];
        }
        m_array = new_m_array;
        delete[] new_m_array;
    }
    m_array[m_size] = PQEntry(value,priority);
    int current_size = m_size;
    while (m_array[current_size/2].priority > priority) {
        m_array[current_size] = PQEntry(m_array[current_size/2].value,m_array[current_size/2].priority);
        m_array[current_size/2] = PQEntry(value,priority);
        current_size = current_size/2;
    }

}

/*
 * Checks to see if the queue is empty.
 * O(1)
 */
bool HeapPriorityQueue::isEmpty() const {
    return (size() == 0);
}


/*
 * Checks to see the value of the top
 * entry on the queue.
 * O(1)
 */
string HeapPriorityQueue::peek() const {
    return m_array[1].value;
}

/*
 * Checks to see the priority of the top
 * entry on the queue.
 * O(1)
 */
int HeapPriorityQueue::peekPriority() const {
    return m_array[1].priority;
}

/*
 * Checks the size of the queue.
 * O(1)
 */
int HeapPriorityQueue::size() const {
    return m_size;
}


/*
 * Outputs the values of the functions.
 * O(N)
 */
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    int n = 0;
    if (queue.size() != 0) {
        for (int i = 1; i <= queue.size(); i++) {
            out << "\"" << queue.m_array[i].value << "\":" << queue.m_array[i].priority;
            n++;
            if (n < queue.size()) out << ", ";
        }
    }
    out << "}";
    return out;
}
