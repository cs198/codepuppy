// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "basicgraph.h"
#include "types.h"
#include "pqueue.h"
#include "console.h"
using namespace std;

bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path);

// This function calls the helper function to do the depth first search and return the path.
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    depthFirstSearchHelper(graph, start, end, path);
    return path;
}

// This is the helper function for depth first search.
// It uses recursive backtracking to explore a single path all the way.
// The path is passed by reference.
bool depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    path.add(start);
    start->visited = true;
    start->setColor(GREEN);
    if (start == end) {
        return true;
    } else {
        Set<Vertex*> neighborSet = graph.getNeighbors(start);
        for (Vertex* neighbor : neighborSet) {
            if (!neighbor->visited) {
                if (depthFirstSearchHelper(graph, neighbor, end, path)) {
                    return true;
                }
            }
        }
        for (int i = 0; i < path.size(); i++) {
            if (path[i] == start) {
                path.remove(i);
                start->setColor(GRAY);
                break;
            }
        }
        return false;
    }
}

// This function uses breadth first search method to find the path.
// It iteratively explores many paths at once.
// and the final path is returned.
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> q;
    q.enqueue(start);
    start->visited = true;
    while (!q.isEmpty()) {
        Vertex* v = q.dequeue();
        v->setColor(GREEN);
        if (v == end) {
            path.add(v);
            while (v->previous != NULL) {
                path.insert(0, v->previous);
                v = v->previous;
            }
            return path;
        }
        Set<Vertex*> neighborSet = graph.getNeighbors(v);
        for (Vertex* neighbor : neighborSet) {
            if (neighbor->visited == false) {
                neighbor->visited = true;
                neighbor->previous = v;
                q.enqueue(neighbor);
                neighbor->setColor(YELLOW);
            }
        }
    }
    return path;
}

// This function used the Dijkstra's method to find the path.
// It is a flavor of BFS, and it iteratively uses a priority queue to store the vertex.
// The path is returned.
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pq;
    Set<Vertex*> vertexInPq;

    Set<Vertex*> vertexSet = graph.getVertexSet();
    for (Vertex* v : vertexSet) {
        v->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    pq.enqueue(start, start->cost);
    vertexInPq.add(start);

    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        v->setColor(GREEN);
        vertexInPq.remove(v);
        v->visited = true;
        if (v == end) {
            path.add(v);
            while (v->previous != NULL) {
                path.insert(0, v->previous);
                v = v->previous;
            }
            return path;
        }
        Set<Vertex*> neighborSet = graph.getNeighbors(v);
        for (Vertex* neighbor : neighborSet) {
            if (neighbor->visited == false) {
                Edge* vToNeighbor = graph.getEdge(v, neighbor);
                double cost = v->cost + vToNeighbor->cost;
                if (cost < neighbor->cost) {
                    neighbor->cost = cost;
                    neighbor->previous = v;
                    if (vertexInPq.contains(neighbor)) {
                        pq.changePriority(neighbor, cost);
                    } else {
                        pq.enqueue(neighbor, cost);
                        neighbor->setColor(YELLOW);
                        vertexInPq.add(neighbor);
                    }
                }
            }
        }
    }
    return path;
}

// This function uses the A* method to find the path.
// The method is very similar to Dijkstra's method.
// Only difference is that it uses a heuristirc function to make prediction, which is add to the weight.
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pq;
    Set<Vertex*> vertexInPq;

    Set<Vertex*> vertexSet = graph.getVertexSet();
    for (Vertex* v : vertexSet) {
        v->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    pq.enqueue(start, + heuristicFunction(start, end));
    vertexInPq.add(start);

    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        v->setColor(GREEN);
        vertexInPq.remove(v);
        v->visited = true;
        if (v == end) {
            path.add(v);
            while (v->previous != NULL) {
                path.insert(0, v->previous);
                v = v->previous;
            }
            return path;
        }
        Set<Vertex*> neighborSet = graph.getNeighbors(v);
        for (Vertex* neighbor : neighborSet) {
            if (neighbor->visited == false) {
                Edge* vToNeighbor = graph.getEdge(v, neighbor);
                double cost = v->cost + vToNeighbor->cost;
                if (cost < neighbor->cost) {
                    neighbor->cost = cost;
                    neighbor->previous = v;
                    if (vertexInPq.contains(neighbor)) {
                        pq.changePriority(neighbor, cost + heuristicFunction(neighbor, end));
                    } else {
                        pq.enqueue(neighbor, cost + heuristicFunction(neighbor, end));
                        neighbor->setColor(YELLOW);
                        vertexInPq.add(neighbor);
                    }
                }
            }
        }
    }
    return path;
    /*
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pq;
    Map<Vertex*, double> vertexInPq;

    Set<Vertex*> vertexSet = graph.getVertexSet();
    for (Vertex* v : vertexSet) {
        v->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    pq.enqueue(start, heuristicFunction(start, end));
    vertexInPq[start] = heuristicFunction(start, end);

    while (!pq.isEmpty()) {
        Vertex* v = pq.dequeue();
        v->setColor(GREEN);
        vertexInPq.remove(v);
        v->visited = true;
        if (v == end) {
            path.add(v);
            while (v->previous != NULL) {
                path.insert(0, v->previous);
                v = v->previous;
            }
            return path;
        }
        Set<Vertex*> neighborSet = graph.getNeighbors(v);
        for (Vertex* neighbor : neighborSet) {
            if (neighbor->visited == false) {
                Edge* vToNeighbor = graph.getEdge(v, neighbor);
                double cost = v->cost + vToNeighbor->cost;
                if (cost < neighbor->cost) {
                    neighbor->cost = cost;
                    neighbor->previous = v;
                    if (vertexInPq.containsKey(neighbor)) {
                        //if (vertexInPq[neighbor] > (cost + heuristicFunction(neighbor, end))) {
                            pq.changePriority(neighbor, cost + heuristicFunction(neighbor, end));
                            vertexInPq[neighbor] = cost + heuristicFunction(end, neighbor);
                        //}
                    } else {
                        pq.enqueue(neighbor, cost + heuristicFunction(end, neighbor));
                        neighbor->setColor(YELLOW);
                        vertexInPq[neighbor] = cost + heuristicFunction(end, neighbor);
                    }
                }
            }
        }
    }
    return path;
    */
}

// This function returns a set a edges of a spanning tree to be used in geneartion of a random maze.
// It creates a minimum cost spanning tree.
// It iteratively chooses edges untils all the edges chosen connect all nodes in the graph.
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    Set<Edge*> allEdges = graph.getEdgeSet();
    Set<Vertex*> allVertex = graph.getVertexSet();
    Map<Vertex*, Set<Vertex*>* > clusters;
    for (Vertex* v : allVertex) {
        Set<Vertex*>* vertexSetP = new Set<Vertex*>();
        vertexSetP->add(v);
        clusters[v] = vertexSetP;
    }
    PriorityQueue<Edge*> pq;
    for (Edge* e : allEdges) {
        pq.enqueue(e, e->cost);
    }
    while (!pq.isEmpty()) {
        Edge* e = pq.dequeue();
        if (clusters[e->start] != clusters[e->finish]) {
            mst.add(e);
            Set<Vertex*>* vertexClusterP1 = clusters[e->start];
            Set<Vertex*>* vertexClusterP2 = clusters[e->finish];
            if (vertexClusterP1->size() > vertexClusterP2->size()) {
                for (Vertex* v : *vertexClusterP2) {
                    vertexClusterP1->add(v);
                }
                clusters[e->start] = vertexClusterP1;
                clusters[e->finish] = vertexClusterP1;
                for (Vertex* v : *vertexClusterP1) {
                    clusters[v] = vertexClusterP1;
                }
                vertexClusterP2->~Set();
            } else {
                for (Vertex* v : *vertexClusterP1) {
                    vertexClusterP2->add(v);
                }
                clusters[e->start] = vertexClusterP2;
                clusters[e->finish] = vertexClusterP2;
                for (Vertex* v : *vertexClusterP2) {
                    clusters[v] = vertexClusterP2;
                }
                vertexClusterP1->~Set();
            }
        }
    }
    return mst;
}
