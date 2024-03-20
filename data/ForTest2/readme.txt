The graph in the folder is used for testing if the program can run correctly, when there is a vertex
does not connect to T. (A-P-T-P-A)
 
This can also test when the index of the type of edge/vertex is not begin with 0, whether the program
can run correctly.
---
vertex type
1
2
3

edge type
1-2: 1
2-3: 2
3-2: 3
2-1: 4
---

./hinscan data/ForTest2/ 0.3 1 0 "1 1 2 4 1" homoGraph_short.txt 0
./hinscan data/ForTest2/ 0.3 1 0 "1 1 2 2 3 3 2 4 1" homoGraph_long.txt 0