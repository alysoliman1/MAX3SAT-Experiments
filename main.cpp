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
    clause *c;
    instance *maxsat;
    std::set<int> obstructions;
    std::set<int>::iterator var1;
    std::set<int>::iterator var2;
    std::set<int>::iterator obstruction;

    

    while (!q.empty()){
        // Grab the next instance from the queue
        maxsat = q.front();
        q.pop();

        // Get a set of variables that are obstructions to the zero assignment
        // being a local max
        obstructions = maxsat->obstructions(2);

        // If the zero assignment is indeed a local max
        if (obstructions.empty()){
            
            
            
            delete maxsat;
            continue;
        }

        // Iterate through the obstructing variables
        for (obstruction = obstructions.begin(); obstruction != obstructions.end(); obstruction++){
            // For all clauses of the form a or b or (not var) where 
            // a and b are in maxsat->variables \ obstructions
            // one of a or b is in maxsat->variables \ obstructions and the other is new
            // both a and b are new
            

            // Iterate through all pairs of variables in our instance
            for (var1 = maxsat->variables.begin(); var1 != maxsat->variables.end(); var1++){
                for (var2 = maxsat->variables.begin(); var2 != var1; var2++){
                    if (obstructions.count(*var1) || obstructions.count(*var2)){
                        continue;
                    }

                    c = new clause(false, *var1, false, *var2, true, *obstruction);
                    q.push(maxsat->extend(c));
                }
            }

            for (var1 = maxsat->variables.begin(); var1 != maxsat->variables.end(); var1++){
                if (obstructions.count(*var1)){
                        continue;
                }

                c = new clause(false, *var1, false, fresh_variable(maxsat->variables), true, *obstruction);
                q.push(maxsat->extend(c));
            }

            c = new clause(false, fresh_variable(maxsat->variables), false, fresh_variable(maxsat->variables) + 1, true, *obstruction);
            q.push(maxsat->extend(c)); 
        }

        if (obstructions.size() < 2){
            continue;
        }

        int obstruction1, obstruction2;
        obstruction = obstructions.begin();
        obstruction1 = *obstruction;
        obstruction ++;
        obstruction2 = *obstruction;
        
        for (var1 = maxsat->variables.begin(); var1 != maxsat->variables.end(); var1++){
            if (obstructions.count(*var1)){
                continue;
            }

            c = new clause(false, *var1, true, obstruction1, true, obstruction2);
            q.push(maxsat->extend(c));  
        }

        c = new clause(false, fresh_variable(maxsat->variables), true, obstruction1, true, obstruction2);
        q.push(maxsat->extend(c));  
    }

    return 0;
}