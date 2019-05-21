#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include "Dataset.hpp"
#include "PSSMatrix.hpp"
#include "SVMModel.hpp"

int main(int argc, const char * argv[]) {
    
    if (argc != 3) {
        cout << "Usage : main_predict training_data_file test_data_file" << endl;
        exit(1);
    }

    Dataset trainset(argv[1]);
    int p = 10;
    int q = 2;

    // Setting parameters
    struct svm_parameter param;

    param.svm_type = C_SVC;
    param.kernel_type = RBF;
    param.degree = 3;
    param.gamma = 0.0078125;	// 1/num_features
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 32;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 0;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    
    SVMModel svmmodel(trainset, p, q, param);

    Dataset testset(argv[2], 0, -1, true);

    cout << testset.size() << endl;
    for (int i=0; i < testset.size(); i++) {
            Data data = testset.get(i);
            string sequence;
            for (int j = p; j<data.sequence.size()-q; j++) {
                sequence = data.sequence.substr(j-p, p+q);
                cout << sequence << endl;
                if (svmmodel.classify(sequence)) {
                    cout << "There is a cleavage site at index :" << j << " of entry " << i << "." << endl;
                }
            }
        }

    return 0;

}