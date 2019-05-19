#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Dataset.hpp"
#include "svm.h"

class SVMModel {
    public:
        SVMModel(Dataset dataset, int p, int q, int kernel_type = RBF, double C = 32, double gamma=0.0078125);
        ~SVMModel();

        bool classify(string sequence);

    private:
        int p;
        int q;
        struct svm_problem datasetToProblem (Dataset dataset, int p, int q);
        struct svm_parameter param;
        struct svm_model* model;
};
