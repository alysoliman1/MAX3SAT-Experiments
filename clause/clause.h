#ifndef CLAUSE_H
#define CLAUSE_H

// A clause is a 3-disjunctive clause (i.e the disjunction of three literals).
class clause
{
    // Attributes
    public:
        // An array storing the indices of the variables
        int variables[3];

        /*
        Whether or not a literal is negated. For example, a clause object 
        where variables = {1, 2, 3} and negated = {false, true, false}
        represents the clause (x1) or (not x2) or (x3) 
        */
        bool negated[3];
    
    // Methods
    public:
        clause(bool n1, int v1, bool n2, int v2, bool n3, int v3);

        void print();

        /*
        Evaluate the clause under the zero assignment
        */
        bool eval();

        /*
        Evaluate the clause under the zero assignment with xi flipped
        */ 
        bool eval(int i);

        /*
        Evaluate the clause under the zero assignment with xi and xj flipped
        */ 
        bool eval(int i, int j);

        /*
        Evaluate the clause under the zero assignment with xi, xj, and xk flipped
        */ 
        bool eval(int i, int j, int k);
};

/*
Determine if two clauses are semantically the same
*/
bool clauses_equal(clause *c1, clause *c2);

#endif