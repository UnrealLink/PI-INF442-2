#pragma once
#include "Dataset.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

class PSSMatrix {
    public:
        PSSMatrix(Dataset dataset, int p, int q, double threshold=75);
        ~PSSMatrix();

        double score(string sequence);
        bool classify(string sequence);
        void optimize_threshold(double start, double end, double step);

    private:
        int p, q;
        map<char, vector<double> > w; // w[a][i+p] weights of amino acid a at relative position i;
        double threshold;
        void optimize_threshold(Dataset dataset, double start, double end, double step);
};