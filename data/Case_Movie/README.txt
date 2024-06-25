This is used for case study.
--------------
Vertex Type

<Actor>: 0
<Movie>:  1
<Type>:  2


--------------
Edge Type

<Actor -> Movie>: 0
<Movie -> Actor>: 1
<Movie -> Type>:  2
<Type -> Movie>:  3

--------------
Meta Path
<Actor -> Movie -> Actor>
0 - 0 - 1 - 1 - 0
args: eps = 0.75, mu = 3, mode = 0/1.

./executable/hinscan ./data/Case_Movie/ 0.75 3 0 "0 0 1 1 0" homoGraph.txt 0