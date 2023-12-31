#include <assert.h>
#include "clause.hpp"
#include <iostream>

int main(){
    clause *c = new clause(false, 2, true, 1, false, 3);
    assert(c->variables[0] == 1);
    assert(c->negations[0] == true);
    assert(c->variables[1] == 2);
    assert(c->negations[1] == false);
    assert(c->variables[2] == 3);
    assert(c->negations[2] == false);
    delete c;

    c = new clause(false, 1, false, 2, false, 3);
    assert(!c->eval());
    assert(c->eval(1));
    assert(c->eval(2));
    assert(c->eval(3));
    assert(c->eval(1, 2));
    assert(c->eval(1, 3));
    assert(c->eval(2, 3));
    delete c;

    c = new clause(false, 1, true, 2, false, 3);
    assert(c->eval());
    assert(c->eval(1));
    assert(!c->eval(2));
    assert(c->eval(3));
    assert(c->eval(1, 2));
    assert(c->eval(1, 3));
    assert(c->eval(2, 3));
    delete c;

    clause *c1 = new clause(false, 1, false, 3, true, 2);
    clause *c2 = new clause(false, 3, true, 2, false, 1);
    assert(clauses_equal(c1, c2));
    delete c1;
    delete c2;

    c1 = new clause(true, 1, false, 2, false, 3);
    c2 = new clause(true, 1, false, 2, true, 3);
    assert(!clauses_equal(c1, c2));
    delete c1; 
    delete c2;

    c = new clause(true, 1, false, 2, false, 3);
    assert(c->latex() == "\\neg x_1 \\lor x_2 \\lor x_3");
}