/* Name: Kevin Vo
 * TA: Jimmy Lee
 * This cpp file that implements the vector as a data
 *structure for a priority queue. This is the file
 * that contains all the member functions and the
 * constructor.
*/

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
    /* I don't need to do anything with this
     * because it is a parameterless constructor
    */

}

VectorPriorityQueue::~VectorPriorityQueue() {
    /* does not need a destructor because
       it does not dynamically allocate
       memory
    */
}

/*This function loops over the vector finds the matching value
and replaces the element with a new matching priority number.
This function runs in O(N) time.*/
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 0; i<vector.size(); i++){
        if (value == vector[i].value){
            if (vector[i].priority < newPriority){
                throw "new priority cannot be greater than old priority";
            }
            vector.remove(i);
            vector.insert(i, PQEntry(value,newPriority));
        }
    }
}

/* This function clears the entire vector. It runs in O(1) constant time.
*/
void VectorPriorityQueue::clear() {
    vector.clear();
}

/* This function loops over the vector to find
 * the element with the highest priority. It
 * returns it and removes it from the vector.
 * This function runs in O(N) time.
*/
string VectorPriorityQueue::dequeue() {
    if (vector.isEmpty()){
        throw "The vector is empty";
    }
    int index = 0;
    PQEntry result = vector[0];
    for (int i = 0; i < vector.size(); i++){
        if (vector[i] <= result){
            result = vector[i];
            index = i;
        }
    }
    vector.remove(index);
    return result.value;
}

/*Enqueue is just a matter of adding a PQEntry object into the vector
which by default adds to the end. This function runs in O(1) time. */
void VectorPriorityQueue::enqueue(string value, int priority) {
    vector.add(PQEntry(value, priority));
    cout << "Enqueued " << "\"" << value << "\"" << " with a priority " << priority << endl;
}
/* This is a boolean function that returns true
 * if the vector is empty and false if the
 * vector contains at least one element.
 * This function runs in O(1) time.
*/
bool VectorPriorityQueue::isEmpty() const {
    if (vector.size() == 0){
        return true;
    }else{
        return false;
    }
}

/* This function loops over the vector
 * finds the element with the highest priority
 * and returns the value without removing the
 * element itself. It runs in O(N).
*/
string VectorPriorityQueue::peek() const {
    if (vector.isEmpty()){
        throw "The vector is empty";
    }
    string result = vector[0].value;
    for (int i = 0; i < vector.size(); i++){
        if (vector[i] < result){
            result = vector[i].value;
        }
    }
    return result;
}

/* This function loops over the vector, finds the
 * element with the most priority and returns the
 * value of the priority. If the vector is empty
 * it throws an exception. It runs in O(N).
*/
int VectorPriorityQueue::peekPriority() const {
    if (vector.isEmpty()){
        throw "The vector is empty.";
    }
    int result = vector[0].priority;
    for (int i = 0; i < vector.size(); i++){
        if (vector[i].priority < result){
            result = vector[i].priority;
        }
    }
    return result;
}

/* The function returns the size of the
 * vector. It runs in O(1).
*/
int VectorPriorityQueue::size() const {
    return vector.size();
}

/* The function prints out the contents of the
 * vector. It runs in O(1).
*/
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << queue.vector << endl;
    return out;
}
