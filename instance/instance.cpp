#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include "instance.hpp"

/*
The list of clauses in the instance is needed upon initiation
*/
instance::instance(std::vector<clause *> *clauses){
    instance::vars = new std::set<int>();
    instance::clauses = clauses;
    for (clause *c : *clauses){
        for (int variable : c->variables){
            instance::vars->insert(variable);
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
        std::cout << "&" << c->latex() << "\\\\\n";
    }
}

int fresh(std::set<int> *variables){
    if (!variables->empty()){
        return *variables->rbegin() + 1;
    }
    return 0;
}