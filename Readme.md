This repository hosts the code for a computer-assisted proof for a result on MAX3SAT approximation using local search.

## The 1-local case


## The 2-local case

If the zero assignment is not a `2`-local maximum then there must exist a non-empty
set `H` of at most `2` elements and a clause `C` that was satisfied under the zero assignment
and becomes unsatisfied when all the variables in `H` are flipped to `1`. Since `C` is satisfied under the zero assignment, it must contain at least one negation. Furthermore, we may suppose without loss of generality that `C` contains at most two negations.

First suppose `H = {i}`. Then `C` must be of the form 

```
a or b or (not xi),
``` 

where `a` and `b` are literals with no negations and don't have the variable `xi`. 

Otherwise, suppose `H = {i, j}`. Then `C` must be of the following forms 
```
a or b or (not xi)
a or b or (not xj)
a or (not xi) or (not xj)
```

Hence, we can apply the following procedure

* For any variable `v` in `H` add all clauses of the form `a or b or (not v)`
* If `H = {i, j}` then add all clauses of the form `a or (not i) or (not j)`

where `a` and `b` are literals with no negations and no variable in `H`. 

## The 3-local case

0 or 1 or 2
not 0 or 1 or 2
not 1 or 2 or 3
0 or not 2 or 4