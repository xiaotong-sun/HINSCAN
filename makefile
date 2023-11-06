all: hinscan

hinscan: .obj/main.o .obj/SCAN.o .obj/MetaPath.o .obj/DataReader.o .obj/HomoGraphBuilder.o
	g++ .obj/main.o .obj/SCAN.o .obj/MetaPath.o .obj/DataReader.o .obj/HomoGraphBuilder.o -I ./include -o hinscan

.obj/main.o: main.cpp
	g++ -c -O3 -std=c++20 -I ./include -o .obj/main.o main.cpp

.obj/SCAN.o: ./src/baseline/SCAN.cpp
	g++ -c -O3 -std=c++20 -I ./include -o .obj/SCAN.o ./src/baseline/SCAN.cpp

.obj/MetaPath.o: ./src/bean/MetaPath.cpp
	g++ -c -O3 -std=c++20 -I ./include -o .obj/MetaPath.o ./src/bean/MetaPath.cpp

.obj/DataReader.o: ./src/util/DataReader.cpp
	g++ -c -O3 -std=c++20 -I ./include -o .obj/DataReader.o ./src/util/DataReader.cpp

.obj/HomoGraphBuilder.o: ./src/util/HomoGraphBuilder.cpp
	g++ -c -O3 -std=c++20 -I ./include -o .obj/HomoGraphBuilder.o ./src/util/HomoGraphBuilder.cpp

clean:
	rd /s /q .obj
	mkdir .obj