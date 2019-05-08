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
        Dataset(const char* file);
        ~Dataset();

        int size();
        Data get(int i);
        void show(bool verbose);

    private:
        std::vector<Data> datas;
};