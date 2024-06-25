This is used for case study.
--------------
Vertex Type

<Manufacture>: 0
<Product>:  1
<Subclass>:  2
<Class>: 3


--------------
Edge Type

<Manufacture -> Product>: 0
<Product -> Manufacture>: 1
<Product -> Subclass>:  2
<Subclass -> Product>:  3
<Subclass -> Class>: 4
<Class -> Subclass>: 5

--------------
Meta Path
<Manufacture -> Product -> Subclass -> Product -> Manufacture>
0 - 0 - 1 - 2 - 2 - 3 - 1 - 1 - 0
args: eps = 0.75, mu = 3, mode = 0/1.

Common: SONY is a hub
Disjoin: SONY is a hub, Apple is a outlier, SUMSUNG is a outlier.

./executable/hinscan ./data/Case_Product/ 0.75 3 1 "0 0 1 2 2 3 1 1 0" homo.txt 2