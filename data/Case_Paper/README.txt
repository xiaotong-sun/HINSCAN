This is used for case study.
--------------
Vertex Type

<Author>: 0
<Paper>:  1
<Topic>:  2


--------------
Edge Type

<Author -> Paper>: 0
<Paper -> Author>: 1
<Paper -> Topic>:  2
<Topic -> Paper>:  3

--------------
Meta Path
<Author -> Paper -> Author>
0 - 0 - 1 - 1 - 0
args: eps = 0.64, mu = 3, mode = 0/1.

// <Author -> Paper -> Topic -> Paper -> Author>
// 0 - 0 - 1 - 2 - 2 - 3 - 1 - 1 - 0

./executable/hinscan ./data/Case_Paper/ 0.64 3 1 "0 0 1 1 0" homo.txt 1 5

for pathsim situation
eps = 0.52