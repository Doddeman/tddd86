//Implementation of four pathfinding algorithms
//depth first search, breadth first search,
//dijkstras algorithm and A* search

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "stack.h"

using namespace std;

//finds path by using previous pointers
//used by both DFS and BFS
vector<Node *> findPath(Vertex* end){
    vector<Node *> path;
    Node* node = end;
    while(node->previous != nullptr){
        path.insert(path.begin(), node);
        node = node->previous;
    }
    path.insert(path.begin(), node);
    return path;
}


vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    Stack<Node*> stack;
    stack.push(start);

    bool finished = false;
    while(!stack.isEmpty()){
        if(finished){
            path = findPath(end);
            break;
        }
        Node *currentNode = stack.top();
        stack.pop();
        currentNode->setColor(GREEN);
        currentNode->visited = true;
        bool deadEnd = true;
        for(Node* neighbor : graph.getNeighbors(currentNode)){
            if(neighbor == end){
                //cout << "end: " << *neighbor << endl;
                neighbor->setColor(GREEN);
                neighbor->previous = currentNode;
                finished = true;
                deadEnd = false;
                break;
            }
            else if(!neighbor->visited){
                //cout << "node: " << *neighbor << endl;
                neighbor->setColor(YELLOW); //marks queued paths to be explored
                neighbor->previous = currentNode;
                stack.push(neighbor);
                deadEnd = false;
            }
        }
        if(deadEnd){
            //marks dead ends
            //currentNode->setColor(GRAY);
        }
    }
    return path;
}

vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    Queue<Node*> queue;
    queue.enqueue(start);

    bool finished = false;
    while(!queue.isEmpty()){
        if(finished){
            path = findPath(end);
            break;
        }
        Node * currentNode = queue.dequeue();
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
                neighbor->setColor(YELLOW);
                neighbor->previous = currentNode;
                queue.enqueue(neighbor);
            }
        }
    }
    return path;
}

vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    vector<Vertex*> path;
    return path;
}

vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    vector<Vertex*> path;
    return path;
}
