
Dataset.o: Dataset.cpp
	g++ -c Dataset.cpp

PSSMatrix.o: PSSMatrix.cpp
	g++ -c PSSMatrix.cpp

SVMModel.o: SVMModel.cpp
	g++ -c SVMModel.cpp

test: test.cpp Dataset.o PSSMatrix.o SVMModel.o
	g++ test.cpp Dataset.o PSSMatrix.o SVMModel.o svm.cpp -o test

main_train: main_train.cpp Dataset.o PSSMatrix.o SVMModel.o
	g++ main_train.cpp Dataset.o PSSMatrix.o SVMModel.o svm.cpp -o main_train

main_predict: main_predict.cpp Dataset.o PSSMatrix.o SVMModel.o
	g++ main_predict.cpp Dataset.o PSSMatrix.o SVMModel.o svm.cpp -o main_predict

all: test.cpp main_train.cpp main_predict.cpp Dataset.cpp PSSMatrix.cpp SVMModel.cpp
	g++ -c Dataset.cpp
	g++ -c PSSMatrix.cpp
	g++ -c SVMModel.cpp
	g++ test.cpp Dataset.o PSSMatrix.o SVMModel.o svm.cpp -o test
	g++ main_train.cpp Dataset.o PSSMatrix.o SVMModel.o svm.cpp -o main_train
	g++ main_predict.cpp Dataset.o PSSMatrix.o SVMModel.o svm.cpp -o main_predict

clear:
	rm -f test.exe  main_train.exe main_predict.exe 