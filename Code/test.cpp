#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "Dataset.hpp"
#include "PSSMatrix.hpp"

int main(int argc, const char * argv[]){

    // DATASET TEST

	Dataset dataset("../Data/EUKSIG_13.red");
    
	dataset.show(false);

    // PSSMATRIX TEST

    /* 
    MASKATLLLAFTLLFATCIARHQQRQQQQNQCQLQNIEALEPIEVIQAEA
    SSSSSSSSSSSSSSSSSSSSCMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    */

    int p = 10;
    int q = 2;
    PSSMatrix pssmatrix(dataset, p, q);

    Data data = dataset.get(1);
    string sequence;
    for (int i = p; i<data.sequence.size()-q; i++) {
        sequence = data.sequence.substr(i-p, p+q);
        cout<<"Sequence "<< sequence;
        if (i == data.cleavage) cout<<" contains cleavage site";
        cout<<". Score :"<<endl<<pssmatrix.score(sequence);
        if (pssmatrix.classify(sequence)) cout<<" -> should contain cleavage site";
        cout<<endl;
    }
	
	return 0;
}


