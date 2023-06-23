#ifndef CLAUSE_H
#define CLAUSE_H

// A clause is a 3-disjunctive clause (i.e the disjunction of three literals).
// A literal is a variable or its negation
class clause
{
    // Attributes
    public:
        // An array storing the indices of the variables
        int variables[3];

        // Whether or not a literal is negated. For example, a clause object 
        // where variables = {1, 2, 3} and negated = {false, true, false}
        // represents the clause (x1) or (not x2) or (x3) 
        bool negated[3];
    
    // Methods
    public:
        clause(bool n1, int v1, bool n2, int v2, bool n3, int v3);

        bool eval();

        // Flip variable i and evaluate the clause
        bool eval(int i);

        // Flip variables i and j and evaluate the clause
        bool eval(int i, int j);

        // Flip variables i, j, and k and evaluate the clause
        bool eval(int i, int j, int k);
};

bool clauses_equal(clause *c1, clause *c2);

#endif