#ifndef INSTANCE_H
#define INSTANCE_H
#include <set>
#include <vector>
#include "../clause/clause.hpp"

/*
An object representing an instance of the MAX3SAT problem. It primarly consists
of a list of 3-disjunctive clauses
*/
class instance {
    public:
        std::set<int> *variables;
        std::vector<clause*> *clauses;

    public:
        /*
        The list of clauses in the instance is needed upon initiation
        */
        instance(std::vector<clause*>* clauses);

        /*
        Return the number of clauses
        */
        int num_clauses();

        /*
        Check if a clause is in the instance
        */
        bool contains(clause *c);

        /* 
        Returns the number of clauses in the MAX3SAT instance satisfied under the
        zero assignment
        */
        int num_satisfied();

        /*
        Returns the number of clauses in the MAX3SAT instance satisfied under the
        zero assignment with variable i flipped to 1
        */
        int num_satisfied(int i);

        /*
        Returns the number of clauses in the MAX3SAT instance satisfied under the
        zero assignment with variables i and j flipped to 1
        */
        int num_satisfied(int i, int j);

        /*
        Returns the number of clauses in the MAX3SAT instance satisfied on the
        zero assignment with variables i, j, and k flipped to 1
        */
        int num_satisfied(int i, int j, int k);

        /*
        Print the instance
        */
        void print();
        
        /*
        Print a latex expression representing the list of clauses
        */
        void print_latex();

    public:
        /*
        Return a new MAX3SAT instance object with all the clauses from the current
        instance in addition to clause c
        */
        instance *extend(clause *c);

        /*
        Returns a set of at most t variables that when flipped will give
        a higher number of satisfied clauses than the zero assignment - these
        variables are obstructions to the zero assignment being a t-local max.
        Otherwise, if the zero assignment is a t-local maximum then the empty set
        is returned.
        */
        std::set<int> *obstructions(int t);

        /*
        See the manuscript 
        */
        std::vector<instance*> *children(int hamming_radius);
};

/*
Given a set of variables, create a variable not in the set
*/
int fresh_variable(std::set<int> *variables);

#endif