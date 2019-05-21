#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include "Dataset.hpp"
#include "PSSMatrix.hpp"
#include "SVMModel.hpp"

int main(int argc, const char * argv[]){

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <0 (for Dataset) | 1 (for PSSMatrix) | 2 (for SVM) | 3 (for SVM with substitution matrix)>" << std::endl;
        return 1;
    }

    int option = atoi(argv[1]);

    // DATASET TEST
    
    if (option == 0) {

        Dataset dataset("../Data/EUKSIG_13.red");
        
        dataset.show(true);

    }

    // PSSMATRIX TEST

    if (option == 1) {

        Dataset trainset("../Data/SIG_13.red", 0, 1200);
        int p = 10;
        int q = 2;
        PSSMatrix pssmatrix(trainset, p, q);

        Dataset testset("../Data/SIG_13.red", 1200, -1);
        double tp = 0;
        double fp = 0;
        double tn = 0;
        double fn = 0;
        Data data;
        string sequence;
        
        for (int i=0; i < testset.size(); i++) {
            Data data = testset.get(i);
            string sequence;
            for (int j = p; j<data.sequence.size()-q; j++) {
                sequence = data.sequence.substr(j-p, p+q);
                if (pssmatrix.classify(sequence)) {
                    if (j == data.cleavage) {tp++;} else {fp++;}
                } else {
                    if (j == data.cleavage) {fn++;} else {tn++;}
                }
            }
        }

        cout << tp << ", " << fp << ", " << tn << ", " << fn << endl;
        double precision = tp/(tp+fp);
        double recall = tp/(tp+fn);

        cout << "Precision : " << precision << endl;
        cout << "Recall : " << recall << endl;
        cout << "FScore : " << (2*precision*recall)/(precision+recall) << endl;

    }

    if (option == 2) {
        
        Dataset trainset("../Data/SIG_13.red", 0, 1200);
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

        Dataset testset("../Data/SIG_13.red", 1200, -1);
        double tp = 0;
        double fp = 0;
        double tn = 0;
        double fn = 0;
        Data data;
        string sequence;

        for (int i=0; i < testset.size(); i++) {
            Data data = testset.get(i);
            string sequence;
            for (int j = p; j<data.sequence.size()-q; j++) {
                sequence = data.sequence.substr(j-p, p+q);
                if (svmmodel.classify(sequence)) {
                    if (j == data.cleavage) {tp++;} else {fp++;}
                } else {
                    if (j == data.cleavage) {fn++;} else {tn++;}
                }
            }
        }

        cout << tp << ", " << fp << ", " << tn << ", " << fn << endl;
        double precision = tp/(tp+fp);
        double recall = tp/(tp+fn);

        cout << "Precision : " << precision << endl;
        cout << "Recall : " << recall << endl;
        cout << "FScore : " << (2*precision*recall)/(precision+recall) << endl;
        
    }

    if (option == 3) {
        Dataset trainset("../Data/SIG_13.red", 0, 100);
        int p = 10;
        int q = 2;

        // Setting parameters
        struct svm_parameter param;


        param.svm_type = C_SVC;
        param.kernel_type = PRECOMPUTED;
        param.degree = 3;
        param.gamma = 0.0078125;	// 1/num_features
        param.coef0 = 0;
        param.nu = 0.5;
        param.cache_size = 100;
        param.C = 1.2;
        param.eps = 1e-3;
        param.p = 0.1;
        param.shrinking = 1;
        param.probability = 0;
        param.nr_weight = 0;
        param.weight_label = NULL;
        param.weight = NULL;
        
        SVMModel svmmodel(trainset, p, q, param);

        Dataset testset("../Data/SIG_13.red", 100, 120);
        double tp = 0;
        double fp = 0;
        double tn = 0;
        double fn = 0;
        Data data;
        string sequence;

        for (int i=0; i < testset.size(); i++) {
            Data data = testset.get(i);
            string sequence;
            for (int j = p; j<data.sequence.size()-q; j++) {
                sequence = data.sequence.substr(j-p, p+q);
                if (svmmodel.classify(sequence)) {
                    if (j == data.cleavage) {tp++;} else {fp++;}
                } else {
                    if (j == data.cleavage) {fn++;} else {tn++;}
                }
            }
        }

        cout << tp << ", " << fp << ", " << tn << ", " << fn << endl;
        double precision = tp/(tp+fp);
        double recall = tp/(tp+fn);

        cout << "Precision : " << precision << endl;
        cout << "Recall : " << recall << endl;
        cout << "FScore : " << (2*precision*recall)/(precision+recall) << endl;

    }

	return 0;
}


