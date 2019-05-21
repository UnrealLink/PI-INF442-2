#include "SVMModel.hpp"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

struct svm_problem SVMModel::datasetToProblem(Dataset dataset) {
    struct svm_problem problem;
    struct svm_node* x_nodes;

    problem.l = 0;
    for (int i=0; i<dataset.size(); i++) {
        Data data = dataset.get(i);
        problem.l += data.sequence.size() - (p+q);
    }

    problem.y = Malloc(double, problem.l);
	problem.x = Malloc(struct svm_node *,problem.l);
	x_nodes = Malloc(struct svm_node, problem.l*(p+q+1));

    int counter = 0;
    for (int i=0; i<dataset.size(); i++) {
        Data data = dataset.get(i);
        for (int j=0; j<data.sequence.size() - (p+q); j++) {
            problem.x[counter] = &x_nodes[counter*(p+q+1)];
            if (j+p == data.cleavage) {
                problem.y[counter] = 1;
            } else {
                problem.y[counter] = -1;
            }
            for (int k=0; k<p+q; k++) {
                x_nodes[counter*(p+q+1)+k].index = (26*k) + (data.sequence.at(j+k) - 65);
                x_nodes[counter*(p+q+1)+k].value = 1;
            }
            x_nodes[counter*(p+q+1)+p+q].index = -1;
            counter++;
        }
    }
    return problem;
}

struct SmallData {
        int label;
        string sequence;
};

struct svm_problem SVMModel::datasetToProblemSubstitution(Dataset dataset, const char* matrix_file) {
    struct svm_problem problem;
    struct svm_node* x_nodes;

    problem.l = 0;
    for (int i=0; i<dataset.size(); i++) {
        Data data = dataset.get(i);
        problem.l += data.sequence.size() - (p+q);
    }

    substitutionMatrixInit(matrix_file);

    vector<SmallData> precomputedDatas;

    Data data;
    string sequence;

    for (int i=0; i < dataset.size(); i++) {
        Data data = dataset.get(i);
        string sequence;
        for (int j = p; j<data.sequence.size()-q; j++) {
            SmallData smalldata;
            sequence = data.sequence.substr(j-p, p+q);
            smalldata.sequence = sequence;
            if (j == data.cleavage) {
                smalldata.label = 1;
            } else {
                smalldata.label = -1;
            }
            precomputedDatas.push_back(smalldata);
        }
    }

    problem.y = Malloc(double, problem.l);
	problem.x = Malloc(struct svm_node *, problem.l);
    size_t len = (long)problem.l*(problem.l+2);
	x_nodes = Malloc(struct svm_node, len);

    for (long i=0; i<problem.l; i++) {
        problem.y[i] = precomputedDatas[i].label;
        problem.x[i] = &x_nodes[i*(problem.l+2)];
        x_nodes[i*(problem.l+2)].index = 0;
        x_nodes[i*(problem.l+2)].value = i;
        for (int j=0; j<problem.l; j++) {
            x_nodes[i*(problem.l+2)+j+1].index = j+1;
            x_nodes[i*(problem.l+2)+j+1].value = substitutionMatrix(precomputedDatas[i].sequence, precomputedDatas[j].sequence);
        }
        x_nodes[i*(problem.l+2)+problem.l].index = -1;
    }

    return problem;
}

double SVMModel::substitutionMatrix(string a, string b) {
    assert(a.size() == b.size());
    int score = 0;
    for (int i=0; i<a.size(); i++) {
        score += matrix[a.at(i)][b.at(i)] / (matrix_max*a.size());
    }
    return score;
}

void SVMModel::substitutionMatrixInit(const char* file) {
    
    stringstream input;


    if (file != NULL) {
        ifstream input_file(file);
        
        if (input_file.fail()) {
            cout<<"Cannot read from file "<<file<<" !"<<endl;
            exit(1);
        }

        copy(istreambuf_iterator<char>(input_file),
             istreambuf_iterator<char>(),
             ostreambuf_iterator<char>(input));
    } else {
        input = stringstream(\
"#  Matrix made by matblas from blosum62.iij\n"
"#  * column uses minimum score \n"
"#  BLOSUM Clustered Scoring Matrix in 1/2 Bit Units \n"
"#  Blocks Database = /data/blocks_5.0/blocks.dat \n"
"#  Cluster Percentage: >= 62 \n"
"#  Entropy =   0.6979, Expected =  -0.5209 \n"
"   A  R  N  D  C  Q  E  G  H  I  L  K  M  F  P  S  T  W  Y  V  B  Z  X  * \n"
"A  4 -1 -2 -2  0 -1 -1  0 -2 -1 -1 -1 -1 -2 -1  1  0 -3 -2  0 -2 -1  0 -4 \n"
"R -1  5  0 -2 -3  1  0 -2  0 -3 -2  2 -1 -3 -2 -1 -1 -3 -2 -3 -1  0 -1 -4 \n"
"N -2  0  6  1 -3  0  0  0  1 -3 -3  0 -2 -3 -2  1  0 -4 -2 -3  3  0 -1 -4 \n"
"D -2 -2  1  6 -3  0  2 -1 -1 -3 -4 -1 -3 -3 -1  0 -1 -4 -3 -3  4  1 -1 -4 \n"
"C  0 -3 -3 -3  9 -3 -4 -3 -3 -1 -1 -3 -1 -2 -3 -1 -1 -2 -2 -1 -3 -3 -2 -4 \n"
"Q -1  1  0  0 -3  5  2 -2  0 -3 -2  1  0 -3 -1  0 -1 -2 -1 -2  0  3 -1 -4 \n"
"E -1  0  0  2 -4  2  5 -2  0 -3 -3  1 -2 -3 -1  0 -1 -3 -2 -2  1  4 -1 -4 \n"
"G  0 -2  0 -1 -3 -2 -2  6 -2 -4 -4 -2 -3 -3 -2  0 -2 -2 -3 -3 -1 -2 -1 -4 \n"
"H -2  0  1 -1 -3  0  0 -2  8 -3 -3 -1 -2 -1 -2 -1 -2 -2  2 -3  0  0 -1 -4 \n"
"I -1 -3 -3 -3 -1 -3 -3 -4 -3  4  2 -3  1  0 -3 -2 -1 -3 -1  3 -3 -3 -1 -4 \n"
"L -1 -2 -3 -4 -1 -2 -3 -4 -3  2  4 -2  2  0 -3 -2 -1 -2 -1  1 -4 -3 -1 -4 \n"
"K -1  2  0 -1 -3  1  1 -2 -1 -3 -2  5 -1 -3 -1  0 -1 -3 -2 -2  0  1 -1 -4 \n"
"M -1 -1 -2 -3 -1  0 -2 -3 -2  1  2 -1  5  0 -2 -1 -1 -1 -1  1 -3 -1 -1 -4 \n"
"F -2 -3 -3 -3 -2 -3 -3 -3 -1  0  0 -3  0  6 -4 -2 -2  1  3 -1 -3 -3 -1 -4 \n"
"P -1 -2 -2 -1 -3 -1 -1 -2 -2 -3 -3 -1 -2 -4  7 -1 -1 -4 -3 -2 -2 -1 -2 -4 \n"
"S  1 -1  1  0 -1  0  0  0 -1 -2 -2  0 -1 -2 -1  4  1 -3 -2 -2  0  0  0 -4 \n"
"T  0 -1  0 -1 -1 -1 -1 -2 -2 -1 -1 -1 -1 -2 -1  1  5 -2 -2  0 -1 -1  0 -4 \n"
"W -3 -3 -4 -4 -2 -2 -3 -2 -2 -3 -2 -3 -1  1 -4 -3 -2 11  2 -3 -4 -3 -2 -4 \n"
"Y -2 -2 -2 -3 -2 -1 -2 -3  2 -1 -1 -2 -1  3 -3 -2 -2  2  7 -1 -3 -2 -1 -4 \n"
"V  0 -3 -3 -3 -1 -2 -2 -3 -3  3  1 -2  1 -1 -2 -2  0 -3 -1  4 -3 -2 -1 -4 \n"
"B -2 -1  3  4 -3  0  1 -1  0 -3 -4  0 -3 -3 -2  0 -1 -4 -3 -3  4  1 -1 -4 \n"
"Z -1  0  0  1 -3  3  4 -2  0 -3 -3  1 -1 -3 -1  0 -1 -3 -2 -2  1  4 -1 -4 \n"
"X  0 -1 -1 -1 -2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -2  0  0 -2 -1 -1 -1 -1 -1 -4 \n"
"* -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4  1 \n");
    }

    string line = "#";

    while (line.at(0) == '#') {
        getline(input, line);
    }

    vector<char> order;

    for (int i=0; i<line.size(); i++){
        if (line.at(i) != ' ') {
            order.push_back(line.at(i));
        }
    }

    while (getline(input, line)) {
        stringstream s(line);
        string splitline;
        getline(s, splitline, ' ');
        char acide = splitline.at(0);
        int i = 0;
        while (getline(s, splitline, ' ')) {
            if (splitline == "") continue;
            matrix[acide][order[i]] = atof(splitline.c_str());
            if (abs(atof(splitline.c_str())) > matrix_max) matrix_max = atof(splitline.c_str());
            i++;
        }
    }
}


SVMModel::SVMModel(Dataset dataset, int p, int q, struct svm_parameter param, const char* matrix_file) {

    this->param = param;
    this->p = p;
    this->q = q;

    struct svm_problem problem;

    // Reading problem
    if (param.kernel_type == PRECOMPUTED) {
        problem = datasetToProblemSubstitution(dataset, matrix_file);

    } else {
        problem = datasetToProblem(dataset);
    }
    
    // Training
    model = svm_train(&problem, &param);
    
}

SVMModel::~SVMModel() {
}

bool SVMModel::classify(string sequence) {
    
    assert(sequence.size() == p+q);

    // Creating Nodes
    struct svm_node* nodes = Malloc(struct svm_node, p+q);
    for (int k=0; k<p+q; k++) {
        nodes[k].index = (26*k) + (sequence.at(k) - 65);
        nodes[k].value = 1;
    }
    nodes[p+q].index = -1;

    // Predicting label
    double result = svm_predict(model, nodes);

    free(nodes);

    return (result > 0);
}