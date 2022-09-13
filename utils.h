#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <string.h>

/* methods */

double Distance(double* x, double* y,int dim);
void freeMatrix(double** matrix, int rowNum);
void nullMatrix(int n, double ** p);

#endif


