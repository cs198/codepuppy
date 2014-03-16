/* Name: Kevin Vo
 * TA: Jimmy Lee
 * This program implements five search algorithms
 * 1) Depth-first search: picks a path explores the path to the end, backtracks if the target is not found
 * 2) Breadth-first search: takes a step in each direction and explores until target is found
 * 3) Dijkstra's algorithm: greedy algorithm, finds the minimum cost path
 * 4) A* algorithm: same as Dijkstra's but with a heuristic
 * 5) Kruskal's algorithm: finds the minimum spanning tree.
*/

#include "costs.h"
#include "trailblazer.h"
#include "trailblazergui.h"
#include "adapter.h"
#include "types.h"
#include "queue.h"
#include "pqueue.h"
#include "hashmap.h"

using namespace std;

Vector<Vertex*> dijkstrasAlgorithmHelper(BasicGraph& graph, Vertex* start, Vertex* end, double heuristicValue, bool isAstar);

/*This function implments depth first search. Depth first search chooses a path
and explores it until it reaches a dead end or the target. This function utilizes
recursive backtracking. dfs is the helper function. Choosing and unchoosing is done
through boolean flags in the vertex field of visited. I utilize a vector of vertices
to keep track of the path.*/
bool dfs(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& visited){
    start->visited = true;
    visited.add(start);
    start->setColor(GREEN);
    if (start == end){
        return true;
    }else{
        for (Edge *n : start->edges){
            if(n->finish->visited != true){
               if(dfs(graph, n->finish, end, visited)){
                   return true;
               }
            }
        }
        start->visited = false;
        visited.remove(visited.size()-1);
        start->setColor(GRAY);
        return false;
    }  
}

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> visited;
    dfs(graph, start, end, visited);
    return visited;
}

/*This function implements breadthFirstSearch. This type of search guarantees that
it will find the shortest path. The algorithm takes a step in every direction and
continues along each layer.*/
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Queue<Vertex*> q;
    Vector<Vertex*> path;
    q.enqueue(start);
    start->visited = true;
    while (!q.isEmpty()){
        Vertex* node = q.dequeue();
        node->setColor(GREEN);
        if (node == end){
            while(node != NULL){
                path.insert(0,node);
                node = node->previous;
            }
            return path;
        }else{
            for (Edge *n : node->edges){
                if (n->finish->visited != true){
                    n->finish->visited = true;
                    n->finish->previous = node;
                    q.enqueue(n->finish);
                    n->finish->setColor(YELLOW);
                }
            }
        }
    }
    return path;
}
/*This is the helper function that implements both A* and Dijkstra's algorithm. It takes in two extra parameters
a heuristic value and a boolean to see if it is Dijkstra or A*. For Dijkstra's algorithm I pass in zero for the
heuristic value. If it is aStar then the heuristic value is activated.*/
Vector<Vertex*> dijkstrasAlgorithmHelper(BasicGraph& graph, Vertex* start, Vertex* end, double heuristicValue, bool isAStar){
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> PQ;
    Set<Vertex*> visited;
    for (Vertex* vertex : graph.getVertexSet()){
        vertex->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    PQ.enqueue(start, heuristicValue);
    visited.add(start);
    while (!PQ.isEmpty()){
        Vertex* node = PQ.dequeue();
        node->visited = true;
        node->setColor(GREEN);
        visited.remove(node);
        if (node == end){
            // reconstruct by following pointers
            while (node != NULL){
                path.insert(0,node);
                node = node->previous;
            }
            return path;
        }
        //explore the neighbors
        for (Edge* n : node->edges){
            // heuristic value is activated
            if (isAStar){
                heuristicValue = heuristicFunction(n->finish, end);
            }
            if (n->finish->visited != true){
                double cost = (node->cost) + (n->cost);
                if (cost < n->finish->cost){
                    n->finish->cost = cost;
                    n->finish->previous = node;
                    if (visited.contains(n->finish)){
                        PQ.changePriority(n->finish, cost + heuristicValue);
                    }else{
                        PQ.enqueue(n->finish,cost + heuristicValue);
                        visited.add(n->finish);
                        n->finish->setColor(YELLOW);
                    }
                }
            }
        }
    }
    return path;
}

/*This function implements Dijkstra's algorithm. I break it up into steps.
1) Set all the cost of the nodes to positive infinity.
2) Enqueue the start node into the priority queue based on the cost of the edge.
3) Dequeue the node, mark as visited.
4) Explore the neighbors.
5) If the finished cost is greater than the start and finish cost combined
set the finish node's cost to the lower cost and set a pointer back to the previous node.
6) change the priority of the node if it already exists. */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    return dijkstrasAlgorithmHelper(graph,start,end,0,false);
}

/*A* function is literally Dijkstra's algorithm but with a heuristic function
The heuristic function is used each time you enqueue the priority queue*/
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
   return dijkstrasAlgorithmHelper(graph, start, end, heuristicFunction(start,end), true);
}
/*Kruskal's algorithm finds the minimum spanning tree. I separate the vertices into
sets, which represent the designated cluster. I then enqueue all the edges of the graph.
I then compare the sets to see if the next vertex is in the set, if it is then don't cluster
the sets, if the next vertex is not in the set then combine the sets. To cluster the vertices
I used a map to map each vertices to its respective cluster.*/
Set<Edge*> kruskal(BasicGraph& graph) {
    graph.resetData();
    Set<Edge*> mst;
    PriorityQueue<Edge*> PQ;
    Map<Vertex*, Set<Vertex*>> M;
    for (Vertex* vertex : graph.getVertexSet()){
        Set<Vertex*> s;
        s.add(vertex);
        M.put(vertex,s);
    }
    for (Edge* edge : graph.getEdgeSet()){
        PQ.enqueue(edge, edge->cost);
    }
    Set<Vertex*> set1;
    Set<Vertex*> set2;
    while (!PQ.isEmpty()){
        Edge* edge = PQ.dequeue();
        set1 = M.get(edge->start);
        set2 = M.get(edge->finish);
        if (!set1.contains(edge->finish) && !set2.contains(edge->start)){
            set1 += set2;
            // change each vertex in the set to the new value.
            for (Vertex* v : set1){
                M.put(v, set1);
            }
            mst.add(edge);
        }
    }
    return mst;
}
