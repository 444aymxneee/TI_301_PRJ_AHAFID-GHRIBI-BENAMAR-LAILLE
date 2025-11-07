#include <stdio.h>
#include "graphe.h"

void main() {
    ListeAdj g = readGraph("../exemple.txt");
    printf("\n\n----- LISTE ADJACENCE -----\n");
    afficherListeAdj(g);

    printf("\n\n----- VERIFICATION -----\n");
    estGrapheMarkov(&g);

    printf("\n \n----- EXPORT MERMAID -----\n");
    exportMermaid(g, "graphe_mermaid.txt");

    return;
}
