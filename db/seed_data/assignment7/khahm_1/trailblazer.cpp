/*
 * Katie Hahm
 * Section Leader: Jimmy Lee
 * Trailblazer cpp file
 * This class finds the path length, path cost, and number of locations visited
 * from the given maze/terrain and search method.
 * DFS: searches for any possible path
 * BFS: searches for the shortest possible path
 * Dijkstra: searches for the path with the minimum cost
 * A*: searches for the path with the minimum cost with a guided path exploration direction
 * Kruskal: generates a random graph
 */

#include "costs.h"
#include "trailblazer.h"
#include "vector.h"
#include "basicgraph.h"
#include "queue.h"
#include "pqueue.h"
#include "set.h"

using namespace std;

/*
 * Helper function for depthFirstSearch
 * parameter: basic graph, current vertex, ending vertex, current path
 * adds the current vertex to the path, marks it as visited,
 * and if the current vertex is the ending vertex, return true.
 * If not, iterate through each unvisited neighbor and recurse.
 * If there are no neighbors left and this method is still running,
 * there is no valid path and returns false.
 */
bool dfsHelper(BasicGraph& graph, Vertex* v1, Vertex* end, Vector<Vertex*>& path) {
    v1->setColor(GREEN);
    path.add(v1);
    v1->visited = true;
    if (v1 == end) {
        return true;
    }
    for (Vertex* neighbor : graph.getNeighbors(v1)) {
        if (neighbor->visited != true) {
            neighbor->setColor(GREEN);
            if (dfsHelper(graph, neighbor, end, path)) {
                return true;
            }
        }
    }
    path.remove(path.size()-1);
    v1->setColor(GRAY);
    return false;
}

/*
 * Depth first search algorithm
 * parameter: basic graph, starting vertex, ending vertex
 * resets the current graph data,
 * Uses the helper method to find any possible path,
 * and returns it, whether it is empty or valid.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    dfsHelper(graph, start, end, path);
    return path;
}

/*
 * Helper method for BFS, Dijkstra's, and AStar
 * parameter: final Vertex v, path to be constructed
 * adds the final vertex to the empty path
 * while the vertex has a previous path, add the vertex to start of the path.
 * precondition: path is empty
 */
void constructPath(Vertex* v, Vector<Vertex*>& path) {
    path.add(v);
    while (v->previous != NULL) {
        path.insert(0,v->previous);
        v = v->previous;
    }
}

/*
 * Breadth first search algorithm
 * parameter: basic graph, starting vertex, ending vertex
 * resets the current graph data,
 * uses a queue to keep track of each possible path from current vertex
 * finds the shortest distance path from starting to ending vertex on the graph
 * returns such path
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> q;
    start->setColor(YELLOW);
    q.enqueue(start);
    start->visited = true;
    while (!q.isEmpty()) {
        Vertex* v = q.dequeue();
        v->setColor(GREEN);
        if (v == end) {
            constructPath(v, path);
            return path;
        }
        for (Vertex* neighbor : graph.getNeighbors(v)) {
            if (neighbor->visited != true) {
                neighbor->visited = true;
                neighbor->previous = v;
                q.enqueue(neighbor);
                neighbor->setColor(YELLOW);
            }
        }
    }
    return path;
}

/*
 * Helper function for perform weighted search (ultimately for Dijkstra's and A*)
 * parameter: basic graph, starting vertex, ending vertex, priority queue, boolean is a Star
 * Initializes the variables and sets up the algorithm for Dijkstra's and A*
 * Uses the boolean variable to determine if this method is being used for dijkstra's or aStar
 * resets the graph data,
 * sets the weights of all vertices to infinity
 * sets the starting weight to zero, and marks it as visited
 * enqueues the start vertex with the corresponding priority
 */
void initializeWeightedSearch(BasicGraph& graph, Vertex* start, Vertex* end, PriorityQueue<Vertex*>& pq, bool isAStar) {
    graph.resetData();
    for (Vertex* v : graph.getVertexSet()) {
        v->cost = POSITIVE_INFINITY;
        pq.enqueue(v, POSITIVE_INFINITY);
    }
    start->cost = 0.0;
    start->visited = true;
    start->setColor(YELLOW);
    if (isAStar) {
        pq.enqueue(start, heuristicFunction(start, end));
    }
    else {
        pq.enqueue(start, 0.0);
    }
}

/*
 * Helper method for perform weighted search (ultimately for Dijkstra's and AStar)
 * parameter: basic graph, current vertex, boolean is a star?, priority queue, ending vertex
 * Advances the weighted search by iterating through the neighbors
 * and enqueueing them with the correct priority
 */
void advanceWeightedSearch(BasicGraph& graph, Vertex* v, bool isAStar, PriorityQueue<Vertex*>& pq, Vertex* end) {
    for (Vertex* neighbor : graph.getNeighbors(v)) {
        Edge* e = graph.getEdge(v, neighbor);
        double newCost = v->cost + e->cost;
        if (newCost < neighbor->cost) {
            neighbor->cost = newCost;
            neighbor->previous = v;
            if (isAStar) {
                pq.changePriority(neighbor, newCost + heuristicFunction(neighbor, end));
            }
            else {
                pq.changePriority(neighbor, newCost);
            }
            neighbor->setColor(YELLOW);
        }
    }
}

/*
 * Helper function for Dijkstra's and AStar
 * parameter: starting vertex, ending vertex, graph, boolean is a star?
 * builds a priority queue and a path vector
 * initializes the method,
 * while priority queue is not empty, advance the weighted search
 * if the current vertex is the ending vertex, stop, and return a constructed path.
 * If we exit the while loop, without having returned a path yet,
 * return the empty path because we could not find a valid path.
 * Main framework for Dijkstra's and AStar,
 * the only difference is from boolean isAStar to determine which priority to enqueue vertex with
 */
Vector<Vertex*> performWeightedSearch(Vertex* start, Vertex* end, BasicGraph& graph, bool isAStar) {
    PriorityQueue<Vertex*> pq;
    Vector<Vertex*> path;
    initializeWeightedSearch(graph, start, end, pq, isAStar);
    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        v->setColor(GREEN);
        v->visited = true;
        if (v == end) {
            constructPath(v, path);
            return path;
        }
        advanceWeightedSearch(graph, v, isAStar, pq, end);
    }
    return path;
}

/*
 * Dijkstra's algorithm implementation
 * parameter: basic graph, starting vertex, ending vertex
 * uses helper methods to search for the path with the minimum cost
 * returns the path
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    bool isAStar = false;
    return performWeightedSearch(start, end, graph, isAStar);
}

/*
 * AStar algorithm implementation
 * parameter: basic graph, starting vertex, ending vertex
 * uses helper methods to search for the path with the minimum cost
 * with a guided path exploration direction
 * returns the path
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    bool isAStar = true;
    return performWeightedSearch(start, end, graph, isAStar);
}

/*
 * Helper method for is same cluster (ultimately for kruskal)
 * parameter: vector of sets of vertex, edge e, index1, index2
 * finds the index of the vector containing the set containing the starting vertex,
 * and finds the index of the vector containing the set containing the ending vertex
 * assigns those index values to index1 and index2 respectively
 */
void assignIndex(Vector<Set<Vertex*> >& cluster, Edge*& e, int& index1, int& index2) {
    Vertex* v1 = e->start;
    Vertex* v2 = e->finish;
    for (int i = 0; i < cluster.size(); i++) {
        Set<Vertex*> temp = cluster[i];
        if (temp.contains(v1)) {
            index1 = i;
        }
        if (temp.contains(v2)) {
            index2 = i;
        }
    }
}

/*
 * Helper method for Kruskal
 * parameter: vector of sets of vertex, edge, basic graph
 * finds the index of the sets containg the starting and ending vertex
 * if the vertex are in the same set, return true
 * combines the sets, removes the sets from the vector,
 * adds the combined set to the vector
 * return false because the vertex aren't in the same set
 */
bool inSameCluster(Vector<Set<Vertex*> >& cluster, Edge*& e) {
    int index1;
    int index2;
    assignIndex(cluster, e, index1, index2);
    if (index1 == index2) {
        return true;
    }
    else {
        Set<Vertex*> set1 = cluster[index1];
        Set<Vertex*> set2 = cluster[index2];
        if (index1 < index2) {
            cluster.remove(index2);
            cluster.remove(index1);
        }
        else {
            cluster.remove(index1);
            cluster.remove(index2);
        }
        set1 += set2;
        cluster.add(set1);
        return false;
    }
}

/*
 * Kruskal algorithm implementation
 * parameter: basic graph
 * initializes all vertex in the graph as its own set in the cluster
 * makes a priority queue of edges, their priority being their cost
 * while cluster has 2 or more elements,
 * dequeues an edge, and determines whether the start/end vertex are in the same cluster
 * if they aren't, add the edge to the returning set of edges.
 * By this, Kruskal calculates the edges necessary to make a valid maze without
 * all vertex being connected to each other.
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    Vector<Set<Vertex*> > cluster;
    for (Vertex* v : graph.getVertexSet()) {
        Set<Vertex*> temp;
        temp.add(v);
        cluster.add(temp);
    }
    PriorityQueue<Edge*> edges;
    for (Edge* e : graph.getEdgeSet()) {
        edges.enqueue(e, e->cost);
    }
    while (cluster.size() > 1) {
        Edge* e = edges.dequeue();
        if (!inSameCluster(cluster, e)) {
            mst.add(e);
        }
    }
    return mst;
}
