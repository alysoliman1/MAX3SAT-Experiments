#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include "instance.hpp"

/*
The list of clauses in the instance is needed upon initiation
*/
instance::instance(std::vector<clause *> *clauses){
    instance::variables = new std::set<int>();
    instance::clauses = clauses;
    for (clause *c : *clauses){
        for (int variable : c->variables){
            instance::variables->insert(variable);
        }
    }
}

/*
Return the number of clauses
*/
int instance::num_clauses(){
    return instance::clauses->size();
}

/*
Check if a clause is in the instance
*/
bool instance::contains(clause *c){
    for (clause *_c : *instance::clauses){
        if (clauses_equal(c, _c)){
            return true;
        }
    }
    return false;
}

/*
Returns the number of clauses in the MAX3SAT instance satisfied on the
zero assignment with variables i, j, and k flipped to 1
*/
int instance::num_satisfied(int i, int j, int k){
    int satisfied = 0;
    for (clause *c : *instance::clauses){
        if (c->eval(i, j, k)){
            satisfied += 1;
        }
    }
    return satisfied;
}

/* 
Returns the number of clauses in the MAX3SAT instance satisfied under the
zero assignment
*/
int instance::num_satisfied(){
    return instance::num_satisfied(-1, -1, -1);
}

/*
Returns the number of clauses in the MAX3SAT instance satisfied under the
zero assignment with variable i flipped to 1
*/
int instance::num_satisfied(int i){
    return instance::num_satisfied(i, -1, -1);
}

/*
Returns the number of clauses in the MAX3SAT instance satisfied under the
zero assignment with variables i and j flipped to 1
*/
int instance::num_satisfied(int i, int j){
    return instance::num_satisfied(i, j, -1);
}

/*
Print the instance
*/
void instance::print(){
    for (clause *c : *instance::clauses){
        c->print();
    } 
}

/*
Print a latex expression representing the list of clauses
*/
void instance::print_latex(){
    for (clause *c : *instance::clauses){
        c->print_latex();
    }
}

/*
Return a new MAX3SAT instance object with all the clauses from the current
instance in addition to clause c
*/
instance *instance::extend(clause *c){
    std::vector<clause *> *extended_clauses = new std::vector<clause *>();
    for (clause *_c : *instance::clauses){
        extended_clauses->push_back(_c);
    }
    extended_clauses->push_back(c);
    return new instance(extended_clauses);
}

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
    for (int var : *instance::variables){
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
    for (var1 = instance::variables->begin(); var1 != instance::variables->end(); var1++){
        for (var2 = instance::variables->begin(); var2 != var1; var2++){
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
    for (var1 = instance::variables->begin(); var1 != instance::variables->end(); var1++){
        for (var2 = instance::variables->begin(); var2 != var1; var2++){
            for (var3 = instance::variables->begin(); var3 != var2; var3++){
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

std::vector<instance*> *instance::children(int hamming_radius){
    // List of children to be returned
    std::vector<instance*> *children = new std::vector<instance*>();

    // Set of variables obstructing the zero assignment from being a local max
    std::set<int> *obstructions = instance::obstructions(hamming_radius);
 
    // If the zero assignment is indeed a local max then return an empty list
    if (obstructions->empty()){
        return children;
    }

    // Buffer variables to be used later
    clause *c;
    std::set<int>::iterator var1, var2;
    std::set<int>::iterator obs1, obs2, obs3;

    // Generate two variables that are not used by the current instance
    int fresh_var1 = fresh_variable(instance::variables);
    int fresh_var2 = fresh_var1 + 1;


    // Extend our instance by clauses of the form a or b or (not obstruction)
    // Where a and b are variable not in the obstructions set
    // Iterate through the obstructing variables
    for (int obstruction : *obstructions){
        // First we add all such clauses where a and b are variables in our instance
        // Iterate through all pairs of variables in our instance
        for (var1 = instance::variables->begin(); var1 != instance::variables->end(); var1++){
            for (var2 = instance::variables->begin(); var2 != var1; var2++){
                // Make sure our variables are not not in the obstructions set
                if (obstructions->count(*var1) || obstructions->count(*var2)){
                    continue;
                }

                // Create a new clause of the form var1 or var2 or (not obstruction)
                c = new clause(false, *var1, false, *var2, true, obstruction);

                // Make sure this clause is not already in our instance
                if (instance::contains(c)){
                    delete c;
                    continue;
                }

                // If the clause is new then extend the instance
                children->push_back(instance::extend(c));
            }
        }

        // Second we add all such clauses where a is in our instance but b
        // is a fresh variable
        // Iterate through all variables in our instance
        for (int var : *instance::variables){
            if (obstructions->count(var)){
                continue;
            }

            c = new clause(false, var, false, fresh_var1, true, obstruction);
            children->push_back(instance::extend(c));
        }

        // Third we add all such clauses where both a and b are fresh variables
        c = new clause(false, fresh_var1, false, fresh_var2, true, obstruction);
        children->push_back(instance::extend(c)); 
    }

    if (obstructions->size() < 2){
        return children;
    }

    // Iterate through all pairs of elements in the set
    for (obs1 = obstructions->begin(); obs1 != obstructions->end(); obs1++){
        for (obs2 = obstructions->begin(); obs2 != obs1; obs2++){
            for (int var : *instance::variables){
                if (obstructions->count(var)){
                    continue;
                }

                c = new clause(false, var, true, *obs1, true, *obs2);
                if (instance::contains(c)){
                    delete c;
                    continue;
                }

                children->push_back(instance::extend(c));  
            }

            c = new clause(false, fresh_var1, true, *obs1, true, *obs2);
            children->push_back(instance::extend(c));  
        }
    }

    if (obstructions->size() < 3){
        return children;
    }

    obs1 = obstructions->begin();
    obs2 = obstructions->begin()++;
    obs3 = (obstructions->begin()++)++;
    c = new clause(true, *obs1, true, *obs2, true, *obs3);
    if (instance::contains(c)){
        delete c;
        return children;
    }
    children->push_back(instance::extend(c)); 

    return children;
}

int fresh_variable(std::set<int> *variables){
    if (!variables->empty()){
        return *variables->rbegin() + 1;
    }
    return 0;
}