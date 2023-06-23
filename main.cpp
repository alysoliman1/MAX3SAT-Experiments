#include <queue>
#include <iostream>
#include "clause/clause.h"
#include "instance/instance.h"

int main(){
    // Create initital instance of MAX3SAT containing the single clause 
    clause *clauses[1] = {
        new clause(false, 0, false, 1, false, 2)
    };
    instance *initial_instance = new instance(clauses, 1);

    // enqueue the initial instance in the queue
    std::queue<instance*> q;
    q.push(initial_instance);


    // Variables to be used later in the procedure
    int k;
    clause *c;
    bool clause_exists;
    instance *maxsat;
    std::set<int> indices;
    std::set<int>::iterator itr;

    while (!q.empty()){
        // Grab the next MAX3SAT instance from the queue
        maxsat = q.front();
        q.pop();

        // If the zero assignment is not a 2-local maximum then indices will
        // contain a set of variables that when flipped will give a higher
        // number of satisfied clauses than the zero assignment.
        indices = maxsat->local_max_obstruction(2);

        // Otherwise, if the zero assignment is a 2-local maximum then the
        // returned set will be empty.
        if (indices.empty()){
            delete maxsat;
            continue;
        }

    }


    return 0;
}