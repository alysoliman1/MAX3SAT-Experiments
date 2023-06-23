#include <queue>
#include <iostream>
#include "clause/clause.h"
#include "instance/instance.h"

int main(){
    // Create first clause and instance of MAX3SAT
    clause *c = new clause(false, 0, false, 1, false, 2);
    clause *clauses[1] = {c};
    instance *first_instance = new instance(3, 1, clauses);

    std::queue<instance*> q;
    q.push(first_instance);

    bool clause_exists;
    instance *maxsat;
    std::set<int> indices;
    while (!q.empty()){
        // dequeue q
        maxsat = q.front();
        q.pop();

        // perform the procedure
        indices = maxsat->local_max_obstruction(1);

        // If the zero assignment is a 1-local maximum
        if (indices.empty()){
            std::cout << maxsat->num_clauses << "\n";
            continue;
        }
        
        // Get the first (and only) element in the set
        // The value of k will be an index for a variable that, when flipped,
        // will give a higher number of satisfied clauses than the zero assignment
        int k = *indices.begin();

        // Hence, we must expand the list to include a clause of the form 
        // a or b or (not x_k) where a and b are literals with no negations

        // First add clauses of the form
        // (xi) or (xj) or (not xk)
        // where xi and xj are variables used in the instance
        for (int i = 0; i < maxsat->num_variables; i++){
            if (i == k){
                continue;
            }

            c = new clause(false, i, false, maxsat->num_variables, true, k);
            q.push(maxsat->extend(c));

            for (int j = 0; j < i; j++){
                if (j == k){
                    continue;
                }

                // Make sure the clause (xi) or (xj) or (not xk) is not already
                // in our instance
                c = new clause(false, i, false, j, true, k);
                if (maxsat->contains(c)){
                    delete c;
                    continue;
                }

                q.push(maxsat->extend(c));
            }
        }

        c = new clause(false, maxsat->num_variables, false, maxsat->num_variables + 1, true, k);
        q.push(maxsat->extend(c));        
    }

    return 0;
}