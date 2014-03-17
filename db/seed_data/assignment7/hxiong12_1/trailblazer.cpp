/* This file implements trailblazer.h and runs the Trailblazer functions. */

#include "costs.h"
#include "trailblazer.h"

#include "pqueue.h"
#include "queue.h"
#include "stack.h"
#include "set.h"
#include "map.h"
#include "strlib.h"
using namespace std;

/*Recursive helper for depth first search. Returns true is a path is found; otherwise
 * returns false.*/
bool dfs(Vector<Vertex*>& path, BasicGraph& graph, Vertex* start, Vertex* end) {
    path.add(start);
    start->visited = true;
    start->setColor(GREEN);
    if (start == end) return true;
    for (Edge* edge : start->edges) {
        if (!edge->finish->visited && dfs(path, graph, edge->finish, end)) return true;
    }
    path.remove(path.size() - 1);
    start->setColor(GRAY);
    return false;
}

/*Depth-first search function. If a path exists, returns a Vector<Vertex*> filled
 * with the vertices that comprise a path; else returns empty vertex.
 *Calls on the dfs helper function.*/
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    dfs(path, graph, start, end);
    return path;
}

/*A helper function called on in breadth-first search,
 * Dijkstra's, and A* that "follows" the found path back to its beginning,
 * and fills in a corresponding Vector<Vertex*> of the path.*/
void backtrackPath (Vector<Vertex*>& path, Vertex* end) {
    Stack<Vertex*> prepath;
    while (end->previous != NULL) {
        prepath.push(end);
        end = end->previous;
    }
    prepath.push(end); //pushes start value; if (start == end) pushes this value onto stack
    while (!prepath.isEmpty()) {
        path.add(prepath.pop());
    }
}

/*Breadth-first search function. Returns a Vector<Vertex*> comprised of the steps in the pathway.*/
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> vqueue;

    //begin breadth-first search algorithm
    vqueue.enqueue(start);
    start->visited = true;
    while (!vqueue.isEmpty()) {
        Vertex* next = vqueue.dequeue();
        next->setColor(GREEN); //dequeued
        if (next == end) { //a path exists!
            backtrackPath(path, next);
            break;
        }
        for (Edge* edge : next->edges) {
            if (!edge->finish->visited) { //for each unvisited neighbor of next
                edge->finish->visited = true; //visited
                edge->finish->previous = next; //points to previous node
                vqueue.enqueue(edge->finish);
                edge->finish->setColor(YELLOW); //enqueued
            }
        }
    }
    //if here no path has been found, path is empty
    return path;
}

/*Helper method for Dijkstra's or A*.
 * This method updates the pqueue after the program visits an edge, and calculates the cost to traverse along it.
 *Three cases:
 * 1) if v is in the pqueue, and newPriority < v's priority
 *    ===>update v's priority to be newPriority
 * 2) if v is in the pqueue, and newPriority > v's priority (relevant for cost + h);
 *    ===>remove v from queue
 *    ===>enqueue a new v with the newPriority
 * 3) if v is not in the pqueue
 *    ===>enqueue v with new Priority
 * NET EFFECT: the pqueue now has an entry with an updated Priority value
 * */
void changeOrEnqueue(PriorityQueue<Vertex*>& pqueue, Vertex*& v, double newPriority) {
    for (int i = 0; i < pqueue.size(); i++) {
        Vertex* test = pqueue.peek();
        int oldPriority = pqueue.peekPriority();
        if (test == v) { //pqueue contains v
            if (newPriority <= oldPriority) { //case 1
                pqueue.changePriority(v, newPriority);
            } else {                          //case 2
                pqueue.dequeue();
                pqueue.enqueue(v, newPriority);
            }
            return;
        }
    }
    pqueue.enqueue(v, newPriority);           //case 3
}

/*Dijkstra's algorithm function. Returns a Vector<Vertex*> comprised of the steps in the pathway.*/
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pqueue;

    //initialize cost of all vertices to infinity
    Set<Vertex*> vertices = graph.getVertexSet();
    for (Vertex* v : vertices) {
        v->cost = POSITIVE_INFINITY;
    }

    start->cost = 0; //set start's cost to 0
    pqueue.enqueue(start, start->cost); //and enqueue start

    //Dijkstra's algorithm
    start->setColor(YELLOW);
    while (!pqueue.isEmpty()) {
        Vertex* v = pqueue.dequeue();
        v->setColor(GREEN); //dequeued = GREEN
        v->visited = true;
        if (v == end) break; //a path exists!
        for (Edge* edge : v->edges) { //for all neighbors of v
            if (!edge->finish->visited) { //for each unvisited neighbor
                double cost = v->cost + edge->cost;
                Vertex* n = edge->finish;
                if (cost < n->cost) {
                    n->cost = cost;
                    n->previous = v;
                    n->setColor(YELLOW); //enqueued = YELLOW
                    changeOrEnqueue(pqueue, n, cost);
                }
            }
        }
    }
    //here, if no path is found Vector<Vertex*> path is empty
    backtrackPath(path, end);
    return path;
}

/* A* algorithm function. Returns a Vector<Vertex*> comprised of the steps in the pathway.*/
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pqueue;

    //initialize cost of all vertices to infinity
    Set<Vertex*> vertices = graph.getVertexSet();
    for (Vertex* v : vertices) {
        v->cost = POSITIVE_INFINITY;
    }
    start->cost = 0; //Set start's cost to 0
    pqueue.enqueue(start, start->cost + heuristicFunction(start, end)); //and enqueue it!

    //A* algorithm
    while (!pqueue.isEmpty()) {
        Vertex* v = pqueue.dequeue();
        v->setColor(GREEN); //dequeued = GREEN
        v->visited = true;
        if (v == end) break; //a path exists
        for (Edge* edge : v->edges) { //else, for each unvisited neighbor of n
            if (!edge->finish->visited) {
                double cost = v->cost + edge->cost;
                Vertex* n = edge->finish;
                if (cost < n->cost) {
                    n->cost = cost;
                    n->previous = v;
                    n->setColor(YELLOW);
                    changeOrEnqueue(pqueue, n, cost + heuristicFunction(n, end));
                }
            }
        }
    }
    backtrackPath(path, end);
    //if we get here and path is empty, then no path exists
    return path;
}

/*Returns a Set of Edges* that form a spanning tree using Kruskal's algorithm,
 * then turns it into a maze*/
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;

    //place all vertices into a map. Each entry in the map corresponds to a cluster of vertices.
    Set<Vertex*> allVertices = graph.getVertexSet();
    Map<int, Set<Vertex*> > clusterSet;
    int i = 0;
    for (Vertex* v : allVertices) {
        Set<Vertex*> cluster;
        cluster.add(v);
        clusterSet[i] = cluster;
        v->cost = (double) i;
        i++;
    }

    //enqueue edges into a priority queue with randomly assigned priorities
    Set<Edge*> allEdges = graph.getEdgeSet();
    PriorityQueue<Edge*> kEdgeSet;
    for (Edge* e : allEdges) {
        double rand = randomReal(0, 1);
        e->cost = rand;
        kEdgeSet.enqueue(e, e->cost);
    }

    //Kruskal's Algorithm
    while (clusterSet.size() > 1) { //there are more than one clusters
        Edge* e = kEdgeSet.dequeue();

        //finds the cluster that the start and the edge of the edge are connected to by calling the "cost" identifier.
        int startIndex = (int) e->start->cost;
        int finishIndex = (int) e->finish->cost;

        //if the edge connects two different clusters, combine the clusters
        if (startIndex != finishIndex) {
            for (Vertex* v : clusterSet[finishIndex]) { //update the cluster number identifier in the second clubster
                v->cost = startIndex;
            }
            clusterSet[startIndex] += clusterSet[finishIndex]; //merge the two clusters
            clusterSet.remove(finishIndex); //delete the old cluster
            mst.add(e); //add connecting edge to spanning tree, because it connected two different clusters
        }
    }
    return mst;
}

