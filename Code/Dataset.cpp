#include "Dataset.hpp"

Dataset::Dataset(const char* file, int start, int end) {

	ifstream input(file);
	
	if (input.fail()) {
		cout<<"Cannot read from file "<<file<<" !"<<endl;
		exit(1);
	}
	
	Data row;
    string line;

    int counter = 0;

	while (getline(input, line)) {
        if (counter >= start && (end == -1 || counter < end)) {
            // each entry is composed of three line
            // first line is ignored

            getline(input, line); // second line gives the structure of the protein
            row.sequence = line;

            getline(input, line); // third line gives the localization of the cleavage site
            for (int i=0; i<line.size(); i++) {
                if (line.at(i) == 'C') row.cleavage = i;
            }
            datas.push_back(row);
        } else {
            getline(input, line);
            getline(input, line);
        }
        counter ++;
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

void Dataset::writeForSVM(const char* file, int p, int q){

    ofstream output(file);
	
	if (output.fail()) {
		cout<<"Cannot write on file "<<file<<" !"<<endl;
		exit(1);
	}

    for (int i=0; i<this->size(); i++) {
        Data data = this->get(i);
        for (int j=0; j<data.sequence.size() - (p+q); j++) {
            if (j+p == data.cleavage) {
                output << "+1 ";
            } else {
                output << "-1 ";
            }
            for (int k=0; k<p+q; k++) {
               output << (26*k) + (data.sequence.at(j+k) - 65);
                output << ":" << 1 << " ";
            }
            output << endl;
        }
    }
}