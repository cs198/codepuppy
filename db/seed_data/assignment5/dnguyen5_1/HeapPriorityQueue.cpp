// Maintains a priority queue using a binary heap.

#include "HeapPriorityQueue.h"

//constructor. Initializes a pointer to array m_array of size 10, initializes number of elements
//in the array to 0, and keeps track of the size of the array with arrayCapacity.
HeapPriorityQueue::HeapPriorityQueue() {
    m_array = new PQEntry[10]();
    numArrayElements = 0;
    arrayCapacity = 10;
}

//deconstructor: deletes array elements.
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] m_array;
}

/* Changes priority of a given value IF newPriority is more urgent than the value's old priority.
 * If the given value is present in the queue and already has a more
 * urgent priority to the given new priority, or if the given value is not already in the
 * queue, the function throws a string exception.*/
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    if (isEmpty()) {
        throw "Error: priority queue is empty"; //string exception
    }
    else {
        int index = 1;
        bool valueFound = false;
        while (true) {
            if (m_array[index].value == value) {
                valueFound = true;
                if (newPriority < m_array[index].priority) {
                    m_array[index].priority = newPriority;
                    bubbleUp(index);
                    break;
                }
            }
            index++;
            if (index > numArrayElements && valueFound == false) {
                throw "Error: The given value is not found in this priority queue";
            }
            if (valueFound == true) {
                throw "Error: new priority cannot be greater than old priority";
            }
        }
    }
}

//clears priority queue. I chose to clear it by deleting all m_array elements (and clearing memory) and reinitializing the queue.
//Another way to do this would be to simply reset numArrayElements to 0 but it would leave previous elements stored in memory.
void HeapPriorityQueue::clear() {
    delete[] m_array;
    m_array = new PQEntry[10]();
    numArrayElements = 0;
    arrayCapacity = 10;
}

//Removes and returns most urgent element from the queue. Throws a string exception if the queue
//does not contain any elements.
string HeapPriorityQueue::dequeue() {
    if (isEmpty()) {
        throw "Error: priority queue is empty"; //string exception
    }
    int parentIndex = 1;
    string parentDequeued = m_array[parentIndex].value;
    m_array[parentIndex] = m_array[numArrayElements];
    numArrayElements--;
    while(true) {
        int higherPriorityChildIndex;
        int leftChildIndex = parentIndex*2;
        int rightChildIndex = parentIndex*2+1;
        if (numArrayElements == 1 || (rightChildIndex > numArrayElements && leftChildIndex > numArrayElements))  { //checks if indices are out of bounds
            break;
        }
        if ((leftChildIndex <= numArrayElements && rightChildIndex > numArrayElements) ||
                m_array[leftChildIndex].priority < m_array[rightChildIndex].priority ||
                (m_array[leftChildIndex].priority == m_array[rightChildIndex].priority && m_array[leftChildIndex].value < m_array[rightChildIndex].value)) {
            higherPriorityChildIndex = leftChildIndex;
        }
        else if (rightChildIndex <= numArrayElements){
            higherPriorityChildIndex = rightChildIndex;
        }
        //If the child entry has a higher priority than the parent entry, they are swapped.
        if (m_array[higherPriorityChildIndex].priority < m_array[parentIndex].priority
                || (m_array[higherPriorityChildIndex].priority == m_array[parentIndex].priority
                    && m_array[higherPriorityChildIndex].value < m_array[parentIndex].value)) {
            PQEntry temp = m_array[parentIndex];
            m_array[parentIndex] = m_array[higherPriorityChildIndex];
            m_array[higherPriorityChildIndex] = temp;
            parentIndex = higherPriorityChildIndex;
        }
        else {
            break;
        }
    }
    return parentDequeued;
}

//adds an entry to the priority queue.
void HeapPriorityQueue::enqueue(string value, int priority) {
    numArrayElements++;
    checkResize();
    int newElementIndex = numArrayElements;
    m_array[newElementIndex] = PQEntry(value, priority);
    bubbleUp(newElementIndex);
    cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
}

//This bubble Up method is called in both enqueue and change priority. Takes in an index as a parameter
//and bubbles up the entry at that index.
void HeapPriorityQueue::bubbleUp(int index) {
    while(true) {
        int parentElementIndex = index/2;
        if (m_array[index].priority < m_array[parentElementIndex].priority || (m_array[index].priority == m_array[parentElementIndex].priority && m_array[index].value < m_array[parentElementIndex].value)) {
            PQEntry temp = m_array[parentElementIndex];
            m_array[parentElementIndex] = m_array[index];
            m_array[index] = temp;
            index = parentElementIndex;
        }
        else {
            break;
        }
    }
}

//returns true if queue is empty, returns false if not.
bool HeapPriorityQueue::isEmpty() const {
    if (numArrayElements == 0) return true;
    else return false;
}

//returns the string element with the highest priority. Throws string exception is queue is empty.
string HeapPriorityQueue::peek() const {
    if (isEmpty()) throw "Error: priority queue is empty";
    else return m_array[1].value;
}

//returns the most urgent integer priority. Throws string exception is queue is empty.
int HeapPriorityQueue::peekPriority() const {
    if (isEmpty()) throw "Error: priority queue is empty";
    else return m_array[1].priority;
}

//returns the number of elements in the array.
int HeapPriorityQueue::size() const {
    return numArrayElements;
}

//if the number of elements in the array equals the array's capacity, this method
//doubles the arrays capacity by creating a new temporary array and setting m_array equal
//to it.
void HeapPriorityQueue::checkResize() {
    if (size() == arrayCapacity) {
        PQEntry* temp = new PQEntry[2 * arrayCapacity]();
        for (int i = 0; i < numArrayElements; i++) {
            temp[i] = m_array[i];
        }
        delete[] m_array;
        m_array = temp;
        arrayCapacity *= 2;
        delete[] temp; //frees memory taken up by temporary array
    }
}

//prints priority queue to console.
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    PQEntry* m_array2 = queue.m_array;
    int numElements = queue.numArrayElements;
    out << "{";
    for (int i = 1; i <= numElements; i++) {
        out << m_array2[i];
        if (i != numElements)
            out << ", ";
    }
    out << "}";
    return out;
}
