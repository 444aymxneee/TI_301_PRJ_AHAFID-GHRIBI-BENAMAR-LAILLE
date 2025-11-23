#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

t_matrix createEmptyMatrix(int rows, int cols) {
    t_matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = (float**)malloc(rows * sizeof(float*));
    for (int i = 0; i < rows; i++) {
        m.data[i] = (float*)calloc(cols, sizeof(float)); // calloc initialise à 0
    }
    return m;
}

void freeMatrix(t_matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        free(m->data[i]);
    }
    free(m->data);
    m->rows = 0;
    m->cols = 0;
}

t_matrix matrixFromGraph(ListeAdj *g) {
    t_matrix m = createEmptyMatrix(g->taille, g->taille);
    
    for (int i = 0; i < g->taille; i++) {
        cell *curr = g->tableau[i].head;
        while (curr != NULL) {
            int j = curr->sommet_arrivee - 1;
            m.data[i][j] = curr->proba;
            curr = curr->suiv;
        }
    }
    return m;
}

void displayMatrix(t_matrix m) {
    for (int i = 0; i < m.rows; i++) {
        printf("| ");
        for (int j = 0; j < m.cols; j++) {
            printf("%5.2f ", m.data[i][j]);
        }
        printf("|\n");
    }
}

void copyMatrix(t_matrix dest, t_matrix src) {
    if (dest.rows != src.rows || dest.cols != src.cols) return;
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            dest.data[i][j] = src.data[i][j];
        }
    }
}

t_matrix multiplyMatrices(t_matrix a, t_matrix b) {
    if (a.cols != b.rows) {
        printf("Erreur dimensions multiplication\n");
        exit(1);
    }
    t_matrix res = createEmptyMatrix(a.rows, b.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            float sum = 0;
            for (int k = 0; k < a.cols; k++) {
                sum += a.data[i][k] * b.data[k][j];
            }
            res.data[i][j] = sum;
        }
    }
    return res;
}

float diffMatrix(t_matrix m, t_matrix n) {
    float diff = 0;
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            diff += fabsf(m.data[i][j] - n.data[i][j]);
        }
    }
    return diff;
}


t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index) {
    t_classe *cls = &part.tab_classes[compo_index];
    int size = 0;
    t_noeud_sommet *curr = cls->sommets;
    while(curr) {
        size++;
        curr = curr->suiv;
    }

    int *indices = (int*)malloc(size * sizeof(int));
    curr = cls->sommets;
    int idx = 0;
    while(curr) {
        indices[idx++] = curr->val - 1;
        curr = curr->suiv;
    }

    t_matrix sub = createEmptyMatrix(size, size);
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            sub.data[i][j] = matrix.data[indices[i]][indices[j]];
        }
    }
    
    free(indices);
    return sub;
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int ged_array(int *vals, int nbvals) {
    if (nbvals == 0) return 0;
    int result = vals[0];
    for (int i = 1; i < nbvals; i++) {
        result = gcd(result, vals[i]);
    }
    return result;
}

int getPeriod(t_matrix sub_matrix) {
    int n = sub_matrix.rows;
    int *periods = (int*)malloc(n * sizeof(int) + 100); 
    int period_count = 0;

    t_matrix power_matrix = createEmptyMatrix(n, n);
    t_matrix result_matrix = createEmptyMatrix(n, n);

    copyMatrix(power_matrix, sub_matrix);

    for (int k = 1; k <= n; k++) {
        int diag_nonzero = 0;
        for (int i = 0; i < n; i++) {
            if (power_matrix.data[i][i] > 0.0001f) {
                diag_nonzero = 1;
                break;
            }
        }

        if (diag_nonzero) {
            periods[period_count++] = k;
        }

        t_matrix temp = multiplyMatrices(power_matrix, sub_matrix);

        copyMatrix(power_matrix, temp);
        freeMatrix(&temp);
    }

    int d = ged_array(periods, period_count);
    
    free(periods);
    freeMatrix(&power_matrix);
    freeMatrix(&result_matrix);
    
    return d;
}