Code for ranking-based semantics
===================================================

Author :
* Jérôme Delobelle (mailto:jerome.delobelle@u-paris.fr), Université Paris Cité

To compile the program (in C):
```
make clean
make Propa
```

Usage :
```
./propa <[CAT | BBS | DBS | FL]> <path_AF_file>
<[CAT | BBS | DBS | FL]> : Ranking-based semantics to apply where no parameter is needed. 
      [CAT represents the h-categorizer semantics,
      BBS represents the burden-based semantics,
      DBS represents the discussion-based semantics,
      FL represents the fuzzy labelling semantics]
      
./propa <[alphaBBS, CS]> <value> <path_AF_file> 
<[alphaBBS, CS]> : Ranking-based semantics to apply where one parameter is needed.
      [alphaBBS represents the alpha-burden-based semantics,
      CS represents the counting semantics]
<value> : represent the alpha value (between 0 exclu and +∞) for alphaBBS and the damping factor for CS (between 0 (exclu) and 1 (exclu)).  

./propa <[PROPA1, PROPA2, PROPA3]> <epsilon> <delta> <[set|mset]> <path_AF_file> 
<[PROPA1, PROPA2, PROPA3]> : Ranking-based semantics to apply where one parameter is needed.
      [PROPA1 represents the propagation semantics (Propa_{epsilon}),
      PROPA2 represents the propagation semantics (Propa_{epsilon + 1}),
      PROPA3 represents the propagation semantics (Propa_{epsilon -> 1})]
<epsilon> : represents the epsilon value (between 0 included and 1 included). 
<delta> : represents the delta value (between 0 excluded and 1 excluded). 
<[set|mset]> : represents the choice between the use of set of multiset.

<path_AF_file> : Path of the file containing the argumentation graph (apx format).
```

For instance, the following command line allows the user to apply the h-categorizer semantics on an argumentation framework (stored in the file called *debate_graph_star_6_arg_8_6997.apx*):

```
./propa CAT graph/debategraph_bench/debate_graph_star_6_arg_8_6997.apx
```
