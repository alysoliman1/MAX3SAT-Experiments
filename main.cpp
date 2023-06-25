#include <queue>
#include <iostream>
#include "clause/clause.h"
#include "instance/instance.h"

int main(int argc, char **argv){
    int hamming_radius;
    if (argc < 2){
        return 0;
    }
    hamming_radius = atoi(argv[1]);

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
    std::set<int>::iterator var1, var2;
    int fresh_var1, fresh_var2;
    std::set<int>::iterator obstruction1, obstruction2, obstruction3; 

    while (!q.empty()){
        // Grab the next max3sat instance from the queue
        maxsat = q.front();
        q.pop();
 
        // Get a set of variables that are obstructions to the zero assignment
        // being a local max
        obstructions = maxsat->obstructions(hamming_radius);
 
        // If the zero assignment is indeed a local max
        if (obstructions.empty()){
            delete maxsat;
            continue;
        }

        // Generate two variables that are not used by the current instance
        fresh_var1 = fresh_variable(maxsat->variables);
        fresh_var2 = fresh_var2 + 1;


        

        // Iterate through the obstructing variables
        for (int obstruction : obstructions){

            ////////////////////////////////////////////////////////////////////

            // Iterate through all pairs of variables in our instance
            for (var1 = maxsat->variables.begin(); var1 != maxsat->variables.end(); var1++){
                for (var2 = maxsat->variables.begin(); var2 != var1; var2++){
                    // Make sure our variables are not obstructing variables
                    if (obstructions.count(*var1) || obstructions.count(*var2)){
                        continue;
                    }

                    // Create a new clause with one obstructing variable
                    c = new clause(false, *var1, false, *var2, true, obstruction);
                    if (maxsat->contains(c)){
                        delete c;
                        continue;
                    }
                    q.push(maxsat->extend(c));
                }
            }

            ////////////////////////////////////////////////////////////////////

            // Iterate through all variables in our instance
            for (int var : maxsat->variables){
                if (obstructions.count(var)){
                    continue;
                }

                c = new clause(false, var, false, fresh_var1, true, obstruction);
                q.push(maxsat->extend(c));
            }

            ////////////////////////////////////////////////////////////////////

            c = new clause(false, fresh_var1, false, fresh_var2, true, obstruction);
            q.push(maxsat->extend(c)); 
        }

        if (obstructions.size() < 2){
            continue;
        }

        // Iterate through all pairs of elements in the set
        for (obstruction1 = obstructions.begin(); obstruction1 != obstructions.end(); obstruction1++){
            for (obstruction2 = obstructions.begin(); obstruction2 != obstruction1; obstruction2++){

                
                for (int var : maxsat->variables){
                    if (obstructions.count(var)){
                        continue;
                    }

                    c = new clause(false, var, true, *obstruction1, true, *obstruction2);
                    if (maxsat->contains(c)){
                        delete c;
                        continue;
                    }
                    q.push(maxsat->extend(c));  
                }

                c = new clause(false, fresh_variable(maxsat->variables), true, *obstruction1, true, *obstruction2);
                q.push(maxsat->extend(c));  
            }
        }

        if (obstructions.size() < 3){
            continue;
        }

        std::set<int>::iterator obstruction = obstructions.begin();
        obstruction1 = obstruction;
        obstruction++;
        obstruction2 = obstruction;
        obstruction++;
        obstruction3 = obstruction;

        c = new clause(true, *obstruction1, true, *obstruction2, true, *obstruction3);
        if (maxsat->contains(c)){
            delete c;
            continue;
        }
        q.push(maxsat->extend(c)); 
    }

    return 0;
}