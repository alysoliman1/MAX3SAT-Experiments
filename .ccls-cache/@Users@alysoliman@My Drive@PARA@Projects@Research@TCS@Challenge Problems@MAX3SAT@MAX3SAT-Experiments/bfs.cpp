#include <queue>
#include <iostream>
#include <thread>
#include "clause/clause.hpp"
#include "instance/instance.hpp"

static const int num_threads = 8;
static std::mutex queue_mtx, io_mtx;
static std::queue<instance*> queue;

void worker(int tid, int hamming_radius){
    instance *node;
    std::vector<instance *> *children;
    while (true){
        queue_mtx.lock();
        if (queue.empty()){
            queue_mtx.unlock();
            break;
        }
        // Grab the next MAX3SAT instance from the queue
        node = queue.front();
        queue.pop();
        queue_mtx.unlock();

        children = node->children(hamming_radius);       
        
        if (children->empty()){
            // A counter example is found
            continue;
        }

        for (instance *child : *children){
            // Check if the child is isomorphic to a node that has been visited 
            // before. If not then just push the child to the queue
            queue_mtx.lock();
            queue.push(child);
            queue_mtx.unlock();
            // Add the isomorphism class of the node
        }
    }    
}

int main(int argc, char **argv){
    if (argc < 2){
        return 0;
    }
    
    const int hamming_radius = atoi(argv[1]);

    // Create and enqueue the initital instance of MAX3SAT
    std::vector<clause*> *clauses = new std::vector<clause*> {
        new clause(false, 0, false, 1, false, 2)
    };
    instance *initial_instance =  new instance(clauses);

    queue.push(initial_instance);

    std::thread threads[num_threads];
    for (int i = 0; i < num_threads; i++){
        threads[i] = std::thread(worker, i, hamming_radius);
    }

    for (int i = 0; i < num_threads; i++){
        threads[i].join();
    }

    return 0;
}