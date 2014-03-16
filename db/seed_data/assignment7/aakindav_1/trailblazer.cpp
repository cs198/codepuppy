/* Charles Akin-David
 * CS106B
 * SL: Jimmy Lee
 * Sources used: Lecture Slides, My BRAIN!!!
 * This program implements 4 (DFS, BFS, DIjkstra's Algorithm, and A*)
 * different search algoritms to find a path in a maze as well as
 * Kruskal's Algorithm in finding a minimum spanning tree.
 */

#include "trailblazer.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "pqueue.h"
#include "map.h"

using namespace std;

// Helper functions
bool depthFirstSearchHelper (Vector<Vertex*> &path, BasicGraph& graph, Vertex* start, Vertex* end);

/* Creates a Vector to store the path then calls the helper function
 * to run the recursive algorithm.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    depthFirstSearchHelper(path, graph, start, end);
    return path;
}
/* Takes in a reference to a path, the graph and the start and end vertices.
 * Adds a vertex to the path and marks it has visited then loops through all
 * of it's unvisited neighbors and runs the function on them recursively.
 * Function breaks if the end is found, if the path leads to no where, or
 * if the vertex is not contained in the graph.
 */
bool depthFirstSearchHelper (Vector<Vertex*> &path, BasicGraph& graph, Vertex* start, Vertex* end) {
    if (graph.containsVertex(start)) {
        path.add(start);
        start->visited = true;
        start->setColor(GREEN);
        if (start == end) return true;
        for (Vertex* neighbor: graph.getNeighbors(start)) {
            if (neighbor->visited == false) {
                if (depthFirstSearchHelper(path, graph, neighbor, end)) return true;
            }
        }
        path.get(path.size()-1)->setColor(GRAY);
        path.remove(path.size()-1);
        return false;
    }
    else {
        return false;
    }
}
/* Takes in a graph and the start and end vertices.
 * Creates a vector for the chosen path and a queue to remember all the routes
 * Then while the path isn't empty, a vertex is dequeued and set to green and if it
 * hasn't been visited already, each unvisited neighbor of that vertex is enqueued
 * and set to yellow, then set the previous of the neighbor to the inital vertex.
 * Function stops when the end is found or if there is no path (queue is empty).
 * Then retraces the steps taking to find the path and returns the path.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> chosenPath;
    Queue<Vertex*> path;
    path.enqueue(start);
    start->visited = true;
    start->setColor(YELLOW);
    Stack<Vertex*> reorder;
    while (!path.isEmpty()) {
        Vertex* vertex = path.dequeue();
        if (vertex->getColor() != GREEN) {
            vertex->setColor(GREEN);
            if (vertex == end) break;
            for (Vertex* neighbor: graph.getNeighbors(vertex)) {
                if (neighbor->visited == false) {
                    neighbor->visited = true;
                    neighbor->setColor(YELLOW);
                    path.enqueue(neighbor);
                    neighbor->previous = vertex;
                }
            }
        }
    }
    Vertex* temp = end;
    while (temp != start) {
        reorder.push(temp);
        temp = temp->previous;
    }
    reorder.push(start);
    while (!reorder.isEmpty()) chosenPath.add(reorder.pop());
    return chosenPath;
}

/* The general function for a star, which dijkstra falls under as well.
 * A graph, start and end vertices, and start and neighbor heuritics are passed in, neighbor is 0
 * if nothing is passed in. Also if 0 is the start the heuristic neighbor double is not calculated.
 * Function creates a Vector for a path and a PQ, then enqueues vertices, by priority of their
 * heuristic, which is always 0 for Dijkstra. Sets the verttex to be green and marked as true.
 * Then for each neighbor of that vertex a cost is evaluated and if the general cost is less than the
 * neighbor's cost, it becomes the neighbor's cost and that neighbor's previous becomes the vertex, then
 * if the neighbor hasn't been enqueued, then it is with its color set to yello, but if it has then then it's
 * priority is changed to the general cost + the heuristic and that neighbor's previous is reset to the new vertex.
 * Path is traced back and reorder then returned.
 */
Vector<Vertex*> aStarGeneral(BasicGraph& graph, Vertex* start, Vertex* end, double heuristicStart, double heuristicNeighbor = 0.0) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> queue;
    Stack<Vertex*> reorder;
    double cost = 0;
    for (Vertex* vertex: graph.getVertexSet()) {
        vertex->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    start->setColor(YELLOW);
    queue.enqueue(start,heuristicStart);
    while (!queue.isEmpty()) {
        Vertex* vertex = queue.dequeue();
        vertex->visited = true;
        vertex->setColor(GREEN);
        if (vertex == end) break;
        for (Vertex* neighbor: graph.getNeighbors(vertex)) {
            cost = vertex->cost + graph.getEdge(vertex, neighbor)->cost;
            if (cost < neighbor->cost) {
                neighbor->cost = cost;
                neighbor->previous = vertex;
                if (neighbor->getColor() == YELLOW) {
                    if (heuristicStart != 0) heuristicNeighbor = heuristicFunction(neighbor, end);
                    queue.changePriority(neighbor, cost + heuristicNeighbor);
                    neighbor->previous = vertex;
                } else if (neighbor->getColor() == WHITE) {
                    if (heuristicStart != 0) heuristicNeighbor = heuristicFunction(neighbor, end);
                    queue.enqueue(neighbor, cost + heuristicNeighbor);
                    neighbor->setColor(YELLOW);
                }
            }
        }
    }
    Vertex* temp = end;
    while (temp != start) {
        reorder.push(temp);
        temp = temp->previous;
    }
    reorder.push(start);
    while (!reorder.isEmpty()) path.add(reorder.pop());
    return path;
}

/* Calls the aStarGeneral function passing in the graph and start and end vertices,
 * also 0.0 for the heuristicStart.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    return aStarGeneral(graph, start, end, 0.0);
}

/* Calls the aStarGeneral function passing in the graph and start and end vertices, with a
 * calculated heuristicStart double, based on the start and end vertices.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    double heuristicStart = heuristicFunction(start, end);
    return aStarGeneral(graph, start, end, heuristicStart);
}

/* A reference to a graph is passed in.
 * A set of edge pointers is made to store the minimum spanning tree.
 * A PQ is built to order the edges and a map is created to store all the clusters.
 * The edges are all enqueued with priority of their cost, and every vertex is added to the map,
 * with their value being a Set of Vertices pointer. Count is set to the size of the cluster and
 * deincremented as the clusters get combined. The while the their are 2 or more clusters in the
 * the map, an edge is dequeued and if the vertices of the edge aren't in the same set, their sets
 * are combined with every vertex in that set pointing to it. The old set is deleted to clear memory,
 * the count is decremented and the edge is added the the MST.
 * Finally the last cluster is deleted and the MST is returned.
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    PriorityQueue<Edge*> edges;
    Map<Vertex*, Set<Vertex*>* > clusters;
    for (Edge* edge: graph.getEdgeSet()) {
        edges.enqueue(edge, edge->cost);
    }
    Vertex* random = NULL;
    for (Vertex* vertex: graph.getVertexSet()) {
        Set<Vertex*>* single = new Set<Vertex*>;
        single->add(vertex);
        clusters[vertex] = single;
        random = vertex;
    }
    int count = clusters.size();
    while (count >= 2) {
        Edge* check = edges.dequeue();
        if (!clusters[check->start]->contains(check->finish)) {
            *clusters[check->start] += *clusters[check->finish];
            Set<Vertex*>* mainCluster = clusters[check->start];
            Set<Vertex*>* wasteCluster = clusters[check->finish];
            for (Vertex* end: *wasteCluster) {
                clusters[end] = mainCluster;
            }
            delete wasteCluster;
            count--;
            mst.add(check);
        }
    }
    delete clusters[random];
    return mst;
}
