#include "Dataset.hpp"

Dataset::Dataset(const char* file) {

	ifstream input(file);
	
	if (input.fail()) {
		cout<<"Cannot read from file "<<file<<" !"<<endl;
		exit(1);
	}
	
	Data row;
    string line;

	while (getline(input, line)) {
        // each entry is composed of three line
        // first line is ignored

        getline(input, line); // second line gives the structure of the protein
        row.sequence = line;

        getline(input, line); // third line gives the localization of the cleavage site
        for (int i=0; i<line.size(); i++) {
            if (line.at(i) == 'C') row.cleavage = i;
        }
        datas.push_back(row);
    }
	
	input.close();
}

Dataset::~Dataset() {
}

int Dataset::size() {
    return datas.size();
}

Data Dataset::get(int i) {
    assert ((i >= 0) && (i < datas.size()));
    return datas[i];
}

void Dataset::show(bool verbose) {
	std::cout<<"Dataset with "<<this->size()<<" samples."<<std::endl;

    string sequence;
    int cleavage;

	if (verbose) {
		for (int i=0; i<datas.size(); i++) {
            sequence = this->get(i).sequence;
            cleavage = this->get(i).cleavage;
			for (int j=0; j<sequence.size(); j++){
                if (j == cleavage) {
                    cout<<" ";
                }
                cout<<sequence.at(j);
            }
            cout<<std::endl;
		}		
	}
}