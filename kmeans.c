#include "kmeans.h"

void updateCentroid(double** centroids, double** clusters, int i, int dim, int n, double**vectorsK);
void assignVectorToCluster(double** vectorsK,double** centroids,double** clusters, int vectorIndex, int dim, int n);
double** matrixAllocK(int rowNum, int colNum);
void writeToFileK(char* outFileName, int dim, double** centroids);
void freeFuncK();
void restartclusters(double ** p, int n);
void printAnErrorHasOccuredK();
void printInvalidInputK();
void readInputK(char *fileName, double **array);
/* define variables */

int num_of_unchanged_centorids, max_iter,t, N, level = 0, K;

FILE * outFile;

char* vectorsFile, centroidsFile;
double **vectorsK = NULL;
double **centroids = NULL;
double** clusters = NULL;
double *old_centroid = NULL;
char* outputFileName = "outputFileName";

/*For successful running return 0 otherwise 1*/
char* execute2(int k_new, int n,int dim, char* vectorsFile, char* centroidsFile) {
    int i, j, l, g;
    int max_iter = 300;
    double eps = 0;
    N = n;
    K = k_new;
    vectorsK = matrixAllocK(n, dim);
    centroids = matrixAllocK(K, dim);
    readInputK(vectorsFile ,vectorsK);
    readInputK(centroidsFile, centroids);
    clusters = matrixAllocK(K, n);
    old_centroid = malloc(dim * sizeof(double));

    for (i=0;  i< max_iter; ++i){
        num_of_unchanged_centorids = 0;
        for (j=0; j<n; ++j){
            assignVectorToCluster(vectorsK, centroids, clusters, j, dim, n);
            }
        for (l=0; l<K; ++l){
            if (old_centroid == NULL){
                printAnErrorHasOccuredK();
            }
            /* level is now 4 = vectorsK + centroids + clusters + old_centroid */
            level += 1;
            for (g=0 ; g < dim ; g++)
            {
                old_centroid[g] = centroids[l][g];
            }
            updateCentroid(centroids, clusters, l, dim, n, vectorsK);
            if (sqrt(Distance(old_centroid, centroids[l], dim)) < eps){
                num_of_unchanged_centorids += 1;
                }
            }
        restartclusters(clusters, n);
        if (num_of_unchanged_centorids == K){
            break;
        }
    }

    writeToFileK(outputFileName, dim, centroids);

    freeFuncK();
    return outputFileName;
       
}

void freeFuncK() {
    /* level 0 */
    free(old_centroid);
    
    if ((level >= 1) && (vectorsK != NULL)) {
        freeMatrix(vectorsK, N);
    }
    free(vectorsK);
    if ((level >= 2) && (centroids != NULL)) {
        freeMatrix(centroids, K);
    }
    free(centroids);
    if ((level >= 3) && (clusters != NULL)) {
        freeMatrix(clusters, K);
    }
    free(clusters);
    /* level 4 includes 3 if's and free(old_centroid) */
    
}

void printInvalidInputK() {
    freeFuncK();
    printf("Invalid Input!\n");
    exit(1);
}

void printAnErrorHasOccuredK() {
    freeFuncK();
    printf("An Error Has Occurred\n");
    exit(1);
}

/* updates centroid */
void updateCentroid(double** centroids, double** clusters, int i, int dim, int n, double**vectorsK){
    int j;
    for (j=0 ; j<dim;++j){
        int cluster_size = 0;
        double sum = 0.0;
        int r=0;
        int vector_ind;
        while((clusters[i][r] != -1.0) && (r < n)){
            vector_ind = clusters[i][r];
            sum += vectorsK[vector_ind][j];
            cluster_size+=1;
            r++;
        }
        if (cluster_size == 0){
            printAnErrorHasOccuredK();
        }
        centroids[i][j] = sum/cluster_size;
    }
}

/* assigns vector to closest cluster */
void assignVectorToCluster(double** vectorsK,double** centroids,double** clusters, int vectorIndex, int dim, int n){
    double min = DBL_MAX;
    int index = 0;
    int i;
    int r;
    double distance;
    for (i=0; i < K;++i){
        distance = Distance(vectorsK[vectorIndex], centroids[i], dim);
        if (distance < min){
            min = distance;
            index = i;}}
    for (r=0; r < n; ++r){
        if (clusters[index][r] == -1.0){
            clusters[index][r] = vectorIndex;
            break;
        }
    }

}

void writeToFileK(char* outFileName, int dim, double** centroids){
    int row, col;
    outFile = fopen(outFileName, "w");
    for (row = 0; row < K; row++) {
        for (col = 0; col < dim; col++) {
            if (col != (dim - 1)) {
                fprintf(outFile, "%.4f,", centroids[row][col]);
            } else {
                fprintf(outFile, "%.4f\n", centroids[row][col]);
            }
        }
    }

    if (fclose(outFile) == EOF) {
        printAnErrorHasOccuredK();
    }
}


void restartclusters(double ** p, int n) {
    int i;
    int j;
    for (i = 0; i < K; i++) {
        for (j = 0; j < n; j++){
            p[i][j] = -1;
        }
    }
}

/* Memory allocation for 2D doubles matrix */
double** matrixAllocK(int rowNum, int colNum) {
    int i, j;
    double **p = (double **)malloc(rowNum * sizeof(double*));
    if (p == NULL) {
        printAnErrorHasOccuredK();
    }
    nullMatrix(rowNum, p);
    level +=1;
    for (i=0; i < rowNum; i++) {
        p[i] = (double*)malloc(colNum * sizeof(double));
        if (p[i] == NULL){
            printAnErrorHasOccuredK();
        }
        for (j=0; j < colNum; j++) {
            p[i][j] = -1.0;
        }

    }
    return p;
}


void readInputK(char *fileName, double **array)
{
    
    double coordinate;
    char comma;
    FILE *fptr;
    int row = 0, col = 0;
    array[0][0] = 0;
    if (!(fptr = fopen(fileName, "r"))) {
        printInvalidInputK();  
    }
    while (fscanf(fptr, "%lf%c", &coordinate, &comma) == 2) {
        array[row][(col)++] = coordinate;
        if (comma == '\n' || comma == '\r') {
            /*fscanf(fptr, "%c", &comma);*/
            row ++;
            col = 0;
        }
        else if (comma != ',') {
            printAnErrorHasOccuredK(); 
        }
    }
    fclose(fptr);
}
