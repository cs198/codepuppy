/*This .cpp file implements a priority queue using a heap.
 * A priority queue is a sorted queue where the elements are sorted by priority,
 * breaking ties by comparing the string elements themselves.*/

#include "HeapPriorityQueue.h"

/* Constructor. Initializes the capacity, the number of elements, and the heap with their null values */
HeapPriorityQueue::HeapPriorityQueue() {
    capacity = 10;
    numElements = 0;
    heap = new PQEntry[capacity]();
}

/* Destructor. Deletes the heap */
HeapPriorityQueue::~HeapPriorityQueue() {
    delete [] heap;
}

/* Modifies the priority of a given existing value in the queue */
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    // Go through heap to find the desired value
    for (int i = 1; i < numElements + 1; i++) {
        if (heap[i].value == value) {
            //If found, update the priority
            if (heap[i].priority < newPriority) {
                heap[i].priority = newPriority;
                bubbleDown(i);
            } else {
                heap[i].priority = newPriority;
                bubbleUp(i);
            }
            return;
        } else {
            //String exception if value not found
            throw ("Value not found in queue");
        }
    }

}

/* Helper function to "bubble up" or swapping the element at given position
 * with its parent index (i/2) so long as it has a more urgent (lower) priority than its parent */
void HeapPriorityQueue::bubbleUp(int pos) {
    //While child has more urgent priority than parent
    while (pos > 1 && heap[pos] < heap[pos/2]) {
        //Swap elements
        PQEntry temp = heap[pos];
        heap[pos] = heap[pos/2];
        heap[pos/2] = temp;
        pos = pos/2;
    }
}

/* Helper function to "bubble down" or swap the element at a given position
 * with its more urgent-priority child index (i*2 or i*2+1) so long as it has a
 * less urgent (higher) priority than its child */
void HeapPriorityQueue::bubbleDown(int pos) {

    //While the position is within range
    while (pos*2 <= numElements) {

        //Check the right side to see if it's null
        bool secondIsNull = ((pos*2+1) > numElements);
        int index;

        //Set index to appropriate child index
        if (secondIsNull || heap[pos*2] < heap[pos*2+1]) {
            index = pos*2;
        } else {
            index = pos*2+1;
        }

        //Swap elements if parent has more urgent priority than child
        if (heap[pos] > heap[index]) {
            PQEntry temp = heap[pos];
            heap[pos] = heap[index];
            heap[index] = temp;
            pos = index;
        } else {
            return;
        }
    }
}

/* Removes all elements in priority queue */
void HeapPriorityQueue::clear() {
    //Loop through and set all elements in array to the null value of PQEntry
    for (int i = 1; i < numElements + 1; i++) {
        heap[i] = PQEntry("", 0);
    }

    //Set number of elements to 0
    numElements = 0;
}

/* Remove and return the element with the most urgent priority from
priority queue */
string HeapPriorityQueue::dequeue() {
    if (isEmpty()) {
        throw ("The queue is empty.");
    }

    PQEntry entry = heap[1];
    heap[1] = heap[numElements];
    heap[numElements] = PQEntry("", 0);
    numElements--;
    bubbleDown(1);


    return entry.value;
}

/* add the given string value into your priority queue with
the given priority */
void HeapPriorityQueue::enqueue(string value, int priority) {

    //If capacity is full, double capacity
    if (numElements + 1 == capacity) {
        doubleCapacity();
    }

    //Add to the end of the heap and bubble up
    heap[numElements+1] = PQEntry(value, priority);
    int pos = numElements+1;
    bubbleUp(pos);

    //Increment number of elements
    numElements++;
}

/* Helper function. Doubles the capacity of the heap */
void HeapPriorityQueue::doubleCapacity() {
    //Create heap with double capacity
    PQEntry* copy = new PQEntry[capacity*2]();
    //Copy elements of heap
    for (int i = 1; i < numElements + 1; i++) {
        copy[i] = heap[i];
    }
    //Delete the old heap
    delete[] heap;

    //Update heap
    heap = copy;
    capacity = capacity*2;
}

/* Returns true if queue is empty, false otherwise */
bool HeapPriorityQueue::isEmpty() const {
    return (numElements == 0);
}

/* Returns value of most urgent priority */
string HeapPriorityQueue::peek() const {
    if (isEmpty()) {
        //String exception if empty queue
        throw ("The queue is empty.");
    }
    return heap[1].value;
}

/* Returns integer of most urgent priority */
int HeapPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        //String exception if empty queue.
        throw ("The queue is empty.");
    }
    return heap[1].priority;
}

/* Returns the size of the queue */
int HeapPriorityQueue::size() const {
    return numElements;
}

/*a << operator for printing priority queue to the console */
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    //Loop through heap and print value and priority
    for (int i = 1; i < queue.numElements + 1; i++) {
        out << queue.heap[i].value << ":" << queue.heap[i].priority;
        if (i < queue.numElements) out << ", ";
    }
    out << "}";
    return out;
}
