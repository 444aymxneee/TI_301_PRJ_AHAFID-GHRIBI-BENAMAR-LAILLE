#ifndef MATRIX_H
#define MATRIX_H

#include "graphe.h"
#include "tarjan.h"

typedef struct {
    int rows;
    int cols;
    float **data;
} t_matrix;

t_matrix createEmptyMatrix(int rows, int cols);
t_matrix matrixFromGraph(ListeAdj *g);
void freeMatrix(t_matrix *m);
void displayMatrix(t_matrix m);
void copyMatrix(t_matrix dest, t_matrix src);
t_matrix multiplyMatrices(t_matrix a, t_matrix b);
float diffMatrix(t_matrix m, t_matrix n);

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);

int gcd(int a, int b);
int getPeriod(t_matrix sub_matrix);

#endif