#pragma once
#include "Dataset.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

class PSSMatrix {
    public:
        PSSMatrix(Dataset dataset, int p, int q, double threshold=74);
        ~PSSMatrix();

        double score(string sequence);
        bool classify(string sequence);

    private:
        int p, q;
        map<char, vector<double> > w; // w[a][i+p] weights of amino acid a at relative position i;
        double threshold;
};