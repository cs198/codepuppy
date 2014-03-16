/* Name: Kevin Vo
 * TA: Jimmy Lee
 * Credit: My helper functions were borrowed from Marty Stepp's lecture slides. And
 * I used his lecture slides on the ArrayList to help code the functions. I also consulted
 * Wikipedia- Binary Heap to strategize the algorithms.
 * This program implements a queue using a heap. The data structure used is an array.
*/

#include "HeapPriorityQueue.h"

/* This constructor initializes the member variables.
*/
HeapPriorityQueue::HeapPriorityQueue() {
    elements = new PQEntry[10]();
    a_size = 0;
    capacity = 10;
}
/*The destructor deletes the entire array*/
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] elements;
}
/* The function loops over the heap, finds the value and then bubbles it
 * up from that position. This function runs in O(logN) time because
 * the bubbling up jumps by powers of 2. It throws and exception when
 * the new priority is greater than the old priority.
*/
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    string result;
    PQEntry entry = PQEntry (value, newPriority);
    int index = a_size;
    int pIndex = index/2;
    for (int i = 1; i < a_size+1; i++){
        result = elements[i].value;
        if (result == value) {
            elements[i] = entry;
            index = i;
            break;
        }
    }
    pIndex = index/2;
    if (newPriority > entry.priority){
        throw "New priority cannot be greater than the old priority";
    }
    bubbleUp(index,pIndex);
}
/*Sets the size to zero and removes all the elements by doing so.
This function runs in O(1) time.*/
void HeapPriorityQueue::clear() {
    a_size = 0;
}
/*This function dequeues the first element of the heap and then bubbles
down to form the new heap. This function runs in O(logN)
*/
string HeapPriorityQueue::dequeue() {
    if (a_size == 0){
        throw "Queue is empty";
    }

    //pull out the first element
    string value = elements[1].value;
    elements [1] = PQEntry("",0);
    a_size--;

    //swaps the last child with the front
    int index = 1;
    elements[index] = elements[a_size+1];
    bubbleDown(index);
    return value;
}
/* This function enqueues by first placing the new element at the end of
 * the heap then it bubbles up to the right location. It runs in O(logN).
*/
void HeapPriorityQueue::enqueue(string value, int priority) {
    checkResize();
    a_size++;
    elements[a_size] = PQEntry(value, priority);;
    int index = a_size;
    int pIndex = index/2;
    bubbleUp(index,pIndex);
    cout << "Enqueued " << "\"" << value << "\"" <<" with priority " << priority<<endl;
}
/*Checks to see if the heap is empty. It runs in O(1)*/
bool HeapPriorityQueue::isEmpty() const {
    return a_size == 0;
}
/*Returns the value of the first element. It runs in O(1)*/
string HeapPriorityQueue::peek() const {
    if (a_size == 0){
        throw "queue is empty";
    }
    return elements[1].value;
}
/*Returns the highest priority number. It runs in O(1)*/
int HeapPriorityQueue::peekPriority() const {
    if (a_size == 0){
        throw "queue is empty";
    }
    return elements[1].priority;
}
/*Returns the size of the heap. It runs in O(1)*/
int HeapPriorityQueue::size() const {
    return a_size;
}
/*Checks to see if the array needs to be resized.
This function runs in O(N) time.*/
void HeapPriorityQueue::checkResize(){
    if (a_size == capacity) {
        PQEntry* oldElements = elements;
        elements = new PQEntry[2 * capacity]();
        for (int i = 1; i < a_size; i++) {
            elements[i] = oldElements[i];
        }
        delete[] oldElements;
        capacity *= 2;
    }
}
/* Checks to see if the conditions to bubble are valid. That means
 * that the priority of the child must be higher and the string value of the parent
 * must be greater than the child. It runs in O(1).
*/
bool HeapPriorityQueue::valid(int index, int pIndex){
    return (elements[index].priority < elements[pIndex].priority) || (elements[index].priority == elements[pIndex].priority && elements[index].value < elements[pIndex].value);
}
/*Implements the bubbling up behavior. This function runs
in O(logN) time. */
void HeapPriorityQueue::bubbleUp(int index, int pIndex){
    PQEntry* temp = new PQEntry[1]();
    while(index > 0 && valid(index,pIndex)){
        temp[0] = elements[index];
        elements[index] = elements[pIndex];
        elements[pIndex] = temp[0];
        index = index/2;
        pIndex = index/2;
    }
}
/*Implements the bubbling down behavior. This function runs
in O(logN) time. */
void HeapPriorityQueue::bubbleDown(int index){
    int sPriority = min(elements[index*2].priority,elements[index*2+1].priority);
    PQEntry* temp = new PQEntry[1]();
    while (elements[index] != PQEntry("",0) && elements[index].priority > sPriority){
        if (sPriority < elements[index].priority){
            if (elements[index*2].priority < elements[index*2+1].priority){
                //goes left
                temp[0] = elements[index];
                elements[index] = elements[index*2];
                elements[index*2] = temp[0];
                index = index*2;
            }else{
                //goes right
                temp[0] = elements[index];
                elements[index] = elements[index*2+1];
                elements[index*2+1] = temp[0];
                index = index*2 + 1;
            }
        }
        //as long as it is not a leaf find the new priority
        if (elements[index*2] != PQEntry("",0), elements[index*2+1] != PQEntry("",0)){
            sPriority = min(elements[index*2].priority,elements[index*2+1].priority);
        }else{
            break;
        }
    }
}

/*Overloads the operator to print out the HeapPriorityQueue object*/
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if (!queue.isEmpty()){
        for (int i = 1; i <= queue.size(); i++){
            if (i == 1){
                out << queue.elements[i];
            }else{
                out << ", " << queue.elements[i];
            }
        }
    }
    out << "}";
    return out;
}
