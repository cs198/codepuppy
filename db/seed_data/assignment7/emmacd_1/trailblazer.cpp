/*
 * CS106B Assignment #7: Trailblazer
 * Emma Daugherty
 * Section Leader: Jimmy Lee
 *
 * This program uses 4 graph searching algorithms: depth-first search, breadth-first search, Dijkstra's algorithm and
 * A* search, to generate paths in 2-D mazes or terrain maps. It also implements Kruskal's algorithm to aid in
 * generating random mazes.
 */

#include "costs.h"
#include "trailblazer.h"
#include "set.h"
#include "queue.h"
#include "pqueue.h"
#include "map.h"

using namespace std;

//Helper methods
bool dfsHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path);
void setPath(Vertex*& start, Vertex*& end, Vector<Vertex*>& path);

/*
 * This method implements depth first search. It takes in a graph and a start and end vertex, then searches for
 * a path between them. It returns a vector of vertices that mark the path
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    dfsHelper(graph, start, end, path);
    return path;
}

/*
 * This is a helper method for depth first search that takes in the graph, start and end vertices, and a vector
 * to store the path in. It recursively searches the graph looking for a path between the two vertices. .
 */
bool dfsHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    path.add(start);
    start->visited = true;
    start->setColor(GREEN);

    if (start == end) {
        return true;
    } else {
        Set<Vertex*> neighbors = graph.getNeighbors(start);
        for (Vertex* vertex : neighbors) {
            if (!(vertex->visited) && dfsHelper(graph, vertex, end, path)) {
                return true;
            }
        }
        path.remove(path.size() - 1);
        start->setColor(GRAY);
        return false;
    }
}

/*
 * This method implements breadth first search. It takes in a graph and the start and end vertices to find a path
 * between. It returns a vector of vertices that mark the path.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> queue;

    queue.enqueue(start);
    start->setColor(YELLOW);
    start->visited = true;
    start->setColor(GREEN);

    while (!queue.isEmpty()) {
        Vertex* v = queue.dequeue();
        if (v == end) {
            break;
        }
        Set<Vertex*> neighbors = graph.getNeighbors(v);
        for (Vertex* vertex : neighbors) {
            if (!vertex->visited) {
                queue.enqueue(vertex);
                vertex->setColor(YELLOW);
                vertex->visited = true;
                vertex->setColor(GREEN);
                vertex->previous = v;

            }
        }
    }

    setPath(start, end, path);
    return path;
}

/*
 * This method implements Dijkstra's algorithm. It takes in a graph and the start and end vertices to find a path
 * between. It returns a vector of vertices that mark the path.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pqueue;
    Set<Vertex*>  vertices = graph.getVertexSet();
    for (Vertex* v: vertices) {
        v->cost = POSITIVE_INFINITY;
    }

    start->cost = 0;
    pqueue.enqueue(start, 0);
    start->setColor(YELLOW);

    while (!pqueue.isEmpty()) {
        Vertex* vertex = pqueue.dequeue();
        vertex->visited = true;
        vertex->setColor(GREEN);
        if (vertex == end) {
            break;
        }
        Set<Vertex*> neighbors = graph.getNeighbors(vertex);
        for (Vertex* n : neighbors) {
            if (!n->visited) {
                Edge* e = graph.getEdge(vertex, n);
                double cost = vertex->cost + e->cost;
                if (cost < n->cost) {
                    n->previous = vertex;
                    if (n->cost != POSITIVE_INFINITY) {
                        n->cost = cost;
                        pqueue.changePriority(n, cost);
                    } else {
                        n->cost = cost;
                        pqueue.enqueue(n, cost);
                        n->setColor(YELLOW);
                    }
                }
            }
        }
    }

    setPath(start, end, path);
    return path;
}

/*
 * This method implements A* search and takes in a graph and a start and end vertex to find a path between. It
 * returns a vector of vertices that mark the path.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pqueue;
    Set<Vertex*>  vertices = graph.getVertexSet();
    for (Vertex* v: vertices) {
        v->cost = POSITIVE_INFINITY;
    }

    start->cost = 0;
    pqueue.enqueue(start, heuristicFunction(start, end));
    start->setColor(YELLOW);

    while (!pqueue.isEmpty()) {
        Vertex* vertex = pqueue.dequeue();
        vertex->visited = true;
        vertex->setColor(GREEN);
        if (vertex == end) {
            break;
        }
        Set<Vertex*> neighbors = graph.getNeighbors(vertex);
        for (Vertex* n : neighbors) {
            if (!n->visited) {
                Edge* e = graph.getEdge(vertex, n);
                double cost = vertex->cost + e->cost;
                if (cost < n->cost) {
                    n->previous = vertex;
                    if (n->cost != POSITIVE_INFINITY) {
                        n->cost = cost;
                        pqueue.changePriority(n, cost + heuristicFunction(n, end));
                    } else {
                        n->cost = cost;
                        pqueue.enqueue(n, cost + heuristicFunction(n, end));
                        n->setColor(YELLOW);
                    }
                }
            }
        }
    }
    setPath(start, end, path);
    return path;

}

/*
 * This is a helper function for bfs, Dijkstra's and A*. It takes in the start and end vertices and a path, then
 * traces the path backwards from the end vertex following its previous pointer all the way back to start.
 */
void setPath(Vertex*& start, Vertex*& end, Vector<Vertex*>& path) {
    if (end->previous != NULL) {
        Vertex* temp = end;
        while (temp != start) {
            path.add(temp);
            //temp->setColor(GREEN);
            temp = temp->previous;
        }
        path.add(start);
        //start->setColor(GREEN);
    }
}

/*
 * This method implements Kruskal's algoritm for finding the minimum spanning tree. It takes in a graph and returns
 * a set of edges that make up the minimum spanning tree. It is used to generate random mazes.
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    PriorityQueue<Edge*> pqueue;
    Map<Vertex*, Set<Vertex*>* > map;
    Set<Vertex*> vertices = graph.getVertexSet();
    for (Vertex* v : vertices) {
        Set<Vertex*>* p = new Set<Vertex*>;
        p->add(v);
        map.put(v, p);
    }

    Set<Edge*> edges = graph.getEdgeSet();
    for (Edge* e : edges) {
        pqueue.enqueue(e, e->cost);
    }

    while (!pqueue.isEmpty()) {
        Edge* edge = pqueue.dequeue();
        if (!(map.get(edge->start)->contains(edge->finish))) {
            Set<Vertex*>* startP = map[edge->start];
            Set<Vertex*>* finishP = map[edge->finish];
            *startP += *finishP;
            for (Vertex* vertex : *finishP) {
                map[vertex] = startP;
            }
            delete finishP;
            mst.add(edge);
        }
    }

    delete map[vertices.first()];

    return mst;
}
