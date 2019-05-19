#include "SVMModel.hpp"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

struct svm_problem SVMModel::datasetToProblem(Dataset dataset, int p, int q) {
    struct svm_problem problem;
    struct svm_node* x_nodes;
    this->p = p;
    this->q = q;

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

SVMModel::SVMModel(Dataset dataset, int p, int q, int kernel_type, double C, double gamma) {

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

    // Reading problem
    struct svm_problem problem = datasetToProblem(dataset, p, q);
    

    // Training
    model = svm_train(&problem, &param);

}

SVMModel::~SVMModel(){
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