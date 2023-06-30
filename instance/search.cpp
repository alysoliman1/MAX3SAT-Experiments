#include "instance.hpp"

/*
Returns a set of at most t variables that when flipped will give
a higher number of satisfied clauses than the zero assignment - these
variables are obstructions to the zero assignment being a t-local max.
Otherwise, if the zero assignment is a t-local maximum then the empty set
is returned.
*/
std::set<int> *instance::obstructions(int t){
    // Set of variables to be returned
    std::set<int> *variables = new std::set<int>;
    std::set<int>::iterator var1, var2, var3;

    // Number of satisfied clauses at the zero assignment 
    int zero_value = instance::num_satisfied();

    // Iterate through the variables used in the instance
    int value;
    for (int var : *instance::vars){
        // Number of clauses satisfied when the variable is flipped
        value = instance::num_satisfied(var);

        // If flipping the variable gives a higher number of satisfied clauses
        if (value > zero_value){
            variables->insert(var);
            return variables;
        }
    }

    // If we're only flipping upto 1 variable
    if (t < 2){
        return variables;
    }

    // Iterate through all pairs of variables
    for (var1 = instance::vars->begin(); var1 != instance::vars->end(); var1++){
        for (var2 = instance::vars->begin(); var2 != var1; var2++){
            // Number of clauses satisfied when the two variables are flipped
            value = instance::num_satisfied(*var1, *var2);

            // If flipping the two variable gives a higher number of satisfied clauses
            if (value > zero_value){
                variables->insert(*var1);
                variables->insert(*var2); 
                return variables;
            }
        }
    }

    if (t < 3){
        return variables;
    }

    // Iterate through all triples of variables
    for (var1 = instance::vars->begin(); var1 != instance::vars->end(); var1++){
        for (var2 = instance::vars->begin(); var2 != var1; var2++){
            for (var3 = instance::vars->begin(); var3 != var2; var3++){
                // Number of clauses satisfied when the two variables are flipped
                value = instance::num_satisfied(*var1, *var2, *var3);

                // If flipping the two variable gives a higher number of satisfied clauses
                if (value > zero_value){
                    variables->insert(*var1);
                    variables->insert(*var2); 
                    variables->insert(*var3);
                    return variables;
                }
            }
        }
    }

    return variables;
}


std::vector<clause *> *instance::NEG1(std::set<int> *obstructions, int v){
    std::vector<clause*> *clauses = new std::vector<clause*>();  
    clause *c;
    std::set<int>::iterator var1, var2;
    
    // Generate two variables that are not used by the current instance
    int fresh_var1 = fresh(instance::vars);
    int fresh_var2 = fresh_var1 + 1;

    // Iterate through all pairs of variables in our instance
    for (var1 = instance::vars->begin(); var1 != instance::vars->end(); var1++){
        for (var2 = instance::vars->begin(); var2 != var1; var2++){
            // Make sure our variables are not not in the obstructions set
            if (obstructions->count(*var1) || obstructions->count(*var2)){
                continue;
            }

            // Create a new clause of the form var1 or var2 or (not obstruction)
            c = new clause(false, *var1, false, *var2, true, v);

            // Make sure this clause is not already in our instance
            if (instance::contains(c)){
                delete c;
                continue;
            }

            // If the clause is new then extend the instance
            clauses->push_back(c);
        }
    }

    // Iterate through all variables in our instance
    for (int var : *instance::vars){
        if (obstructions->count(var)){
            continue;
        }

        c = new clause(false, var, false, fresh_var1, true, v);
        clauses->push_back(c);
    }

    // Third we add all such clauses where both a and b are fresh variables
    c = new clause(false, fresh_var1, false, fresh_var2, true, v);
    clauses->push_back(c);
    return clauses;
}

std::vector<clause*> *instance::NEG2(std::set<int> * obstructions, int v1, int v2){
    std::vector<clause*> *clauses = new std::vector<clause*>();  
    clause *c;
    int fresh_var = fresh(instance::vars);
    for (int var : *instance::vars){
        if (obstructions->count(var)){
            continue;
        }

        c = new clause(false, var, true, v1, true, v2);
        if (instance::contains(c)){
            delete c;
            continue;
        }

        clauses->push_back(c);  
    }

    c = new clause(false, fresh_var, true, v1, true, v2);
    clauses->push_back(c);  
    return clauses;
}

std::vector<clause*> *instance::NEG3(std::set<int> * obstructions, int v1, int v2, int v3){
    std::vector<clause*> *clauses = new std::vector<clause*>();     
    clause *c = new clause(true, v1, true, v2, true, v3);
    if (instance::contains(c)){
        delete c;
        return clauses;
    }
    clauses->push_back(c); 
    return clauses;
}

std::vector<instance*> *instance::children(int hamming_radius){
    // List of children to be returned
    std::vector<instance*> *children = new std::vector<instance*>();

    // Set of variables obstructing the zero assignment from being a local max
    std::set<int> *obstructions = instance::obstructions(hamming_radius);

    std::vector<clause *> *clauses;

    std::set<int>::iterator v1, v2, v3;
 
    // If the zero assignment is indeed a local max then return an empty list
    if (obstructions->empty()){
        return children;
    }

    for (int v : *obstructions){
        clauses = instance::NEG1(obstructions, v);
        for (clause *c : *clauses){
            children->push_back(instance::extend(c));
        }
    }

    if (obstructions->size() < 2){
        return children;
    }

    // Iterate through all pairs of elements in the set
    for (v1 = obstructions->begin(); v1 != obstructions->end(); v1++){
        for (v2 = obstructions->begin(); v2 != v1; v2++){
            clauses = instance::NEG2(obstructions, *v1, *v2);
            for (clause *c : *clauses){
                children->push_back(instance::extend(c));
            }   
        }
    }

    if (obstructions->size() < 3){
        return children;
    }

    // Iterate through all triples of elements in the set
    for (v1 = obstructions->begin(); v1 != obstructions->end(); v1++){
        for (v2 = obstructions->begin(); v2 != v1; v2++){
            for (v3 = obstructions->begin(); v3 != v2; v3++){
                clauses = instance::NEG3(obstructions, *v1, *v2, *v3);
                for (clause *c : *clauses){
                    children->push_back(instance::extend(c));
                }   
            }
        }
    }

    return children;
}