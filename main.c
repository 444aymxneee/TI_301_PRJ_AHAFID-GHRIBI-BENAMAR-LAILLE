#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"
#include "tarjan.h"
#include "matrix.h"

int main() {
    ListeAdj g = readGraph("../exemple.txt");

    printf("\n\n----- LISTE D'ADJACENCE -----\n");
    afficherListeAdj(g);

    printf("\n\n----- VERIFICATION MARKOV -----\n");
    estGrapheMarkov(&g);

    printf("\n\n----- EXPORT MERMAID -----\n");
    exportMermaid(g, "graphe_mermaid.txt");

    printf("\n\n----- ALGORITHME DE TARJAN -----\n");
    t_partition partition = tarjan(&g);
    afficherPartition(partition);

    printf("\n\n----- DIAGRAMME DE HASSE ET PROPRIETES -----\n");
    analyseHasse(&g, &partition);

    printf("\n\n----- MATRICE DE TRANSITION M -----\n");
    t_matrix M = matrixFromGraph(&g);
    displayMatrix(M);
    printf("\n--- Calcul de M^3 (Probabilites a J+3) ---\n");
    t_matrix M2 = multiplyMatrices(M, M);
    t_matrix M3 = multiplyMatrices(M2, M);
    displayMatrix(M3);
    printf("\n--- Calcul de M^7 (Probabilites a J+7) ---\n");
    t_matrix M4 = multiplyMatrices(M2, M2);
    t_matrix M7 = multiplyMatrices(M4, M3);
    displayMatrix(M7);
    printf("\n--- Convergence (diff(M^n, M^(n-1)) < epsilon) ---\n");

    t_matrix M_prev = createEmptyMatrix(M.rows, M.cols);
    copyMatrix(M_prev, M);

    int n = 2;
    float epsilon = 0.01;
    float diff = 1.0;
    int max_iter = 1000;

    t_matrix M_curr;

    while (diff > epsilon && n < max_iter) {
        M_curr = multiplyMatrices(M_prev, M);

        diff = diffMatrix(M_curr, M_prev);
        freeMatrix(&M_prev);

        M_curr.rows = M.rows;
        M_curr.cols = M.cols;
        M_prev = M_curr;

        n++;
    }

    if (n >= max_iter) {
        printf("Pas de convergence stricte a partir de %d iterations (diff=%.4f).\n", max_iter, diff);
    } else {
        printf("Convergence atteinte a n = %d avec diff = %.4f\n", n-1, diff);
        printf("Matrice limite approximative :\n");
        displayMatrix(M_prev);
    }

    freeMatrix(&M_prev);
    freeMatrix(&M2); freeMatrix(&M3); freeMatrix(&M4); freeMatrix(&M7);
    printf("\n\n----- ANALYSE PAR CLASSE -----\n");

    for(int i = 0; i < partition.nb_classes; i++) {
        t_classe *c = &partition.tab_classes[i];
        printf("\nAnalyse de la composante %s (Taille: %d sommets) <<<\n", c->nom, (c->sommets ? 1 : 0));
        t_matrix subM = subMatrix(M, partition, i);

        if (subM.rows > 0) {
            printf("Sous-matrice associee :\n");
            displayMatrix(subM);

            int periode = getPeriod(subM);

            if (periode > 1) {
                printf("Class Periodique (d = %d)\n", periode);
                printf(" -> Pas de distribution de limite unique, mais cyclique.\n");
            }
            else {
                printf("Classe aperiodique (Periode = 1)\n");
                if (c->est_puits) {
                    printf("Classe Persistante et aperiodique\n");
                    t_matrix SM_state = createEmptyMatrix(subM.rows, subM.cols);
                    copyMatrix(SM_state, subM);
                    t_matrix SM_temp;

                    for(int k=0; k<50; k++) {
                         SM_temp = multiplyMatrices(SM_state, subM);
                         freeMatrix(&SM_state);
                         SM_state = SM_temp;
                    }

                    printf("Distribution stationnaire : (");
                    for(int j=0; j<SM_state.cols; j++) {
                        printf("%.3f ", SM_state.data[0][j]);
                    }
                    printf(")\n");
                    freeMatrix(&SM_state);
                } else {
                    printf(" Classe Transitoire : Probabilites tendent vers 0.\n");
                }
            }
            freeMatrix(&subM);
        } else {
            printf("Classe vide ou erreur d'extraction.\n");
        }
    }
    freeMatrix(&M);
    return 0;
}