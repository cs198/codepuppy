/**
 * Name: Milan Doshi
 * Assignment: trailblazer.cpp
 * TA: Jimmy Lee
 *
 * This file implements four different ways to traverse through a graph (DFS, BFS, Dijkstra's, and A*) and also
 * implements Kruskal's algortihm to generate random mazes.
 */

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"
using namespace std;

/**
 * method: depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end)
 * return: Vector<Vertex*>
 * This method calls on a helper method, which it passes an empty path, that conducts the depth first search
 * on either the maze or terrain chosen.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    depthFirstSearchHelper(graph, start, end, path);
    return path;
}

/**
 * method: depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path)
 * return: bool
 * This method is the helper method for the recursive DFS and returns true or false based on whether or not
 * a path is found.
 */
bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    path.add(start);
    start->visited = true;
    start->setColor(GREEN);
    if(start == end) return true;
    for(Vertex* neighbor : graph.getNeighbors(start)) {
        if(neighbor->visited == false) {
            neighbor->visited = true;
            if(depthFirstSearchHelper(graph, neighbor, end, path) == true) return true;
        }
    }
    path[path.size()-1]->setColor(GRAY);
    path.remove(path.size()-1);
    return false;
}

/**
 * method: breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end)
 * return: Vector<Vertex*>
 * This method conducts the breadth first search on the maze or terrain chosen and then goes back through
 * and traces the path.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> queue;
    queue.enqueue(start);
    start->setColor(YELLOW);
    start->visited = true;
    while(!queue.isEmpty()) {
        Vertex* v = queue.dequeue();
        v->setColor(GREEN);
        if(v == end) break;
        for(Vertex* neighbor : graph.getNeighbors(v)) {
            if(neighbor->visited == false) {
                neighbor->visited = true;
                neighbor->previous = v;
                queue.enqueue(neighbor);
                neighbor->setColor(YELLOW);
            }
        }
    }
    path.add(end);
    while(end->previous != NULL) {
        path.add(end->previous);
        end = end->previous;
    }
    return path;
}

/**
 * method: dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end)
 * return: Vector<Vertex*>
 * This method, using dijkstras's algorithm conducts finds a path in either the maze or terrain chosen and then goes back through
 * and traces the path.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Set<Vertex*> vertexes = graph.getVertexSet();
    for(Vertex* vertex : vertexes) {
        vertex->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    PriorityQueue<Vertex *> queue;
    Set<Vertex *> queueCopy;
    queue.enqueue(start, 0);
    start->setColor(YELLOW);
    queueCopy.add(start);
    while(!queue.isEmpty()) {
        Vertex* v = queue.dequeue();
        v->setColor(GREEN);
        v->visited = true;
        if(v == end) break;
        for(Vertex *neighbor : graph.getNeighbors(v)) {
            double cost = v->cost + graph.getEdge(neighbor,v)->cost;
            if(cost < neighbor->cost) {
                neighbor->cost = cost;
                neighbor->previous = v;
                if(queueCopy.contains(neighbor)) {
                    queue.changePriority(neighbor,cost);
                } else {
                    queue.enqueue(neighbor, cost);
                    neighbor->setColor(YELLOW);
                }
            }
        }
    }
    path.add(end);
    while(end->previous != NULL) {
        path.add(end->previous);
        end = end->previous;
    }
    return path;
}

/**
 * method: aStar(BasicGraph& graph, Vertex* start, Vertex* end)
 * return: Vector<Vertex*>
 * This method, using the A star algorithm, conducts finds a path in either the maze or terrain chosen and then goes back through
 * and traces the path.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Set<Vertex*> vertexes = graph.getVertexSet();
    for(Vertex* vertex : vertexes) {
        vertex->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    PriorityQueue<Vertex *> queue;
    Set<Vertex *> queueCopy;
    queue.enqueue(start, heuristicFunction(start,end));
    start->setColor(YELLOW);
    queueCopy.add(start);
    while(!queue.isEmpty()) {
        Vertex* v = queue.dequeue();
        v->setColor(GREEN);
        v->visited = true;
        if(v == end) break;
        for(Vertex *neighbor : graph.getNeighbors(v)) {
            double cost = v->cost + graph.getEdge(neighbor,v)->cost;
            if(cost < neighbor->cost) {
                neighbor->cost = cost;
                neighbor->previous = v;
                if(queueCopy.contains(neighbor)) {
                    queue.changePriority(neighbor,cost + heuristicFunction(neighbor,end));
                } else {
                    queue.enqueue(neighbor, cost + heuristicFunction(neighbor,end));
                    neighbor->setColor(YELLOW);
                }
            }
        }
    }
    path.add(end);
    while(end->previous != NULL) {
        path.add(end->previous);
        end = end->previous;
    }
    return path;
}

/**
 * method: kruskal(BasicGraph& graph)
 * return: Set<Edge*>
 * This method, using Kruskal's algorithm, generates a random maze.
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    graph.resetData();
    Set<Edge*> mst;
    Vector<Set<Vertex*> > clusters;
    for(Vertex* vertex: graph.getVertexSet()) {
        Set<Vertex*> cluster;
        cluster.add(vertex);
        clusters.add(cluster);
    }
    PriorityQueue<Edge*> edges;
    for(Edge* edge: graph.getEdgeSet()) {
        edges.enqueue(edge,edge->cost);
    }
    while(clusters.size() >= 2) {
        Edge* e = edges.dequeue();
        Set<Vertex*> startCluster;
        Set<Vertex*> finishCluster;
        int start, finish;
        for(int i = 0; i < clusters.size(); i++){
            Set<Vertex*> current = clusters[i];
            if(current.contains(e->start)) {
                startCluster = current;
                start = i;
                if(startCluster.contains(e->finish)) {
                    finishCluster = startCluster;
                    break;
                }
            } if(current.contains(e->finish)) {
                finishCluster = current;
                finish = i;
                if(finishCluster.contains(e->start)) {
                    startCluster = finishCluster;
                    break;
                }
            }
        }
        if(startCluster != finishCluster) {
            if(start < finish) {
                clusters.remove(finish);
                clusters.remove(start);
            } else {
                clusters.remove(start);
                clusters.remove(finish);
            }
            Set<Vertex*> merged = startCluster + finishCluster;
            clusters.add(merged);
            mst.add(e);
        }
    }
    return mst;
}
