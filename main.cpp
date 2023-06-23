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
    std::set<int>::iterator itr;
    int k;

    int cases = 0;
    int len = 0;

    while (!q.empty()){
        std::cout << q.size() << " " << cases << " " << len << "\n";

        // dequeue q
        maxsat = q.front();
        q.pop();

        // perform the procedure
        indices = maxsat->local_max_obstruction(2);

        // If the zero assignment is a 2-local maximum
        if (indices.empty()){
            cases += 1;
            if (len < maxsat->num_clauses){
                len = maxsat->num_clauses;
            }
            
            delete maxsat;
            continue;
        }

        /*
        
        Otherwise, if indices = {k} then we need to 

        a or b or (not xk) where a and b are not k 

        if indices = {k, p} then we need to add

        a or b or (not xk) where a, b are not k or p (i.e not in indices)
        a or b or (not xp) where a, b are k or p (i.e not in indices)
        a or (not xk) or (not xp) (a not in indices)

        --------------

        Hence, in summary, 
        A) For every k in indices, add a or b or xk where a and b are not in indices
        There are three options
        - one of a or b is new
        - both a and b are variables in the instance
        - both of a and b are new

       B) Furthermore, if indices = {k, p} then add 
       a or (not xk) or (not xp) (a not in indices) 

       - a is in the instance
       - a is new
        */

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