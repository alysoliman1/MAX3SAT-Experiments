#include <algorithm>
#include <iostream>
#include <format>
#include "clause.h"

clause::clause(bool n1, int v1, bool n2, int v2, bool n3, int v3){
    // Buffer to store pre-sorted variables and negations
    int variables[3] = {v1, v2, v3};
    int negated[3] = {n1, n2, n3};

    // Populate the variables attribute
    for (int i = 0; i < 3; i++){
        clause::variables[i] = variables[i];
    }

    // Next, we "normalize" the object. We simply sort the variables by ascending
    // order of indices. For example, the clause (x2) or (not x1) or (x3) will
    // become (not x1) or (x2) or (x3) when normalized.

    // Sort the variables attribute in place by ascending order of indices
    std::sort(std::begin(clause::variables), std::end(clause::variables));

    // Match the variables to their proper negations
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (clause::variables[i] == variables[j]){
                clause::negated[i] = negated[j];
            }
        }
    }
}

std::string clause::latex(){  
    std::string output;  
    for (int i = 0; i < 3; i++){
        output += (clause::negated[i] ? "\\neg " : "");
        output +=  "x_";
        output += std::to_string(clause::variables[i]);
        if (i == 2){
            continue;
        }
        output += " \\lor ";
    }
    return output;
}

void clause::print(){
    std::cout << clause::latex() << "\n";
}
 
bool clause::eval(){
    return clause::eval(-1, -1, -1);
}

bool clause::eval(int i){
    return clause::eval(i, -1, -1);
}

bool clause::eval(int i, int j){
    return clause::eval(i, j, -1);
}

bool clause::eval(int i, int j, int k){
    int var;
    bool negated;
    for (int t = 0; t < 3; t++){
        var = clause::variables[t];
        negated = clause::negated[t];

        // If the variable has been flipped
        if (var == i || var == j || var == k){
            if (negated){
                continue;
            }

            // If the variable has been flipped and it's not negated
            return true;
        }
        
        // If the variable has not been flipped and it's negated
        if (negated){
            return true;
        }
    }

    return false;
}

bool clauses_equal(clause *c1, clause *c2){
    // The following procedure works since the clauses are "normalized" - see
    // the function body of the class
    for (int i = 0; i < 3; i++){
        // Variables don't match
        if (c1->variables[i] != c2->variables[i]){
            return false;
        }

        // Negations don't match
        if (c1->negated[i] != c2->negated[i]){
            return false;
        }
    }
    return true;
}