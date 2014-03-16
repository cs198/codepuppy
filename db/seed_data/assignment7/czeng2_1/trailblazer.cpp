/* Connie Zeng
 * CS 106B, Winter 2014
 * -------------------------
 * Assignmet 7: Trailblazer
 * This program loads two-dimensional worlds (maze or terrain) and finds a path
 * between two points using DFS, BFS, Dijkstra's algorithm, or A* search. Mazes
 * can also be generated using Kruskal's algorithm.
 */

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"
#include "map.h"
// TODO: include any other headers you need; remove this comment
using namespace std;

/* Function prototypes */

bool DFS(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path);
void BFS(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path);
void BFSvisit(Queue<Vertex*>& visited, Vertex* v);
void DA(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path, bool heuristic = false);
void DAvisit(PriorityQueue<Vertex*>& visited, Vertex* v, Vertex* end, double priority, bool heuristic);
void construct(Vertex* curr, Vector<Vertex*>& path);
void storeGraph(BasicGraph& graph, Map<Vertex*, Set<Vertex*>* >& clusterMap, PriorityQueue<Edge*>& edgeQueue);
void merge(Set<Vertex*>*& start, Set<Vertex*>*& finish, Map<Vertex*, Set<Vertex*>* >& clusterMap);

/* Function: depthFirstSearch
 * Usage: depthFirstSearch(graph, start, end);
 * --------------------------------------------
 * This function performs a depth-first serach for a path from the start to the
 * end in the passed graph, and returns the path. If no path is found, an
 * empty path is returned. This method is guaranteed to find a path, if it
 * exists, but it may not be the shortest path.
 */

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    graph.resetData();
    DFS(graph, start, end, path);
    return path;
}

/* Function: breadthFirstSearch
 * Usage: breadthFirstSearch(graph, start, end);
 * ----------------------------------------------
 * This function performs a breadth-first search for a path from the start to
 * the end in the passed graph, and returns the path. If no path is found, an
 * empty path is returned. This method will find the shortest path, if a path
 * exists, but requires more memory than a depth-first search.
 */

Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    graph.resetData();
    BFS(graph, start, end, path);
    return path;
}

/* Function: dijkstrasAlgorithm
 * Usage: dijkstrasAlgorithm(graph, start, end);
 * ----------------------------------------------
 * This function searches for a path from the start to the end in the passed
 * graph, using Dijkstra's algorithm, and returns the path. If no path is found,
 * an empty path is returned. This method will find the minimum-cost path, if
 * a path exists; thus, it will find the same path as breadth-first search in
 * a maze, where all edges have the same cost, but may find a different path
 * on terrain, where edges have different costs depending on elevation.
 */

Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    graph.resetData();
    DA(graph, start, end, path);
    return path;
}

/* Function: aStar
 * Usage: aStar(graph, start, end);
 * ---------------------------------
 * This function searches for a path from the start to the end in the passed
 * graph, using the A* algorithm, and returns the path. If no path is found,
 * an empty path is returned. This method is similar to Dijkstra's algorithm,
 * but uses an additional heuristic function to focus the search in directions
 * with higher likelihood of finding the desired path.
 */

Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    graph.resetData();
    DA(graph, start, end, path, true);
    return path;
}

/* Function: kruskal
 * Usage: kruskal(graph);
 * -----------------------
 * This function generates and returns a random minimum spanning tree for the
 * passed graph, using Kruskal's algorithm. The tree can then be used to create
 * a random maze.
 */

Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    Map<Vertex*, Set<Vertex*>* > clusterMap;    // maps vertex to cluster
    PriorityQueue<Edge*> edgeQueue;             // orders edges in priority of cost
    storeGraph(graph, clusterMap, edgeQueue);

    // Adds one edge at a time to MST, and combines endpoint vertex clusters
    while(!edgeQueue.isEmpty()){
        Edge* curr = edgeQueue.dequeue();
        Set<Vertex*>* start = clusterMap[curr->start];
        Set<Vertex*>* finish = clusterMap[curr->finish];
        if(start != finish){
            merge(start, finish, clusterMap);
            mst.add(curr);
        }
    }
    return mst;
}

/* Function: DFS
 * Usage: DFS(graph, start, end, path);
 * -------------------------------------
 * This is a helper function for depthFirstSearch. It explores each possible
 * path as far possible by recursively exploring one neighbor at a time,
 * starting at the start vertex, and continuing to the neighbor's neighbor. The
 * function returns whether the end vertex can be reached from the current vertex.
 * The path parameter keeps track of the vertices added to the growing path.
 */

bool DFS(BasicGraph& graph, Vertex* curr, Vertex* end, Vector<Vertex*>& path) {
    // Adds current vertex to path and marks as visited
    path += curr;
    curr->setColor(GREEN);
    curr->visited = true;

    // End vertex reached, path found
    if(curr == end){
        return true;
    }

    // Explores unvisited neighbor vertices to see if end vertex can be reached
    Set<Vertex*> neighbors = graph.getNeighbors(curr);
    for(Vertex* next : neighbors){
        if(!(next->visited) && DFS(graph, next, end, path)){
            return true;
        }
    }

    // Removes current vertex from path if end vertex cannot be reached
    curr->setColor(GRAY);
    path.remove(path.size() - 1);
    return false;
}

/* Function: BFS
 * Usage: BFS(graph, start, end, path);
 * -------------------------------------
 * This is a helper function for breadthFirstSearch. It starts at the start
 * vertex and simultaneously explores every neighbor of the current vertex
 * until the end vertex is reached. Pointers keep track of the vertices added
 * to each growing path, and the completed path is constructed at the end.
 */

void BFS(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    // Marks start vertex to be visited
    Queue<Vertex*> visited;
    BFSvisit(visited, start);

    while(!visited.isEmpty()){
        // Actively visits current vertex
        Vertex* curr = visited.dequeue();
        curr->setColor(GREEN);

        // End vertex reached, path found
        if(curr == end){
            construct(curr, path);
            break;
        }

        // Explores all unvisited neighbor vertices
        Set<Vertex*> neighbors = graph.getNeighbors(curr);
        for(Vertex* next : neighbors){
            if(!(next->visited)){
                BFSvisit(visited, next);
                next->previous = curr;
            }
        }
    }
}

/* Function: BFSvisit
 * Usage: BFSvisit(visited, v);
 * -----------------------------
 * This function marks the passed vertex as visited and enqueues it to be
 * visited, along with the other vertices in the same neighborhood.
 */

void BFSvisit(Queue<Vertex*>& visited, Vertex* v) {
    visited.enqueue(v);
    v->setColor(YELLOW);
    v->visited = true;
}

/* Function: DA
 * Usage: DA(graph, start, end, path);
 *        DA(graph, start, end, path, true);
 * ------------------------------------------
 * This is a helper function for dijkstrasAlgorithm and aStar. The two
 * algorithms can be distinguished by passing the last optional boolean as true
 * to indicate the use of a heuristic in A*. It starts at the start vertex and
 * simultaneously explores every neighbor of the current vertex whose cost will
 * decrease if visited from the current vertex. Once the end vertex is reached,
 * the function keeps searching until the minimum-cost path to the vertex is
 * found. Pointers keep track of the vertices added to each growing path, and
 * the completed path is constructed at the end.
 */
void DA(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path, bool heuristic) {
    // Sets cost of all vertices to infinity
    Set<Vertex*> vertices = graph.getVertexSet();
    for(Vertex* v : vertices){
        v->cost = POSITIVE_INFINITY;
    }

    // Marks start vertex to be visited, with initial cost of zero
    PriorityQueue<Vertex*> visited;
    DAvisit(visited, start, end, 0, heuristic);

    while(!visited.isEmpty()){
        // Actively visits current vertex and marks as visited
        Vertex* curr = visited.dequeue();
        curr->setColor(GREEN);
        curr->visited = true;

        // End vertex reached, path found
        if(curr == end){
            construct(curr, path);
            break;
        }

        // Explores all unvisited neighbor vertices whose cost will decrease
        Set<Vertex*> neighbors = graph.getNeighbors(curr);
        for(Vertex* next : neighbors){
            if(!(next->visited)){
                double cost = curr->cost + graph.getEdge(curr, next)->cost;
                if(cost < next->cost){
                    DAvisit(visited, next, end, cost, heuristic);
                    next->previous = curr;
                }
            }
        }
    }
}

/* Function: DAvisit
 * Usage: DAvisit(vistied, v, end, priority, heuristic);
 * ------------------------------------------------------
 * This function enqueues the passed vertex to be visited and updates its cost.
 */

void DAvisit(PriorityQueue<Vertex*>& visited, Vertex* v, Vertex* end, double priority, bool heuristic) {
    v->cost = priority;
    if(heuristic){
        priority += heuristicFunction(v, end);
    }
    if(v->getColor() == YELLOW){
        visited.changePriority(v, priority);
    }
    else{
        visited.enqueue(v, priority);
        v->setColor(YELLOW);
    }
}

/* Function: construct
 * Usage: construct(curr, path);
 * ------------------------------
 * This function constructs the path by traversing the sequence of previous
 * pointers from BFS and DA.
 */

void construct(Vertex* curr, Vector<Vertex*>& path) {
    while(curr != NULL){
        path.insert(0, curr);
        curr = curr->previous;
    }
}

/* Function: storeGraph
 * Usage: storeGraph(graph, clusterMap, edgeQueue);
 * -------------------------------------------------
 * This function adds each vertex to its own cluster, and stores the edges
 * in priority of cost.
 */

void storeGraph(BasicGraph& graph, Map<Vertex*, Set<Vertex*>* >& clusterMap, PriorityQueue<Edge*>& edgeQueue) {
    Set<Vertex*> vertices = graph.getVertexSet();
    Set<Edge*> edges = graph.getEdgeSet();
    for(Vertex* v : vertices){
        Set<Vertex*>* cluster = new Set<Vertex*>;
        cluster->add(v);
        clusterMap[v] = cluster;
    }
    for(Edge* e : edges){
        edgeQueue.enqueue(e, e->cost);
    }
}

/* Function: merge
 * Usage: merge(start, finish, clusterMap);
 * -----------------------------------------
 * This function combines the clusters that the endpoint vertices belong to.
 * All vertices that were originally in either cluster are now part of the
 * same combined cluster.
 */

void merge(Set<Vertex*>*& start, Set<Vertex*>*& finish, Map<Vertex*, Set<Vertex*>* >& clusterMap) {
    *start += *finish;
    Set<Vertex*>* trash = finish;
    for(Vertex* member : *finish){
        clusterMap[member] = start;
    }
    delete trash;
}



