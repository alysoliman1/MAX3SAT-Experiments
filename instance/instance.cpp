#include <stdlib.h>
#include <stdexcept>
#include "instance.h"

instance::instance(clause **clauses, int num_clauses){
    instance::clauses = clauses;
    instance::num_clauses = num_clauses;
    // Create set of variables used in the clauses
    for (int i = 0; i < num_clauses; i++){
        for (int j = 0; j < 3; j++){
            instance::variables.insert(clauses[i]->variables[j]);
        }
    }
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
    int i;
    int num_clauses = instance::num_clauses + 1;
    clause** extended_clauses = (clause**) malloc(num_clauses * sizeof(clause *));
    for (i = 0; i < instance::num_clauses; i++){
        extended_clauses[i] = instance::clauses[i];
    }
    extended_clauses[i] = c;
    return new instance(extended_clauses, num_clauses);
}

// See instance.h
int instance::num_satisfied(int i, int j, int k){
    int satisfied = 0;
    for (int m = 0; m < instance::num_clauses; m++){
        if (instance::clauses[m]->eval(i, j, k)){
            satisfied += 1;
        }
    }
    return satisfied;
}

////////////////////////////////////////////////////////////////////////////////
// Overloading the previous method for ease of use
////////////////////////////////////////////////////////////////////////////////
int instance::num_satisfied(){
    return instance::num_satisfied(-1, -1, -1);
}

int instance::num_satisfied(int i){
    return instance::num_satisfied(i, -1, -1);
}

int instance::num_satisfied(int i, int j){
    return instance::num_satisfied(i, j, -1);
}
////////////////////////////////////////////////////////////////////////////////

std::set<int> instance::local_max_obstruction(int t){
    // Set of variables to be returned
    std::set<int> variables;

    // Number of satisfied clauses at the zero assignment 
    int zero_value = instance::num_satisfied();

    // Iterate through the variables used in the instance
    int value;
    std::set<int>::iterator var;
    for (var = instance::variables.begin(); var != instance::variables.end(); var++){
        // Number of clauses satisfied when the variable is flipped
        value = instance::num_satisfied(*var);

        // If flipping the variable gives a higher number of satisfied clauses
        if (value > zero_value){
            variables.insert(*var);
            return variables;
        }
    }

    // If we're only flipping upto 1 variable
    if (t == 1){
        return variables;
    }

    // Iterate through all pairs of variables
    std::set<int>::iterator var1;
    std::set<int>::iterator var2;
    for (var1 = instance::variables.begin(); var1 != instance::variables.end(); var1++){
        for (var2 = instance::variables.begin(); var2 != var1; var2++){
            // Number of clauses satisfied when the two variables are flipped
            value = instance::num_satisfied(*var1, *var2);
        }

        // If flipping the two variable gives a higher number of satisfied clauses
        if (value > zero_value){
            variables.insert(*var1);
            variables.insert(*var2);
            return variables;
        }
    }

    return variables;
}