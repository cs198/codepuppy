//trailblazer.cpp

#include "costs.h"
#include "trailblazer.h"
#include "pqueue.h"
#include "hashmap.h"
#include "hashset.h"

using namespace std;

/*Function Prototypes*/
bool dFSHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path);
void retracePath(Vector<Vertex*>& path, Vertex*& current);
void visitBFSNeighbors(BasicGraph& graph, Vertex*& current, PriorityQueue<Vertex*>& pq);
void setVertexToInfinity(BasicGraph& graph);
void updateUnvisitedNeighbor(BasicGraph& graph, Vertex*& neighbor, Vertex* v, PriorityQueue<Vertex*>& pq, double heuristic);
void createEdgePQ(BasicGraph& graph, PriorityQueue<Edge*>& pq);
void initializeMap(BasicGraph& graph, Map<Vertex*, Set<Vertex*>* >& clusterMap);
void mergeClusters(Vertex* start, Vertex* end, Map<Vertex*,Set<Vertex*>* >& clusterMap, Set<Edge*>& mst, int& numClusters, Edge* curEdge);

/*
 * Implements the depth first search algorithm.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    graph.resetData();
    dFSHelper(graph, start, end, path);
    return path;
}

/*
 * Recursive helper for Depth First Search.
 */
bool dFSHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    path.add(start);
    start->setColor(GREEN);
    start->visited = true;
    if(start == end) return true;
    for (Vertex* next : graph.getNeighbors(start)) {
        if (!next->visited && dFSHelper(graph, next, end, path)) {
            return true;
        }
    }
    Vertex* deadEnd = path.get(path.size() - 1);
    deadEnd->setColor(GRAY);
    path.remove(path.size() - 1);
    return false;
}

/*
 * Implements the breadth first search algorithm.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    graph.resetData();
    PriorityQueue<Vertex*> pq;

    //Marks start vertex as visited and enqueues it into the priority queue
    pq.enqueue(start, start->cost);
    start->visited = true;
    start->setColor(YELLOW);

    while(!pq.isEmpty()) {
        Vertex* current = pq.dequeue();
        current->setColor(GREEN);
        if (current == end) {
            retracePath(path, current);  //Reconstructs by following previous back to V1
            break;
        }
        visitBFSNeighbors(graph, current, pq); //Check all neighbors
    }
    return path;
}

/*
 * Visits all neighbors of current. If a neighbor has not been visited before, this method will mark that neighbor
 * as visited and enqueue it back into the queue.
 */
void visitBFSNeighbors(BasicGraph& graph, Vertex*& current, PriorityQueue<Vertex*>& pq) {
    for(Vertex* next: graph.getNeighbors(current)) {
        if (!next->visited) {
            next->visited = true;
            next->previous = current;
            pq.enqueue(next, next->cost);
            next->setColor(YELLOW);
        }
    }
}

/*
 * Adds all the previous vertices stored in each Vertex* into the path vector. To add them in the correct order,
 * the function uses a tempStack to first store the vertices and then add them in order to the vector.
 */
void retracePath(Vector<Vertex*>& path, Vertex*& current) {
    Stack<Vertex*> tempStack;
    tempStack.push(current);
    while (current->previous != NULL) {
        tempStack.push(current->previous);
        current = current->previous;
    }
    while(!tempStack.isEmpty()) {
        path.add(tempStack.pop());
    }
}

/*
 * Implements Dijkstra's Algorithm.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pq;
    graph.resetData();
    setVertexToInfinity(graph);
    start->cost = 0;
    pq.enqueue(start, start->cost);
    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        v->visited = true;
        v->setColor(GREEN);

        if(v == end) break;

        for (Vertex* neighbor : graph.getNeighbors(v)) {
            if (!neighbor->visited) {
                updateUnvisitedNeighbor(graph, neighbor, v, pq, 0);
            }
        }
    }
    retracePath(path, end);
    return path;
}

/*
 * Updates an unvisited neighbor in Dijkstra and A* Algorithm.
 * The neighbor vertex that is passed into this function is assumed to be an unvisited neighbor.
 * The heuristic value is 0 if it is being called by Dijkstra's Algorithm.
 */
void updateUnvisitedNeighbor(BasicGraph& graph, Vertex*& neighbor, Vertex* v, PriorityQueue<Vertex*>& pq, double heur) {
    Edge* edge = graph.getEdge(v, neighbor);
    double cost = v->cost + edge->cost;
    if (cost < neighbor->cost) {
        neighbor->cost = cost;
        neighbor->previous = v;

        if (neighbor->visited) { //already in pq
            pq.changePriority(neighbor, cost + heur);
        } else {
            pq.enqueue(neighbor, cost + heur);
            neighbor->setColor(YELLOW);
        }
    }
}

/*
 * Initialize all vertices to have a cost of infinity in Dijkstra's Algorithm and the A* Algorithm.
 */
void setVertexToInfinity(BasicGraph& graph) {
    for (Vertex* vert : graph.getVertexSet()) {
        vert->cost = POSITIVE_INFINITY;
    }
}

/*
 * Implements the A* Algorithm.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pq;
    graph.resetData();
    setVertexToInfinity(graph);
    start->cost = 0;
    pq.enqueue(start, heuristicFunction(start, end));
    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        v->visited = true;
        v->setColor(GREEN);
        if(v == end) break;
        for (Vertex* neighbor : graph.getNeighbors(v)) {
            if (!neighbor->visited) {
                updateUnvisitedNeighbor(graph, neighbor, v, pq, heuristicFunction(neighbor, end));
            }
        }
    }
    retracePath(path, end);
    return path;
}

/*
 * Initializes the priority queue in Kruskal's Algorithm to contain all the edges and their costs as the priority value.
 */
void createEdgePQ(BasicGraph& graph, PriorityQueue<Edge*>& pq) {
    for (Edge* e : graph.getEdgeSet()) {
        pq.enqueue(e, e->cost);
    }
}

/*
 * Initialized all vertices in the map to point to a set that only contains that vertex.
 */
void initializeMap(BasicGraph& graph, Map<Vertex*, Set<Vertex*>* >& clusterMap) {
    for (Vertex* v : graph.getVertexSet()) {
        Set<Vertex*> *tempSet = new Set<Vertex*>();
        tempSet->add(v);
        clusterMap[v] = tempSet;
    }

}

/*
 * Merges two clusters given the clusterMap. Adds the set that endPointer points to to the set that startPointer points to.
 * Then changes all of the vertices in endPointer's set to point to startPointer's set. Finally, it adds the edge to the mst set and
 * then subtracts one from numClusters.
 */
void mergeClusters(Vertex* start, Vertex* end, Map<Vertex*, Set<Vertex*>* >& clusterMap, Set<Edge*>& mst, int& numClusters, Edge* curEdge) {
    Set<Vertex*>* startPointer = clusterMap[start];
    Set<Vertex*>* endPointer =  clusterMap[end];
    Set<Vertex*>* trash = endPointer;
    *startPointer += *endPointer;
    for (Vertex* v : *endPointer) {
        clusterMap[v] = startPointer;
    }
    delete trash;
    mst.add(curEdge);
    numClusters--;
}

/*
 * Implements Kruskal's Algorithm by returning a set of edges (instead of removing edges).
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    PriorityQueue<Edge*> pq;
    Map<Vertex*, Set<Vertex*>* > clusterMap;
    createEdgePQ(graph, pq);
    int numClusters = graph.getVertexSet().size();
    initializeMap(graph, clusterMap);
    while (numClusters >= 2) {
        Edge* curEdge = pq.dequeue();
        Vertex* start = curEdge->start;
        Vertex* end = curEdge->finish;
        if(clusterMap[start] != clusterMap[end]) {
            mergeClusters(start, end, clusterMap, mst, numClusters, curEdge);
        }
    }
    return mst;
}

