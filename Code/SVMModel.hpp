#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include "Dataset.hpp"
#include "svm.h"

class SVMModel {
    public:
        SVMModel(Dataset dataset, int p, int q, struct svm_parameter param, const char* matrix_file=NULL);
        ~SVMModel();

        bool classify(string sequence);

    private:
        int p;
        int q;
        struct svm_problem datasetToProblem (Dataset dataset);
        struct svm_problem datasetToProblemSubstitution (Dataset dataset, const char* matrix_file);
        double substitutionMatrix(string a, string b);
        void substitutionMatrixInit(const char* file=NULL);
        map<char, map<char, double> > matrix;
        double matrix_max = 0; // to scale data for svm
        struct svm_parameter param;
        struct svm_model* model;
};