#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include "Dataset.hpp"
#include "svm.h"

class SVMModel {
    public:
        SVMModel(Dataset dataset, int p, int q, int kernel_type = RBF, double C = 32, double gamma=0.0078125, const char* matrix="BLOSUM62");
        ~SVMModel();

        bool classify(string sequence);

    private:
        int p;
        int q;
        struct svm_problem datasetToProblem (Dataset dataset);
        struct svm_problem datasetToProblemSubstitution (Dataset dataset, const char* matrix);
        double substitutionMatrix(string a, string b);
        void substitutionMatrixInit(const char* file="BLOSUM62");
        map<char, map<char, double> > matrix;
        double matrix_max = 0; // to scale data for svm
        struct svm_parameter param;
        struct svm_model* model;
};