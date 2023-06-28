#include <queue>
#include <iostream>
#include "clause/clause.hpp"
#include "instance/instance.hpp"

void BFS(int hamming_radius){
    std::queue<instance*> queue;

    // Create and enqueue the initital instance of MAX3SAT
    std::vector<clause*> clauses;
    clauses.push_back(new clause(false, 0, false, 1, false, 2));
    instance *initial_instance =  new instance(&clauses);
    queue.push(initial_instance);

    instance *node;
    std::vector<instance *> *children;
    while (!queue.empty()){
        // Grab the next MAX3SAT instance from the queue
        node = queue.front();
        queue.pop();
        children = node->children(hamming_radius);       
        
        if (children->empty()){
            std::cout << node->num_clauses() << "\n";
            node->print();
            exit(0);
        }

        for (instance *child : *children){
            // Check if the child is isomorphic to a node that has been visited before
            queue.push(child);
            // Add the isomorphism class of the node
        }
    }    
}

int main(int argc, char **argv){
    if (argc < 2){
        return 0;
    }
    int hamming_radius = atoi(argv[1]);
    BFS(hamming_radius);
    return 0;
}