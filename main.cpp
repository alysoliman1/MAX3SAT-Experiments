#include <queue>
#include <iostream>
#include "clause/clause.h"
#include "instance/instance.h"

int main(){
    // Create initital instance of MAX3SAT containing the single clause 
    clause *clauses[1] = {
        new clause(false, 0, false, 1, false, 2)
    };
    instance *initial_instance = new instance(3, 1, clauses);

    // enqueue the initial instance in the queue
    std::queue<instance*> q;
    q.push(initial_instance);


    int cases = 0;
    int len = 0;

    // Variables to be used later in the procedure
    int k;
    clause *c;
    bool clause_exists;
    instance *maxsat;
    std::set<int> indices;
    std::set<int>::iterator itr;

    while (!q.empty()){
        std::cout << q.size() << " " << cases << " " << len << "\n";

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
            cases += 1;
            if (len < maxsat->num_clauses){
                len = maxsat->num_clauses;
            }
            
            delete maxsat;
            continue;
        }

       // Procedure A
        for (int i = 0; i < maxsat->num_variables; i++){
            // Make sure i is not in indices
            if (indices.count(i) == 1){
                continue;
            }
            
            // For k in indices
            for (itr = indices.begin(); itr != indices.end(); itr++){
                k = *itr;
                c = new clause(false, i, false, maxsat->num_variables, true, k);
                q.push(maxsat->extend(c));
            }

            for (int j = 0; j < i; j++){
                // Make sure j is not in indices
                if (indices.count(i) == 1){
                    continue;
                }

                for (itr = indices.begin(); itr != indices.end(); itr++){
                    k = *itr;
                    // Create the new clause and check if it's not already in the instance
                    c = new clause(false, i, false, j, true, k);
                    if (maxsat->contains(c)){
                        delete c;
                        continue;
                    }
                    q.push(maxsat->extend(c));
                }
            }
        }

        for (itr = indices.begin(); itr != indices.end(); itr++){
            k = *itr;
            c = new clause(false, maxsat->num_variables, false, maxsat->num_variables + 1, true, k);
            q.push(maxsat->extend(c));        
        }

        // Procedure B
        if (indices.size() < 2){
            continue;
        }

        itr = indices.begin();
        int p = *itr;
        itr++;
        int k = *itr;

        for (int i = 0; i < maxsat->num_variables; i++){
            // Make sure i is not in indices
            if (indices.count(i) == 1){
                continue;
            }

           c = new clause(false, i, true, p, true, k); 
           q.push(maxsat->extend(c));    
        }

        c = new clause(false, maxsat->num_variables, true, p, true, k);
        q.push(maxsat->extend(c));         


    }

    return 0;
}