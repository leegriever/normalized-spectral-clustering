#ifndef SPKMEANS_H
#define SPKMEANS_H

#include "utils.h"

/* struct */
typedef struct spk_info
{
    int k;
    char * spk_mat_filename;

} SPK_INFO;

/* define main methods */
SPK_INFO* execute1(int kToCheck, char *goal, char *filename);





#endif
