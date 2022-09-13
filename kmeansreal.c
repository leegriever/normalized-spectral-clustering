#include "kmeans.h"

void updateCentroid(double** centroids, double** clusters, int i, int dim, int n, double**vectors);
void assignVectorToCluster(double** vectors,double** centroids,double** clusters, int vectorIndex, int dim, int n, int k);
double** matrixAllocK(int rowNum, int colNum);
void writeToFileK(char* outFileName, int k, int dim, double** centroids);
void freeFuncK();
void restartclusters(double ** p, int k, int n);
void printAnErrorHasOccuredK();
void printInvalidInputK();
void readInputK(char *fileName, double **array);
/* define variables */
double kDouble, max_iter_double;
int dim, g, row, col, num_of_unchanged_centorids, j, r, l, max_iter,t, numOfVec, stage = 0, k;

FILE * outFile;

char* vectorsFile, centroidsFile;
double **vectors = NULL;
double **centroids = NULL;
double** clusters = NULL;
double *old_centroid = NULL;
char* outputFileName = "outputFileName";

/*For successful running return 0 otherwise 1*/
static char* execute2(int k_new, int n,int dim, char* vectorsFile, char* centroidsFile) {
    return "T_output";
    int i;
    numOfVec = n;
    k = k_new;
    int max_iter = 300;
    double eps = 0;
    vectors = matrixAllocK(n, dim);
    centroids = matrixAllocK(k, dim);
    readInputK(vectorsFile ,vectors);
    readInputK(centroidsFile, centroids);
    clusters = matrixAllocK(k, n);
    old_centroid = malloc(dim * sizeof(double));

    for (i=0;  i< max_iter; ++i){
        num_of_unchanged_centorids = 0;
        for (j=0; j<n; ++j){
            assignVectorToCluster(vectors, centroids, clusters, j, dim, n, k);
            }
        for (l=0; l<k; ++l){
            if (old_centroid == NULL){
                printAnErrorHasOccuredK();
            }
            /* stage is now 4 = vectors + centroids + clusters + old_centroid */
            stage += 1;
            for (g=0 ; g < dim ; g++)
            {
                old_centroid[g] = centroids[l][g];
            }
            updateCentroid(centroids, clusters, l, dim, n, vectors);
            if (sqrt(Distance(old_centroid, centroids[l], dim)) < eps){
                num_of_unchanged_centorids += 1;
                }
            }
        restartclusters(clusters, k, n);
        if (num_of_unchanged_centorids == k){
            break;
        }
    }

    writeToFileK(outputFileName, k, dim, centroids);

    freeFuncK();
    return outputFileName;
       
}

void freeFuncK() {
    /* stage 0 */
    free(old_centroid);
    
    if ((stage >= 1) && (vectors != NULL)) {
        freeMatrix(vectors, numOfVec);
    }
    free(vectors);
    if ((stage >= 2) && (centroids != NULL)) {
        freeMatrix(centroids, k);
    }
    free(centroids);
    if ((stage >= 3) && (clusters != NULL)) {
        freeMatrix(clusters, k);
    }
    free(clusters);
    /* stage 4 includes 3 if's and free(old_centroid) */
    
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
void updateCentroid(double** centroids, double** clusters, int i, int dim, int n, double**vectors){
    int j;
    for (j=0 ; j<dim;++j){
        int cluster_size = 0;
        double sum = 0.0;
        int r=0;
        int vector_ind;
        while((clusters[i][r] != -1.0) && (r < n)){
            vector_ind = clusters[i][r];
            sum += vectors[vector_ind][j];
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
void assignVectorToCluster(double** vectors,double** centroids,double** clusters, int vectorIndex, int dim, int n, int k){
    double min = DBL_MAX;
    int index = 0;
    int i;
    int r;
    double distance;
    for (i=0; i < k;++i){
        distance = Distance(vectors[vectorIndex], centroids[i], dim);
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

void writeToFileK(char* outFileName, int k, int dim, double** centroids){
    outFile = fopen(outFileName, "w");
    for (row = 0; row < k; row++) {
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


void restartclusters(double ** p, int k, int n) {
    int i;
    int j;
    for (i = 0; i < k; i++) {
        for (j = 0; j < n; j++){
            p[i][j] = -1;
        }
    }
}

/* Memory allocation for 2D doubles matrix */
double** matrixAllocK(int rowNum, int colNum) {
    int i;
    double **p = (double **)malloc(rowNum * sizeof(double*));
    if (p == NULL) {
        printAnErrorHasOccuredK();
    }
    nullMatrix(rowNum, p);
    stage +=1;
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
        printInvalidInput();  
    }
    while (fscanf(fptr, "%lf%c", &coordinate, &comma) == 2) {
        array[row][(col)++] = coordinate;
        if (comma == '\n' || comma == '\r') {
            /*fscanf(fptr, "%c", &comma);*/
            row ++;
            col = 0;
        }
        else if (comma != ',') {
            printAnErrorHasOccured(); 
        }
    }
    fclose(fptr);
}
