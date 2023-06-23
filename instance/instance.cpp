#include <stdlib.h>
#include <stdexcept>
#include "instance.h"

instance::instance(int num_variables, int num_clauses, clause **clauses){
    instance::num_variables = num_variables;
    instance::num_clauses = num_clauses;
    instance::clauses = clauses;
}

bool instance::contains(clause *c){
    for (int i = 0; i < instance::num_clauses; i++){
        if (clauses_equal(c, instance::clauses[i])){
            return true;
        }
    }
    return false;
}

instance *instance::extend(clause *c){
    /*
    The state of an instance is assumed to satisfy the following
    - Every clause is normalized
    - If 0 <= i < instance::num_variables then there exists some clause in the
    instance with (xi) or (not xi) as a literal
    */

    int i;
    int num_variables = instance::num_variables;
    int num_clauses = instance::num_clauses + 1;

    // Increment the num_variables counter for every new variable
    for (i = 0; i < 3; i++){
        if (c->variables[i] >= instance::num_variables){
            if (c->variables[i] > instance::num_variables + 3){
                throw std::invalid_argument("Clause has variable indices higher than expected");
            }
            num_variables += 1;
        }
    }

    // Create extended list of clauses
    clause** extended_clauses = (clause**) malloc(num_clauses * sizeof(clause *));
    for (i = 0; i < instance::num_clauses; i++){
        extended_clauses[i] = instance::clauses[i];
    }
    extended_clauses[i] = c;

    return new instance(num_variables, num_clauses, extended_clauses);
}

int instance::num_satisfied(){
    return instance::num_satisfied(-1, -1, -1);
}

int instance::num_satisfied(int i){
    return instance::num_satisfied(i, -1, -1);
}

int instance::num_satisfied(int i, int j){
    return instance::num_satisfied(i, j, -1);
}

int instance::num_satisfied(int i, int j, int k){
    int satisfied = 0;
    for (int m = 0; m < instance::num_clauses; m++){
        if (instance::clauses[m]->eval(i, j, k)){
            satisfied += 1;
        }
    }
    return satisfied;
}

std::set<int> instance::local_max_obstruction(int t){
    int value;
    std::set<int> indices;

    // Number of satisfied clauses at the zero assignment 
    int zero_value = instance::num_satisfied();

    for (int i = 0; i < instance::num_variables; i++){
        // Number of clauses satisfied when xi is flipped to 1
        value = instance::num_satisfied(i);

        // If flipping xi gives a higher number of satisfied clauses than the 
        // zero assignment
        if (value > zero_value){
            indices.insert(i);
            return indices;
        }

        // If we're only flipping upto 1 variable
        if (t == 1){
            continue;
        }

        for (int j = 0; j < i; j++){
            value = instance::num_satisfied(i, j);
            if (value > zero_value){
                indices.insert(i);
                indices.insert(j);
                return indices;
            }
        }
    }
    return indices;
}