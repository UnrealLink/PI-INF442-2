
Dataset.o: Dataset.cpp
	g++ -c Dataset.cpp

PSSMatrix.o: PSSMatrix.cpp
	g++ -c PSSMatrix.cpp

test: test.cpp Dataset.o PSSMatrix.o
	g++ test.cpp Dataset.o PSSMatrix.o -o test