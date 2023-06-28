#include <assert.h>
#include <iostream>
#include "instance.hpp"

int main(){    
    std::vector<clause *> clauses1;
    clauses1.push_back(new clause(false, 2, false, 1, false, 3));
    clauses1.push_back(new clause(true,  1, true,  2, false, 3));
    
    instance *maxsat1 = new instance(&clauses1);
    assert(maxsat1->num_satisfied(1) == 2);
    assert(maxsat1->num_satisfied(2) == 2);
    assert(maxsat1->num_satisfied(3) == 2);
    assert(maxsat1->num_satisfied(1, 2) == 1);
    assert(maxsat1->num_satisfied(1, 3) == 2);
    assert(maxsat1->num_satisfied(2, 3) == 2);
    assert(maxsat1->num_satisfied() == 1);

    instance *maxsat_extended = maxsat1->extend(new clause(false, 2, true, 1, false, 4));
    assert(maxsat_extended->num_clauses() == maxsat1->num_clauses() + 1);
    assert(maxsat_extended->variables->size() == maxsat1->variables->size() + 1);

    std::vector<clause *> clauses2;
    clauses2.push_back(new clause(false, 0, false, 1, false, 2));
    clauses2.push_back(new clause(true,  0, false, 1, false, 2));
    clauses2.push_back(new clause(false, 0, true,  1, false, 2));
    clauses2.push_back(new clause(false, 0, false, 1, true,  2));
    
    instance *maxsat2 = new instance(&clauses2);
    assert(maxsat2->obstructions(1)->empty());
    assert(!maxsat2->obstructions(2)->empty());

    std::vector<clause *> clauses3;
    clauses3.push_back(new clause(false, 0, false, 1, false, 2));
    clauses3.push_back(new clause(true,  0, false, 1, false, 2));
    clauses3.push_back(new clause(false, 0, true,  1, false, 2));
    clauses3.push_back(new clause(false, 0, false, 1, true,  2));
    clauses3.push_back(new clause(false, 0, true,  1, true,  2));
    clauses3.push_back(new clause(true,  0, false, 1, true,  2));
    clauses3.push_back(new clause(true,  0, true,  1, false, 2));
    
    instance *maxsat3 = new instance(&clauses3);
    assert(maxsat3->obstructions(1)->empty());
    assert(maxsat3->obstructions(2)->empty());

    std::vector<clause *> clauses4; 
    clauses4.push_back(new clause(false, 0, false, 1, false, 2));
    clauses4.push_back(new clause(true,  0, false, 1, false, 3));
    clauses4.push_back(new clause(true,  1, false, 2, false, 3));
    clauses4.push_back(new clause(false, 0, true,  2, false, 3));
    clauses4.push_back(new clause(true,  3, false, 4, false, 5));
    
    instance *maxsat4 = new instance(&clauses4);
    assert(maxsat4->obstructions(2)->empty()); 

    std::set<int> variables;
    assert(fresh_variable(&variables) == 0);
    variables.insert(0);
    variables.insert(1);
    variables.insert(2);
    assert(fresh_variable(&variables) == 3);
    variables.insert(4);
    assert(fresh_variable(&variables) == 5);

    return 0;
}