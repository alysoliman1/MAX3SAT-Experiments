#ifndef INSTANCE_H
#define INSTANCE_H
#include <set>
#include "../clause/clause.h"

/*
An object representing an instance of the MAX3SAT problem
*/
class instance {
    // Attributes
    public:
        int num_variables;
        int num_clauses;
        clause **clauses;

    // Methods
    public:
        instance(int num_variables, int num_clauses, clause **clauses);

        /*
        Check if a clause is in the instance
        */
       bool contains(clause *c);

        /*
        Return a new MAX3SAT instance object with all the clauses from the current
        instance in addition to clause c
        */
        instance *extend(clause *c);

        /* 
        Returns the number of clauses in the MAX3SAT instance satisfied on the
        zero assignment
        */
        int num_satisfied();

        /*
        Returns the number of clauses in the MAX3SAT instance satisfied on the
        zero assignment with variable (xi) flipped to 1
        */
        int num_satisfied(int i);

        /*
        Returns the number of clauses in the MAX3SAT instance satisfied on the
        zero assignment with variable (xi) and (xj) flipped to 1
        */
        int num_satisfied(int i, int j);

        /*
        Returns the number of clauses in the MAX3SAT instance satisfied on the
        zero assignment with variables (xi), (xj), and (xk) flipped to 1
        */
        int num_satisfied(int i, int j, int k);

        /*
        Returns a set of at most t variables that when flipped will give
        a higher number of satisfied clauses than the zero assignment.
        Otherwise, if the zero assignment is a t-local maximum then the empty set
        is returned
        */
        std::set<int> local_max_obstruction(int t);
};

#endif