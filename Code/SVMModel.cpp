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

struct svm_problem SVMModel::datasetToProblemSubstitution(Dataset dataset, const char* matrix) {
    struct svm_problem problem;
    struct svm_node* x_nodes;

    problem.l = 0;
    for (int i=0; i<dataset.size(); i++) {
        Data data = dataset.get(i);
        problem.l += data.sequence.size() - (p+q);
    }

    substitutionMatrixInit(matrix);

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
    
    ifstream input(file);
	
	if (input.fail()) {
		cout<<"Cannot read from file "<<file<<" !"<<endl;
		exit(1);
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
        std::stringstream s(line);
        string splitline;
        getline(s, splitline, ' ');
        char acide = splitline.at(0);
        int i = 0;
        while (getline(s, splitline, ' ')) {
            matrix[acide][order[i]] = atof(splitline.c_str());
            if (abs(atof(splitline.c_str())) > matrix_max) matrix_max = atof(splitline.c_str());
        }
    }
}


SVMModel::SVMModel(Dataset dataset, int p, int q, int kernel_type, double C, double gamma, const char* matrix) {

    // Setting parameters
    param.svm_type = C_SVC;
	param.kernel_type = kernel_type;
	param.degree = 3;
	param.gamma = gamma;	// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = C;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

    this->p = p;
    this->q = q;

    struct svm_problem problem;

    // Reading problem
    if (kernel_type == PRECOMPUTED) {
        problem = datasetToProblemSubstitution(dataset, matrix);
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

    return (result > 0);
}