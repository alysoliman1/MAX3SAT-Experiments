#include <assert.h>
#include <iostream>
#include "instance.h"

int main(){
    clause *clauses[2] = {
        new clause(false, 2, false, 1, false, 3),
        new clause(true, 1, true, 2, false, 3)
    };
    
    instance *maxsat = new instance(clauses, 2);
    assert(maxsat->num_satisfied(1) == 2);
    assert(maxsat->num_satisfied(2) == 2);
    assert(maxsat->num_satisfied(3) == 2);
    assert(maxsat->num_satisfied(1, 2) == 1);
    assert(maxsat->num_satisfied(1, 3) == 2);
    assert(maxsat->num_satisfied(2, 3) == 2);
    assert(maxsat->num_satisfied() == 1);

    clause *c = new clause(false, 2, true, 1, false, 4);
    instance *maxsat_extended = maxsat->extend(c);
    assert(maxsat_extended->num_clauses == maxsat->num_clauses + 1);
    assert(maxsat_extended->variables.size() == maxsat->variables.size() + 1);

    clause *clauses2[4] = {
        new clause(false, 0, false, 1, false, 2),
        new clause(true, 0, false, 1, false, 2),
        new clause(false, 0, true, 1, false, 2),
        new clause(false, 0, false, 1, true, 2),
    };

    instance *maxsat2 = new instance(clauses2, 4);
    assert(maxsat2->obstructions(1).empty());
    assert(!maxsat2->obstructions(2).empty());

    clause *clauses3[7] = {
        new clause(false, 0, false, 1, false, 2),
        new clause(true, 0, false, 1, false, 2),
        new clause(false, 0, true, 1, false, 2),
        new clause(false, 0, false, 1, true, 2),
        new clause(false, 0, true, 1, true, 2),
        new clause(true, 0, false, 1, true, 2),
        new clause(true, 0, true, 1, false, 2)
    };

    instance *maxsat3 = new instance(clauses3, 7);
    assert(maxsat3->obstructions(1).empty());
    assert(maxsat3->obstructions(2).empty());

    std::set<int> variables;
    assert(fresh_variable(variables) == 0);
    variables.insert(0);
    variables.insert(1);
    variables.insert(2);
    assert(fresh_variable(variables) == 3);
    variables.insert(4);
    assert(fresh_variable(variables) == 5);

    clause *clauses4[5] = {
        new clause(false, 0, false, 1, false, 2),
        new clause(true, 0, false, 1, false, 3),
        new clause(true, 1, false, 2, false, 3),
        new clause(false, 0, true, 2, false, 3),
        new clause(true, 3, false, 4, false, 5),
    };

    instance *maxsat4 = new instance(clauses4, 5);
    assert(maxsat4->obstructions(2).empty()); 

    return 0;
}