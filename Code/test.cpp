#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "Dataset.hpp"

int main(int argc, const char * argv[]){

    // DATASET TEST

	Dataset dataset("../Data/EUKSIG_13.red");
    
	dataset.show(true);
	
	return 0;
}


