
Dataset.o: Dataset.cpp
	g++ -c Dataset.cpp

PSSMatrix.o: PSSMatrix.cpp
	g++ -c PSSMatrix.cpp

SVMModel.o: SVMModel.cpp
	g++ -c SVMModel.cpp

test: test.cpp Dataset.o PSSMatrix.o
	g++ test.cpp Dataset.o PSSMatrix.o SVMModel.o svm.cpp -o test

clear:
	rm -f test.exe