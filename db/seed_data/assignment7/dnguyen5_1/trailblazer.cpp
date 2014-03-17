// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "stack.h"
#include "pqueue.h"
#include "map.h"
#include "set.h"
using namespace std;

//This method calls a recursive helper function to run a DFS (explores a single path). It returns a path
//from the start and end parameters.
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData(); //resets map/terrain from any previous method calls.
    Vector<Vertex*> path;
    if (depthFirstSearchHelper(graph, start, end, path) == true) {
        return path;
    }
    else {
        return path; //returns empty path
    }
}

//Boolean recursive helper function. Takes in an additional Vector<Vertex*> parameter by reference: path.
bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    start->visited = true;
    start->setColor(GREEN);
    path.add(start);
    if (start == end) {
        return true; //base case
    }
    for(Edge* e : start->edges) {
        if (e->finish->visited == false) {
            if (depthFirstSearchHelper(graph, e->finish, end, path) == true) {
                return true;
            }
        }
    }
    start->setColor(GRAY);
    path.remove(path.size()-1);
    return false;
}

//Runs a BFS (explores many paths at once) from parameters start to end.
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData(); //resets map/terrain from any previous method calls.
    Queue<Vertex*> verticesQueue; //keeps track o
    verticesQueue.enqueue(start);
    start->setColor(YELLOW);
    start->visited = true;
    Stack<Vertex*> pathStack;
    Vector<Vertex*> path;
    while (!verticesQueue.isEmpty()) {
        Vertex* current = verticesQueue.dequeue();
        current->setColor(GREEN);
        if (current == end) {
            createPathVector(current, path);
            break;
        }
                for(Edge* edge : current->edges) {
                    if (edge->finish->visited == false) {
                        current->setColor(GREEN);
                        Vertex* finish = edge->finish;
                        finish->previous = current;
                        finish->visited = true;
                        finish->setColor(YELLOW);
                        verticesQueue.enqueue(finish);
                    }
                }
        current->setColor(GRAY);
    }
    return path;
}

//Finds the minimum-weight path between a
//pair of vertices (start and end, passed in as parameters) in a weighted directed graph.
//Returns the path as a Vector of Vertex pointers.
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Set<Vertex*> vertexes = graph.getNodeSet();
    for (Vertex* v : vertexes) {
        v->cost = POSITIVE_INFINITY; //initializes every node to have cost infinity
    }
    start->cost = 0; //sets first vertex's cost to 0
    start->setColor(YELLOW);
    PriorityQueue<Vertex*> pq;
    pq.enqueue(start, start->cost); //enqueues first vertex with its priority (0)
    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue(); //dequeues most urgent element
        v->setColor(GREEN);
        v->visited = true; //marks v as visited
        if (v == end) { //v is the final destination
            createPathVector(v, path);
            break;
        }
        for(Edge* edge : v->edges) {
            Vertex* nextVertex = edge->finish;
            if (nextVertex->visited == false) { //this for... if... combination checks each unvisited neighbor
                int cost = v->cost + edge->cost;
                if (cost < nextVertex->cost) {
                    nextVertex->cost = cost;
                    nextVertex->previous = v;
                    if (nextVertex->getColor() == YELLOW) {
                        pq.changePriority(nextVertex, nextVertex->cost);
                    }
                    else {
                        pq.enqueue(nextVertex, nextVertex->cost);
                        nextVertex->setColor(YELLOW);
                    }
                }
            }
        }
    }
    return path;
}

//Very similar to Dijkstra's algorithm, but is modified to use a heuristic function to guide its order of
//path exploration, making it more efficient.
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Set<Vertex*> vertexes = graph.getNodeSet();
    for (Vertex* v : vertexes) {
        v->cost = POSITIVE_INFINITY; //initalizes every vertex to have a cost of infinity.
    }
    start->cost = 0;
    start->setColor(YELLOW);
    PriorityQueue<Vertex*> pq;
    pq.enqueue(start, heuristicFunction(start, end));
    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        v->setColor(GREEN);
        v->visited = true;
        if (v == end) { //This if statement represents a successful path found.
            createPathVector(v, path);
            break;
        }
        for(Edge* edge : v->edges) {
            Vertex* nextVertex = edge->finish;
            if (nextVertex->visited == false) {
                int cost = v->cost + edge->cost;
                if (cost < nextVertex->cost) {
                    nextVertex->cost = cost;
                    nextVertex->previous = v;
                    if (nextVertex->getColor() == YELLOW) {
                        pq.changePriority(nextVertex, nextVertex->cost +heuristicFunction(nextVertex, end));
                    }
                    else {
                        pq.enqueue(nextVertex, nextVertex->cost + heuristicFunction(nextVertex, end));
                        nextVertex->setColor(YELLOW);
                    }
                }
            }
        }
    }
    return path;
}

//Creates a minimum cost spanning tree that in this case is used to create
//a maze.
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst; //Initializes an empty set that will later store the edges in the maze
    Map<Vertex*, Set<Vertex*> > clusterMap; //Initializes a map that will store every vertex as a key, and the contents
                                            //of a vertex's cluster within the value (A set of vertexes).
    Set<Vertex*> vertexes = graph.getNodeSet(); //retrieves a set of all vertexes in the graph.
    for (Vertex* v : vertexes) { //places every vertex as a key within the map and adds the vertex to the cluster
        Set<Vertex*> tempSet;
        tempSet += v;
        clusterMap.put(v, tempSet);
    }
    PriorityQueue<Edge*> pq; //initializes a priority queue of edges.
    for (Edge* edge : graph.getEdgeSet()) { //stores all edges in the graph within the priority queue.
        pq.enqueue(edge, edge->cost);
    }
    while (!pq.isEmpty()) {
        Edge* e = pq.dequeue();
        if (clusterMap[e->finish] != clusterMap[e->start]) { //if the vertices of e are not in the same cluster
            mst.add(e);
            clusterMap[e->finish] += clusterMap[e->start]; //merges clusters
            for(Vertex* v : clusterMap[e->finish]) {
                clusterMap[v] = clusterMap[e->finish];
            }
        }
    }
    return mst;
}

//Follows previous pointers to reconstruct path. Pointers are initially placed in a stack to preserve
//original order.
void createPathVector(Vertex* pathVertex, Vector<Vertex*>& path) {
    Stack<Vertex*> pathStack;
    while(pathVertex != NULL) {
        pathStack.push(pathVertex);
        pathVertex = pathVertex->previous;
    }
    while (!pathStack.isEmpty()) {
        path.add(pathStack.pop());
    }
}
