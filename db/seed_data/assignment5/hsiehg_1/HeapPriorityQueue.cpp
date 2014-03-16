// HeapPriorityQueue.cpp
// This file contains the implementation of the HeapPriorityQueue structure.

#include "HeapPriorityQueue.h"

/*
 * Constructor for HeapPriorityQueue.
 * Creates a new array of capacity 10. Size starts at 0 because there are no elements in the queue.
 */
HeapPriorityQueue::HeapPriorityQueue() {
    m_array = new PQEntry[10];
    m_size = 0;
    capacity = 10;
}

/*
 * Destructor for HeapPriorityQueue.
 */
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] m_array;
}

/*
 * Changes the priority of a specified element.
 */
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    if (m_size == 0) {
        throw "Error: priority queue is empty";
    }
    int i = getIndex(value, newPriority);
    bubbleUp(i);
}

/*
 * Bubbles up an element of the array given its index. The element is bubbled up if it is less than the element being compared.
 */
void HeapPriorityQueue::bubbleUp (int startIndex) {
    int index = startIndex / 2;
    while (index > 0) {
        if (m_array[startIndex] < m_array[index]) {
            swapElements(startIndex, index);
            startIndex = index;
            index /=  2;
        } else {
            break;
        }
    }
}

/*
 * Returns the index of the specified value that the user wants to change.
 * Throws an exception if the new priority is greater than the old priority.
 */
int HeapPriorityQueue::getIndex(string value, int newPriority) {
    int elementIndex;
    for (int i = 1; i < m_size + 1; i++) {
        if (m_array[i].value == value) {
            if (newPriority < m_array[i].priority) {
                m_array[i].priority = newPriority;
                elementIndex = i;
                break;
            }
            else {
                throw "Error: new priority cannot be greater than old priority";
            }
        }
    }
    return elementIndex;
}

/*
 * Deletes all elements in the queue.
 */
void HeapPriorityQueue::clear() {
    m_size = 0;
}

/*
 * Removes the first element of the array and replaces it with the last element. Returns the string value of the first element.
 * If the array is empty, a string exception is thrown.
 */
string HeapPriorityQueue::dequeue() {
    if(m_size > 0) {
        PQEntry maxEntry = moveLastToTop();
        bubbleDown();
        return maxEntry.value;
    } else {
        throw "Error: priority queue is empty";
    }

}

/*
 * Bubbles down the PQEntry at index 1 of the array if it is less urgent than any of its children.
 */
void HeapPriorityQueue::bubbleDown() {
    int index = 1;
    int left = index * 2;
    int right = index * 2 + 1;
    if (m_size == 2 && m_array[index] > m_array[left]) {
        swapElements(index, left);
    }
    while (m_array[index] > m_array[left] || m_array[index] > m_array[right]) {
        if (index * 2 >= m_size) {
            break;
        }
        if (m_array[left] < m_array[right]) {
            swapElements(index, left);
            index *= 2;
        } else {
            swapElements(index, right);
            index = index * 2 + 1;
        }
        right = index * 2 + 1;
        left = index * 2;
    }
}

/*
 * Returns the PQEntry at index 1 and then replaces it with the PQEntry at the last index. Decreases size of the queue by 1.
 */
PQEntry HeapPriorityQueue::moveLastToTop() {
    PQEntry maxEntry = m_array[1];
    m_array[1] = m_array[m_size];
    m_array[m_size] = PQEntry("", 0);
    m_size--;
    return maxEntry;
}

/*
 * Adds an element to the queue.
 * Checks if the array needs to be resized before adding the element to the end of the array and bubbling it up.
 */
void HeapPriorityQueue::enqueue(string value, int priority) {
    checkResize();
    int pos = m_size + 1;
    m_array[pos] = PQEntry(value, priority);
    bubbleUp(pos);
    m_size++;
}

/*
 * Swaps two elements in the array given the index of the first element and the index of the second element.
 */
void HeapPriorityQueue::swapElements(int index1, int index2) {
    PQEntry temp = m_array[index2];
    m_array[index2] = m_array[index1];
    m_array[index1] = temp;
}

/*
 * Checks to see if the size of the array is equal to the capacity. If the size is equal to the capacity, this means that the array must be made larger
 * before new elements are added.
 */
void HeapPriorityQueue::checkResize() {
    if (m_size == capacity) {
        PQEntry* oldArray = m_array;
        m_array = new PQEntry[capacity * 2];
        for (int i = 0; i < m_size; i++) {
            m_array[i] = oldArray[i];
        }
        delete [] oldArray;
        capacity *= 2;
    }
}

/*
 * Returns true if no elements are in the queue.
 */
bool HeapPriorityQueue::isEmpty() const {
    return m_size == 0;
}

/*
 * Returns the string element with the most urgent priority.
 */
string HeapPriorityQueue::peek() const {
    if (m_size == 0) {
        throw "Error: priority queue is empty";
    }
    return m_array[1].value;
}

/*
 * Returns the integer priority that is most urgent.
 */
int HeapPriorityQueue::peekPriority() const {
    if (m_size == 0) {
        throw "Error: priority queue is empty";
    }
    return m_array[1].priority;
}

/*
 * Returns the size of the priority queue.
 */
int HeapPriorityQueue::size() const {
    return m_size;
}

/*
 * Overrides the << operator so that the priority queue can be printed to the console.
 */
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if(!queue.isEmpty()) {
        out << queue.m_array[1];
        for (int i = 2; i < queue.m_size + 1; i++) {
            out << ", " << queue.m_array[i];
        }
    }
    out << "}";
    return out;
}
