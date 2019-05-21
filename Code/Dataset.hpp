#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <assert.h>
#include <vector>
#include <string>

using namespace std;

struct Data {
    string sequence;
    int cleavage;
};


class Dataset {
    public:
        Dataset(const char* file, int start=0, int end=-1, bool test=false);
        ~Dataset();

        int size();
        Data get(int i);
        void show(bool verbose);
        void writeForSVM(const char* file, int p, int q);

    private:
        std::vector<Data> datas;
};