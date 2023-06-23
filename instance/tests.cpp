#include <assert.h>
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
    assert(maxsat2->local_max_obstruction(1).empty());
    assert(!maxsat2->local_max_obstruction(2).empty());

    return 0;
}