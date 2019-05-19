#include "PSSMatrix.hpp"
#include <math.h>

PSSMatrix::PSSMatrix(Dataset dataset, int p, int q, double threshold) {
    this->p = p;
    this->q = q;

    map<char, vector<double> > f; // f[a][i+p] frequency of amino acid a at relative position i
    map<char, double> g; // g[a] frequency of amino acid a

    Data data;
    char a;

    int N = dataset.size();
    int N_total = dataset.size()*(p+q);
    for (int i=0; i<N; i++) {
        data = dataset.get(i);
        for (int j=-p; j<q; j++) {

            a = data.sequence.at(data.cleavage + j);
            map<char, vector<double> >::iterator f_it = f.find(a);
            map<char, double>::iterator g_it = g.find(a);

            if (f_it == f.end()) {
                f[a] = vector<double>(p+q, 1./ (N + 26)); // frequency is not initialized at 0 for log
            }
            if (g_it != g.end()) {
                g[a] = 1./(N_total + 26);// frequency is not initialized at 0 for log
            }

            f[a][p+j] += 1./ (N + 26);
            if (data.cleavage+j != 0) { // the first amino acide is not taken into account
                g[a] += 1./(N_total + 26);
            }
        }
    }

    map<char, vector<double> >::iterator f_it;

    for (f_it=f.begin(); f_it != f.end(); f_it++) {
        w[f_it->first] = f_it->second;
        for (int i=-p; i<q; i++) {
            w[f_it->first][p+i] = log(f_it->second[p+i]) - log(g[f_it->first]);
        }
    }
    
    this->threshold = threshold;
    optimize_threshold(dataset, threshold-5, threshold+5, 0.1);

}

PSSMatrix::~PSSMatrix() {
}

double PSSMatrix::score(string sequence) {
    double s = 0.;
    for (int i=0; i<p+q; i++) {
        s += w[sequence.at(i)][i];
    }
    return s;
}

bool PSSMatrix::classify(string sequence) {
    double s = score(sequence);
    return s>threshold;
}

void PSSMatrix::optimize_threshold(Dataset dataset, double start, double end, double step) {
    double best_score = 0;
    double best_t = start;
    
    for (double t=start; t<end; t += step) {

        this->threshold = t;

        double tp = 0;
        double fp = 0;
        double tn = 0;
        double fn = 0;
        Data data;
        string sequence;
        
        for (int i=0; i < dataset.size(); i++) {
            Data data = dataset.get(i);
            string sequence;
            for (int j = p; j<data.sequence.size()-q; j++) {
                sequence = data.sequence.substr(j-p, p+q);
                if (this->classify(sequence)) {
                    if (j == data.cleavage) {tp++;} else {fp++;}
                } else {
                    if (j == data.cleavage) {fn++;} else {tn++;}
                }
            }
        }

        double precision = tp/(tp+fp);
        double recall = tp/(tp+fn);
        double fscore = (2*precision*recall)/(precision+recall);

        if (fscore > best_score) {best_score = fscore; best_t = t;}
    }

    this->threshold=best_t;
}