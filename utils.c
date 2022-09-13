#include "utils.h"


/* calculates euclideanDistance between 2 vectors, returns dis */
double Distance(double* x, double* y,int dim) {
    double sumOfSquaredDiffrence = 0;
    int i;
    for (i=0; i < dim; ++i){
        sumOfSquaredDiffrence += pow(x[i]-y[i],2);}
    return sumOfSquaredDiffrence;
}

void freeMatrix(double** matrix, int rowNum) {
    int i;
    for (i=0; i < rowNum; i++) {
        free(matrix[i]);
    }
}

void nullMatrix(int n, double ** p) {
    int i;
    for (i = 0; i < n; i++) {
        p[i] = NULL;
    }
}
