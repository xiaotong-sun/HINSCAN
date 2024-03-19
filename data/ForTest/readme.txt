The files in this folder are used to test whether the SCAN program is running correctly when the
vertex id of target type is not consecutive.

---
vertex type
0
1
2

edge type
0-1: 0
1-2: 1
2-1: 2
1-0: 3

---
./hinscan ./data/ForTest/ 0.5 2 0 "0 0 1 3 0" homoGraph_short.txt 0
./hinscan ./data/ForTest/ 0.5 2 0 "0 0 1 1 2 2 1 3 0" homoGraph_long.txt 0
