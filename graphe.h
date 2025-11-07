#ifndef GRAPHE_H
#define GRAPHE_H
#include "list.h"

typedef struct {
    int taille;
    Liste *tableau;
} ListeAdj;

ListeAdj creerListeAdj(int taille);
void afficherListeAdj(ListeAdj g);
ListeAdj readGraph(const char *filename);
int estGrapheMarkov(ListeAdj *g);
char *getId(int num);
void exportMermaid(ListeAdj g, const char *filename);

#endif
