//Implementation of four pathfinding algorithms
//depth first search, breadth first search,
//dijkstras algorithm and A* search

#include "costs.h"
#include "trailblazer.h"
#include "stack.h"
#include "queue.h"
#include "pqueue.h"
using namespace std;

//finds path by using previous pointers
//used by all algorithms
vector<Node *> findPath(Vertex* end){
    vector<Node *> path;
    Node* node = end;
    while(node->previous != nullptr){
        path.insert(path.begin(), node);
        node->setColor(GREEN);
        node = node->previous;
    }
    path.insert(path.begin(), node);
    node->setColor(GREEN);
    return path;
}

vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    Stack<Node*> stack;
    stack.push(start);

    Node *currentNode;
    bool finished = false;
    while(!stack.isEmpty()){
        if(finished){
            path = findPath(end);
            break;
        }
        currentNode = stack.pop();
        currentNode->setColor(GREEN);
        currentNode->visited = true;
        bool deadEnd = true;
        for(Node* neighbor : graph.getNeighbors(currentNode)){
            neighbor->setColor(GRAY);
            if(neighbor == end){
                neighbor->setColor(GREEN);
                neighbor->previous = currentNode;
                finished = true;
                deadEnd = false;
                break;
            }
            else if(!neighbor->visited){
                neighbor->visited = true;
                neighbor->setColor(YELLOW); //marks queued paths to be explored
                neighbor->previous = currentNode;
                stack.push(neighbor);
                deadEnd = false;
            }
        }
        if(deadEnd){
            //marks dead ends
            currentNode->setColor(GRAY);
        }
    }
    return path;
}

vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    Queue<Node*> queue;
    queue.enqueue(start);

    Node * currentNode;
    bool finished = false;
    while(!queue.isEmpty()){
        if(finished){
            path = findPath(end);
            break;
        }
        currentNode = queue.dequeue();
        currentNode->setColor(GREEN);
        currentNode->visited = true;
        for(Node* neighbor : graph.getNeighbors(currentNode)){
            if(neighbor == end){
                //cout << "end: " << *neighbor << endl;
                neighbor->setColor(GREEN);
                neighbor->previous = currentNode;
                finished = true;
                break;
            }
            if(!neighbor->visited){
                //cout << "node: " << *neighbor << endl;
                neighbor->visited = true;
                neighbor->setColor(YELLOW);
                neighbor->previous = currentNode;
                queue.enqueue(neighbor);
            }
        }
    }
    return path;
}

vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;

    //Initialize priority queue
    PriorityQueue<Node*> pQueue;
    for(Node * node : graph.getNodeSet()){
        node->cost = INFINITY;
        pQueue.enqueue(node, node->cost);
    }
    start->cost = 0;
    pQueue.changePriority(start, start->cost);

    Node* currentNode;
    while(!pQueue.isEmpty()){
        currentNode = pQueue.dequeue();
        if(currentNode == end){
            path = findPath(end);
            break;
        }
        //cout << "current: " << *currentNode << endl;
        currentNode->setColor(GREEN);
        currentNode->visited = true;
        for(Edge * edge : graph.getEdgeSet(currentNode)){
            Node * neighbor = edge->finish;
            if(!neighbor->visited){
                double cost = currentNode->cost + edge->cost;
                if(cost < neighbor->cost){
                    neighbor->cost = cost;
                    neighbor->setColor(YELLOW);
                    neighbor->previous = currentNode;
                    pQueue.changePriority(neighbor, cost);
                }
            }
        }
    }
    return path;
}

vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;

    //Initialize priority queue
    PriorityQueue<Node*> pQueue;
    for(Node * node : graph.getNodeSet()){
        node->cost = INFINITY;
        pQueue.enqueue(node, node->cost);
    }
    start->cost = 0;
    pQueue.changePriority(start, start->heuristic(end));

    Node* currentNode;
    bool finished = false;
    while(!pQueue.isEmpty()){
        if(finished){
            path = findPath(end);
            break;
        }
        currentNode = pQueue.dequeue();
        currentNode->setColor(GREEN);
        currentNode->visited = true;
        for(Edge * edge : graph.getEdgeSet(currentNode)){
            Node * neighbor = edge->finish;
            if(neighbor == end){
                neighbor->setColor(GREEN);
                neighbor->previous = currentNode;
                finished = true;
                break;
            }
            else if(!neighbor->visited){
                double cost = currentNode->cost + edge->cost;
                if(cost < neighbor->cost){
                    neighbor->cost = cost;
                    neighbor->setColor(YELLOW);
                    neighbor->previous = currentNode;
                    pQueue.changePriority(neighbor, cost + neighbor->heuristic(end));
                }
            }
        }
    }
    return path;
}
