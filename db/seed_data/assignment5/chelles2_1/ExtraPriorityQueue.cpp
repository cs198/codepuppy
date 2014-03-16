/* This .cpp file implements a priority queue using a map */

#include "ExtraPriorityQueue.h"

ExtraPriorityQueue::ExtraPriorityQueue() {
    //No constructor needed
}

ExtraPriorityQueue::~ExtraPriorityQueue() {
    //Destructor. Calls function to clear
    clear();
}

/* Updates priority of the given value in the queue */
void ExtraPriorityQueue::changePriority(string value, int newPriority) {
    bool found = false;

    //Loop through the map
    for (int priority: map) {

        //Get the queue of values of a given priority
        Queue<string>* q = map.get(priority);
        string dequeued;

        //Loop through the queue of values of that priority
        for (int i = 0; i < q->size(); i++) {

            //Dequeue
            dequeued = q->dequeue();

            //If the desired value is found and hasn't been found yet
            if (dequeued == value  && !found) {
                found = true;
                //Enqueue the value with new priority in the map
                enqueue(value, newPriority);

                //If there is no value of the priority left, remove it from map
                //and delete from memory
                if (q->isEmpty()) {
                    delete q;
                    map.remove(priority);
                }
            } else {
                //Restore the queue
                q->enqueue(dequeued);
            }
        }
    }
    //String exception if not found
    if (!found) throw ("The given value is not found in the queue.");
}

//Remove all elements from queue
void ExtraPriorityQueue::clear() {
    for (int priority: map) {
        delete map.get(priority);
    }
}

//Return value associated with most urgent priority and remove it from queue
string ExtraPriorityQueue::dequeue() {
    if (isEmpty()) {
        //String exception
        throw ("Queue is empty.");
    }
    string result;
    //Loop through every key in map
    for (int priority: map) {
        //Find most urgent priority and remove and return
        Queue<string>* q = map.get(priority);
        result = q->dequeue();
        if (q->isEmpty()) {
            delete q;
            map.remove(priority);
        }
        return result;
    }
}

//Add the given value and priority to the queue
void ExtraPriorityQueue::enqueue(string value, int priority) {
    Queue<string>* q;
    //If that priority doesn't exist, add it a new key
    if (!map.containsKey(priority)) {
        q = new Queue<string>;
        q->enqueue(value);
        map.put(priority, q);
    } else {
        //Get the queue of that priority
        q = map.get(priority);
        bool found = false;

        //Enqueue all the ones less than (alphabetically) the given value
        if (q->peek() > value) {
            q->enqueue(value);
            for (int i = 0; i < q->size()-1; i++) {
                q->enqueue(q->dequeue());
            }
        } else {
            for (int i = 0; i < q->size(); i++) {
                string dequeued = q->dequeue();
                if (dequeued < value) {
                    q->enqueue(dequeued);
                } else {
                    //Then enqueue the value and the ones greater than given value
                    found = true;
                    q->enqueue(value);
                    q->enqueue(dequeued);
                    for (int j = i+1; j < q->size(); j++) {
                        dequeued = q->dequeue();
                        q->enqueue(dequeued);
                    }
                    break;
                }
            }
            if (!found) {
                q->enqueue(value);
            }
        }
        //Put it in map
        map.put(priority, q);
    }
}


//Returns true if empty equeue
bool ExtraPriorityQueue::isEmpty() const {
    return map.isEmpty();
}


//Returns the element with most urgent
string ExtraPriorityQueue::peek() const {
    if (isEmpty()) {
        throw ("Queue is empty.");
    }
    string result;
    for (int priority: map) {
        Queue<string>* q = map.get(priority);
        result = q->peek();
        return result;
    }
}

//Returns most urgent priority value
int ExtraPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw ("Queue is empty.");
    }
    for (int priority: map) {
        return priority;
    }
}


//Returns size of queue
int ExtraPriorityQueue::size() const {
    if (isEmpty()) return 0;
    int counter = 0;
    for (int priority: map) {
        counter += map.get(priority)->size();
    }
    return counter;
}

//Outputs onto console
ostream& operator<<(ostream& out, const ExtraPriorityQueue& queue) {
    int numKeys = 0;
    out << "{";
    for (int priority: queue.map) {
        Queue<string>* q = queue.map.get(priority);
        for (int i = 0; i < q->size(); i++) {
            string value = q->dequeue();
            out << value << ":" << priority;
            q->enqueue(value);
            if (i < q->size()-1) out << ",";
        }
        numKeys++;
        if (queue.map.size() > numKeys) out << ",";
    }
    out << "}";
    return out;
}
