// This is a .cpp file that represents the Vector
// priority queue class. The queue uses a vector
// to run a proper priority queue.

#include "VectorPriorityQueue.h"

VectorPriorityQueue::VectorPriorityQueue() {
    pQVector;
}
/**
 * @brief VectorPriorityQueue::~VectorPriorityQueue
 * ~VectorPriorityQueue() emptys out the memory that
 * was used when the queue is terminated.
 * O(1)
 */
VectorPriorityQueue::~VectorPriorityQueue() {
     pQVector.clear();
}
/**
 * @brief VectorPriorityQueue::changePriority
 * @param value
 * @param newPriority
 * change priority accepts a string and a priority
 * and then changes the priority for the value inputed.
 * newPriority must be less than current priority
 * O(N)
 */
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    if(pQVector.size() == 0){
        throw "No names in queue.";
    }
    for(int i = 0; i < pQVector.size(); i++){
        if(pQVector[i].value == value){
            if(pQVector[i].priority > newPriority){
                pQVector[i].priority = newPriority;
            }
            return;
        }
    }
}
/**
 * @brief VectorPriorityQueue::clear
 * clear emptys the queue.
 * O(1)
 */
void VectorPriorityQueue::clear() {
    pQVector.clear();
}

/**
 * @brief VectorPriorityQueue::dequeue
 * @return dequeued
 * removes the highest priority from the cue.
 * O(N)
 */
string VectorPriorityQueue::dequeue() {
    int holder = findHighest();
    string current = pQVector[holder].value;
    pQVector.remove(holder);
    return current;
}

/**
 * @brief VectorPriorityQueue::enqueue
 * @param value
 * @param priority
 * enqueue takes in a value and a priority
 * and adds them to the appropriate
 * place in the queue.
 * O(1)
 */
void VectorPriorityQueue::enqueue(string value, int priority) {
    pQVector.add(PQEntry(value, priority));
}

/**
 * @brief VectorPriorityQueue::isEmpty
 * @return
 * checks to see if the queue is empty.
 * If it is returns true, else false.
 * O(1)
 */
bool VectorPriorityQueue::isEmpty() const {
    if(pQVector.size() == 0){
            return true;
    } else return false;

}

/**
 * @brief VectorPriorityQueue::peek
 * @return
 * Retruns the value of the highest priority
 * entry in the queue
 * O(N)
 */
string VectorPriorityQueue::peek() const {
    int holder = findHighest();
    string const current = pQVector[holder].value;
    return current;
}
/**
 * @brief VectorPriorityQueue::peekPriority
 * @return
 * Retruns the priority of the highest priority
 * entry in the queue
 * O(N)
 */
int VectorPriorityQueue::peekPriority() const {
    int holder = findHighest();
    int current = pQVector[holder].priority;
    return current;
}

/**
 * @brief VectorPriorityQueue::findHighest
 * @return
 * fidnHighest is a private method that finds the
 * highest priority entry.
 * O(N)
 */
int VectorPriorityQueue::findHighest() const{
    if(pQVector.size() == 0){
        throw "No names in queue.";
    }
    int place;
    for(int i = 0; i < pQVector.size(); i++){
        if(i == 0){
            place = i;
        }else if(pQVector[i].priority < pQVector[place].priority){
            place = i;
        }
    }
    return place;
}

/**
 * @brief VectorPriorityQueue::size
 * @return number of objects in the queue.
 * O(1)
 */
int VectorPriorityQueue::size() const {
    return pQVector.size();
}

/**
 * @brief operator <<
 * @param out
 * @param queue
 * @return
 * prints out all of the objects in the queue.
 * O(N)
 */
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    cout << "{";
    int counter = 0;
    for(int i = 0; i < queue.size(); i++){
        if(counter != queue.size()-1){
            cout <<  queue.pQVector[i] << ", ";
        }else cout <<  queue.pQVector[i];
        counter++;
    }
    cout << "}" << endl;
    return out;
}
